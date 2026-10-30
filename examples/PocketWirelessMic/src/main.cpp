#include <Arduino.h>
#include <PocketSDK.h>
#include <WebServer.h>
#include <WiFi.h>
#include <WiFiUdp.h>

#include <math.h>

#include "WirelessMicProtocol.h"

namespace {

using pocketsdk::Button;

pocketsdk::PocketSettings settings;
WebServer setupServer(80);
WiFiUDP udp;

String wifiSsid;
String wifiPassword;
String receiverHost;
uint16_t receiverPort = pocket_wireless_mic::kDefaultPort;

int16_t audioBuffer[pocket_wireless_mic::kSamplesPerPacket];
bool capturePending = false;
bool setupMode = false;
uint32_t sequenceNumber = 0;
uint32_t packetsSent = 0;

enum class StreamState { Idle, Streaming, Stopping };
StreamState streamState = StreamState::Idle;

String htmlEscape(const String &input) {
    String output;
    output.reserve(input.length() + 8);
    for (const char c : input) {
        if (c == '&') output += F("&amp;");
        else if (c == '<') output += F("&lt;");
        else if (c == '>') output += F("&gt;");
        else if (c == '"') output += F("&quot;");
        else output += c;
    }
    return output;
}

void drawStatus(const char *title, uint16_t color,
                const String &detail = String()) {
    Pocket.display().fillScreen(TFT_BLACK);
    Pocket.display().setTextColor(color, TFT_BLACK);
    Pocket.display().setTextDatum(top_center);
    Pocket.display().setTextSize(2);
    Pocket.display().drawString(title, Pocket.display().width() / 2, 14);
    Pocket.display().setTextColor(TFT_WHITE, TFT_BLACK);
    Pocket.display().setTextSize(1);
    Pocket.display().drawString(detail, Pocket.display().width() / 2, 52);
}

void loadSettings() {
    if (!settings.begin("pwmic", true)) return;
    wifiSsid = settings.getString("ssid");
    wifiPassword = settings.getString("password");
    receiverHost = settings.getString("receiver");
    receiverPort = static_cast<uint16_t>(
        settings.getUInt("port", pocket_wireless_mic::kDefaultPort));
    settings.end();
}

void saveSettings() {
    if (!settings.begin("pwmic")) return;
    settings.putString("ssid", wifiSsid);
    settings.putString("password", wifiPassword);
    settings.putString("receiver", receiverHost);
    settings.putUInt("port", receiverPort);
    settings.end();
}

String setupPage(const String &message = String()) {
    String page;
    page.reserve(1800);
    page += F("<!doctype html><html><head><meta name=viewport content='width=device-width,initial-scale=1'>"
              "<title>Pocket Mic Setup</title><style>body{font-family:system-ui;max-width:34rem;"
              "margin:2rem auto;padding:0 1rem;background:#111;color:#eee}label{display:block;margin-top:1rem}"
              "input{box-sizing:border-box;width:100%;padding:.7rem;font-size:1rem}button{margin-top:1.3rem;"
              "padding:.8rem 1.2rem;font-size:1rem}small{color:#aaa}</style></head><body>"
              "<h1>Pocket Wireless Mic</h1>");
    if (message.length()) page += "<p>" + htmlEscape(message) + "</p>";
    page += F("<form method=post action=/save><label>Wi-Fi network</label><input name=ssid required value='");
    page += htmlEscape(wifiSsid);
    page += F("'><label>Wi-Fi password</label><input name=password type=password value=''>"
              "<label>Receiver IP or hostname</label><input name=receiver required value='");
    page += htmlEscape(receiverHost);
    page += F("'><label>UDP port</label><input name=port type=number min=1 max=65535 value='");
    page += String(receiverPort);
    page += F("'><small>Start pocket_mic_receiver.py on the receiver first.</small><br>"
              "<button type=submit>Save and restart</button></form></body></html>");
    return page;
}

void startSetupMode() {
    setupMode = true;
    WiFi.mode(WIFI_AP);
    WiFi.softAP("Pocket-Mic-Setup");
    setupServer.on("/", HTTP_GET, [] {
        setupServer.send(200, "text/html", setupPage());
    });
    setupServer.on("/save", HTTP_POST, [] {
        wifiSsid = setupServer.arg("ssid");
        const String submittedPassword = setupServer.arg("password");
        if (!submittedPassword.isEmpty() || wifiPassword.isEmpty()) {
            wifiPassword = submittedPassword;
        }
        receiverHost = setupServer.arg("receiver");
        const long port = setupServer.arg("port").toInt();
        receiverPort = port > 0 && port <= 65535
                           ? static_cast<uint16_t>(port)
                           : pocket_wireless_mic::kDefaultPort;
        saveSettings();
        setupServer.send(200, "text/html",
                         setupPage("Saved. The StickS3 is restarting."));
        delay(500);
        ESP.restart();
    });
    setupServer.onNotFound([] {
        setupServer.sendHeader("Location", "/", true);
        setupServer.send(302);
    });
    setupServer.begin();
    drawStatus("SETUP", TFT_YELLOW, "Wi-Fi: Pocket-Mic-Setup");
    Pocket.display().drawString("Open 192.168.4.1",
                                Pocket.display().width() / 2, 72);
}

bool connectWifi() {
    WiFi.mode(WIFI_STA);
    WiFi.setSleep(false);
    WiFi.begin(wifiSsid.c_str(), wifiPassword.c_str());
    drawStatus("CONNECTING", TFT_CYAN, wifiSsid);
    const uint32_t deadline = millis() + 15000;
    while (WiFi.status() != WL_CONNECTED &&
           static_cast<int32_t>(deadline - millis()) > 0) {
        Pocket.update();
        delay(50);
    }
    if (WiFi.status() != WL_CONNECTED) return false;
    udp.begin(0);
    drawStatus("READY", TFT_GREEN, "Hold front button to talk");
    return true;
}

uint16_t calculateLevel(const int16_t *samples, size_t count) {
    uint64_t sum = 0;
    for (size_t i = 0; i < count; ++i) {
        const int32_t sample = samples[i];
        sum += static_cast<uint64_t>(sample * sample);
    }
    const float rms = sqrtf(static_cast<float>(sum) / count);
    return static_cast<uint16_t>(min(65535.0f, rms));
}

bool sendPacket(uint16_t flags, const int16_t *samples = nullptr,
                uint16_t sampleCount = 0, uint16_t level = 0) {
    if (WiFi.status() != WL_CONNECTED || receiverHost.isEmpty()) return false;
    pocket_wireless_mic::PacketHeader header = {
        pocket_wireless_mic::kMagic,
        pocket_wireless_mic::kVersion,
        flags,
        sequenceNumber++,
        pocket_wireless_mic::kSampleRate,
        sampleCount,
        level,
    };
    if (!udp.beginPacket(receiverHost.c_str(), receiverPort)) return false;
    udp.write(reinterpret_cast<const uint8_t *>(&header), sizeof(header));
    if (samples && sampleCount) {
        udp.write(reinterpret_cast<const uint8_t *>(samples),
                  sampleCount * sizeof(int16_t));
    }
    const bool sent = udp.endPacket() == 1;
    if (sent && sampleCount) ++packetsSent;
    return sent;
}

void beginStream() {
    if (streamState != StreamState::Idle) return;
    M5.Speaker.end();
    if (!M5.Mic.begin()) {
        drawStatus("MIC ERROR", TFT_RED, "Release and try again");
        return;
    }
    packetsSent = 0;
    sequenceNumber = 0;
    capturePending = false;
    streamState = StreamState::Streaming;
    sendPacket(pocket_wireless_mic::kStart);
    drawStatus("LISTENING", TFT_RED, "Release button to send");
}

void finishStream() {
    sendPacket(pocket_wireless_mic::kEnd);
    M5.Mic.end();
    capturePending = false;
    streamState = StreamState::Idle;
    drawStatus("SENT", TFT_GREEN, String(packetsSent) + " audio packets");
    delay(250);
    drawStatus("READY", TFT_GREEN, "Hold front button to talk");
}

void serviceAudio() {
    if (streamState == StreamState::Idle) return;

    if (capturePending && !M5.Mic.isRecording()) {
        const uint16_t level =
            calculateLevel(audioBuffer, pocket_wireless_mic::kSamplesPerPacket);
        sendPacket(pocket_wireless_mic::kAudio, audioBuffer,
                   pocket_wireless_mic::kSamplesPerPacket, level);
        capturePending = false;
    }

    if (streamState == StreamState::Stopping) {
        if (!capturePending) finishStream();
        return;
    }

    if (!capturePending) {
        capturePending = M5.Mic.record(
            audioBuffer, pocket_wireless_mic::kSamplesPerPacket,
            pocket_wireless_mic::kSampleRate, false);
        if (!capturePending) {
            streamState = StreamState::Stopping;
            drawStatus("MIC ERROR", TFT_RED, "Capture failed");
        }
    }
}

}  // namespace

void setup() {
    pocketsdk::Config config;
    config.rotation = 0;
    config.brightness = 96;
    config.internalMicrophone = true;
    config.internalSpeaker = false;
    Pocket.begin(config);
    M5.Mic.end();

    loadSettings();
    Pocket.update();
    delay(50);
    Pocket.update();
    if (Pocket.buttons().down(Button::M5) || wifiSsid.isEmpty() ||
        receiverHost.isEmpty()) {
        startSetupMode();
        return;
    }
    if (!connectWifi()) startSetupMode();
}

void loop() {
    Pocket.update();
    if (setupMode) {
        setupServer.handleClient();
        delay(2);
        return;
    }

    if (WiFi.status() != WL_CONNECTED && streamState == StreamState::Idle) {
        if (!connectWifi()) {
            startSetupMode();
            return;
        }
    }

    if (Pocket.buttons().pressed(Button::Face)) beginStream();
    if (Pocket.buttons().released(Button::Face) &&
        streamState == StreamState::Streaming) {
        streamState = StreamState::Stopping;
    }
    serviceAudio();
    delay(1);
}
