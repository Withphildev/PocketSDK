#include <NimBLEDevice.h>
#include <PocketSDK.h>
#include <WiFi.h>

namespace {
constexpr char kVersion[] = "0.3.0";
constexpr uint32_t kBleScanMs = 4000;

enum class TestStatus : uint8_t { Pending, Running, Pass, Fail };
enum class Page : uint8_t {
    Summary,
    Cpu,
    Ram,
    Storage,
    Wifi,
    Bluetooth,
    Speaker,
    Count
};

struct DiagnosticResults {
    TestStatus cpu = TestStatus::Pending;
    TestStatus ram = TestStatus::Pending;
    TestStatus storage = TestStatus::Pending;
    TestStatus wifi = TestStatus::Pending;
    TestStatus bluetooth = TestStatus::Pending;
    TestStatus speaker = TestStatus::Pending;

    String chipModel;
    uint8_t chipRevision = 0;
    uint8_t chipCores = 0;
    uint32_t cpuMHz = 0;

    uint32_t heapTotal = 0;
    uint32_t heapFree = 0;
    uint32_t psramTotal = 0;
    uint32_t psramFree = 0;

    uint32_t flashTotal = 0;
    uint32_t sketchSize = 0;
    bool nvsRoundTrip = false;

    int wifiNetworks = 0;
    int wifiBestRssi = -127;
    int wifiBestChannel = 0;

    int bleDevices = 0;
    int bleBestRssi = -127;
};

DiagnosticResults results;
Page page = Page::Summary;
bool speakerAwaitingConfirmation = false;

uint16_t statusColor(TestStatus status) {
    switch (status) {
        case TestStatus::Running: return TFT_YELLOW;
        case TestStatus::Pass: return TFT_GREEN;
        case TestStatus::Fail: return TFT_RED;
        default: return TFT_DARKGREY;
    }
}

const char *statusText(TestStatus status) {
    switch (status) {
        case TestStatus::Running: return "RUN";
        case TestStatus::Pass: return "PASS";
        case TestStatus::Fail: return "FAIL";
        default: return "--";
    }
}

String megabytes(uint32_t bytes) {
    return String(bytes / (1024U * 1024U)) + " MB";
}

void drawHeader(const char *title) {
    auto &display = Pocket.display();
    display.fillScreen(TFT_BLACK);
    display.setTextDatum(top_left);
    display.setTextSize(1);
    display.setTextColor(TFT_CYAN, TFT_BLACK);
    display.drawString("PocketCheck", 5, 4);
    display.setTextColor(TFT_DARKGREY, TFT_BLACK);
    display.drawString(String("v") + kVersion, 196, 4);
    display.drawFastHLine(5, 18, 230, TFT_DARKGREY);
    display.setTextColor(TFT_WHITE, TFT_BLACK);
    display.setTextSize(2);
    display.drawString(title, 5, 23);
}

void drawFooter() {
    auto &display = Pocket.display();
    display.drawFastHLine(5, 119, 230, TFT_DARKGREY);
    display.setTextSize(1);
    display.setTextColor(TFT_LIGHTGREY, TFT_BLACK);
    display.drawString("Face: next", 5, 123);
    display.setTextDatum(top_right);
    const char *action = page == Page::Summary
                             ? "M5: run all"
                             : (page == Page::Speaker ? "M5: play" : "M5: retest");
    display.drawString(action, 235, 123);
    display.setTextDatum(top_left);
}

void drawProgress(const char *label) {
    drawHeader("Diagnostics");
    auto &display = Pocket.display();
    display.setTextSize(1);
    display.setTextColor(TFT_YELLOW, TFT_BLACK);
    display.drawString(String("Running ") + label + "...", 12, 62);
    display.setTextColor(TFT_LIGHTGREY, TFT_BLACK);
    display.drawString("Please wait", 12, 80);
}

void drawSummaryRow(int y, const char *label, TestStatus status) {
    auto &display = Pocket.display();
    display.setTextDatum(top_left);
    display.setTextColor(TFT_WHITE, TFT_BLACK);
    display.drawString(label, 12, y);
    display.setTextDatum(top_right);
    display.setTextColor(statusColor(status), TFT_BLACK);
    display.drawString(statusText(status), 228, y);
    display.setTextDatum(top_left);
}

void drawSummary() {
    drawHeader("Diagnostics");
    Pocket.display().setTextSize(1);
    drawSummaryRow(42, "CPU", results.cpu);
    drawSummaryRow(54, "RAM / PSRAM", results.ram);
    drawSummaryRow(66, "Storage", results.storage);
    drawSummaryRow(78, "Wi-Fi scan", results.wifi);
    drawSummaryRow(90, "Bluetooth scan", results.bluetooth);
    drawSummaryRow(102, "Speaker (manual)", results.speaker);
    drawFooter();
}

void drawResultLine(int y, const char *label, const String &value) {
    auto &display = Pocket.display();
    display.setTextDatum(top_left);
    display.setTextColor(TFT_LIGHTGREY, TFT_BLACK);
    display.drawString(label, 8, y);
    display.setTextDatum(top_right);
    display.setTextColor(TFT_WHITE, TFT_BLACK);
    display.drawString(value, 232, y);
    display.setTextDatum(top_left);
}

void drawStatus(TestStatus status) {
    auto &display = Pocket.display();
    display.setTextDatum(top_right);
    display.setTextSize(1);
    display.setTextColor(statusColor(status), TFT_BLACK);
    display.drawString(statusText(status), 232, 27);
    display.setTextDatum(top_left);
}

void drawCpu() {
    drawHeader("CPU");
    drawStatus(results.cpu);
    drawResultLine(49, "Chip", results.chipModel);
    drawResultLine(64, "Revision", String(results.chipRevision));
    drawResultLine(79, "Cores", String(results.chipCores));
    drawResultLine(94, "Clock", String(results.cpuMHz) + " MHz");
    drawFooter();
}

void drawRam() {
    drawHeader("RAM / PSRAM");
    drawStatus(results.ram);
    drawResultLine(49, "Heap total", megabytes(results.heapTotal));
    drawResultLine(64, "Heap free", String(results.heapFree / 1024U) + " KB");
    drawResultLine(79, "PSRAM total", megabytes(results.psramTotal));
    drawResultLine(94, "PSRAM free", megabytes(results.psramFree));
    drawFooter();
}

void drawStorage() {
    drawHeader("Storage");
    drawStatus(results.storage);
    drawResultLine(49, "Flash", megabytes(results.flashTotal));
    drawResultLine(64, "Firmware", String(results.sketchSize / 1024U) + " KB");
    drawResultLine(79, "NVS write/read", results.nvsRoundTrip ? "PASS" : "FAIL");
    drawResultLine(94, "Test data", "removed");
    drawFooter();
}

void drawWifi() {
    drawHeader("Wi-Fi");
    drawStatus(results.wifi);
    drawResultLine(49, "Networks", String(results.wifiNetworks));
    drawResultLine(64, "Strongest", results.wifiNetworks > 0
                                       ? String(results.wifiBestRssi) + " dBm"
                                       : "none seen");
    drawResultLine(79, "Channel", results.wifiNetworks > 0
                                     ? String(results.wifiBestChannel)
                                     : "--");
    drawResultLine(94, "Privacy", "no connection");
    drawFooter();
}

void drawBluetooth() {
    drawHeader("Bluetooth");
    drawStatus(results.bluetooth);
    drawResultLine(49, "BLE devices", String(results.bleDevices));
    drawResultLine(64, "Strongest", results.bleDevices > 0
                                       ? String(results.bleBestRssi) + " dBm"
                                       : "none seen");
    drawResultLine(79, "Mode", "scan only");
    drawResultLine(94, "Privacy", "no pairing");
    drawFooter();
}

void drawSpeaker() {
    drawHeader("Speaker");
    drawStatus(results.speaker);
    drawResultLine(49, "Test", "two tones");
    drawResultLine(64, "Frequencies", "880 + 1320 Hz");
    drawResultLine(79, "Volume", "moderate");
    drawResultLine(94, "Start", "press M5");
    drawFooter();
}

void drawSpeakerPrompt() {
    drawHeader("Speaker");
    auto &display = Pocket.display();
    display.setTextSize(1);
    display.setTextColor(TFT_WHITE, TFT_BLACK);
    display.drawString("Did you hear two tones?", 12, 55);
    display.setTextColor(TFT_GREEN, TFT_BLACK);
    display.drawString("Face: PASS", 12, 78);
    display.setTextColor(TFT_RED, TFT_BLACK);
    display.drawString("M5: FAIL", 150, 78);
}

void drawPage() {
    switch (page) {
        case Page::Summary: drawSummary(); break;
        case Page::Cpu: drawCpu(); break;
        case Page::Ram: drawRam(); break;
        case Page::Storage: drawStorage(); break;
        case Page::Wifi: drawWifi(); break;
        case Page::Bluetooth: drawBluetooth(); break;
        case Page::Speaker: drawSpeaker(); break;
        default: page = Page::Summary; drawSummary(); break;
    }
}

void testCpu() {
    results.cpu = TestStatus::Running;
    drawProgress("CPU");
    results.chipModel = ESP.getChipModel();
    results.chipRevision = ESP.getChipRevision();
    results.chipCores = ESP.getChipCores();
    results.cpuMHz = ESP.getCpuFreqMHz();
    results.cpu = !results.chipModel.isEmpty() && results.chipCores > 0 &&
                          results.cpuMHz > 0
                      ? TestStatus::Pass
                      : TestStatus::Fail;
}

void testRam() {
    results.ram = TestStatus::Running;
    drawProgress("RAM / PSRAM");
    results.heapTotal = ESP.getHeapSize();
    results.heapFree = ESP.getFreeHeap();
    results.psramTotal = ESP.getPsramSize();
    results.psramFree = ESP.getFreePsram();
    results.ram = results.heapTotal > 0 && results.heapFree > 0 &&
                          results.psramTotal > 0 && results.psramFree > 0
                      ? TestStatus::Pass
                      : TestStatus::Fail;
}

void testStorage() {
    results.storage = TestStatus::Running;
    drawProgress("storage");
    results.flashTotal = ESP.getFlashChipSize();
    results.sketchSize = ESP.getSketchSize();

    pocketsdk::PocketSettings settings;
    const uint32_t token = 0x50434B54U ^ micros();
    bool opened = settings.begin("pcheckv2");
    bool written = opened && settings.putUInt("probe", token);
    bool matched = written && settings.getUInt("probe", 0) == token;
    bool removed = opened && settings.remove("probe");
    settings.end();

    results.nvsRoundTrip = written && matched && removed;
    results.storage = results.flashTotal > 0 && results.sketchSize > 0 &&
                              results.nvsRoundTrip
                          ? TestStatus::Pass
                          : TestStatus::Fail;
}

void testWifi() {
    results.wifi = TestStatus::Running;
    results.wifiNetworks = 0;
    results.wifiBestRssi = -127;
    results.wifiBestChannel = 0;
    drawProgress("Wi-Fi scan");

    WiFi.persistent(false);
    WiFi.mode(WIFI_STA);
    WiFi.disconnect(false, false);
    delay(50);
    const int count = WiFi.scanNetworks(false, true, false, 300);
    if (count >= 0) {
        results.wifiNetworks = count;
        for (int i = 0; i < count; ++i) {
            if (WiFi.RSSI(i) > results.wifiBestRssi) {
                results.wifiBestRssi = WiFi.RSSI(i);
                results.wifiBestChannel = WiFi.channel(i);
            }
        }
        results.wifi = TestStatus::Pass;
    } else {
        results.wifi = TestStatus::Fail;
    }
    WiFi.scanDelete();
    WiFi.mode(WIFI_OFF);
}

void testBluetooth() {
    results.bluetooth = TestStatus::Running;
    results.bleDevices = 0;
    results.bleBestRssi = -127;
    drawProgress("Bluetooth scan");

    if (!NimBLEDevice::init("PocketCheck")) {
        results.bluetooth = TestStatus::Fail;
        return;
    }

    NimBLEScan *scan = NimBLEDevice::getScan();
    scan->setActiveScan(true);
    scan->setInterval(100);
    scan->setWindow(60);
    scan->setMaxResults(64);
    NimBLEScanResults found = scan->getResults(kBleScanMs, false);
    results.bleDevices = found.getCount();
    for (int i = 0; i < results.bleDevices; ++i) {
        const NimBLEAdvertisedDevice *device = found.getDevice(i);
        if (device != nullptr && device->getRSSI() > results.bleBestRssi) {
            results.bleBestRssi = device->getRSSI();
        }
    }
    results.bluetooth = TestStatus::Pass;
    scan->stop();
    scan->clearResults();
    NimBLEDevice::deinit(true);
}

void testSpeaker() {
    results.speaker = TestStatus::Running;
    drawProgress("speaker");
    M5.Speaker.setVolume(160);
    const bool firstTone = M5.Speaker.tone(880.0f, 250);
    delay(325);
    const bool secondTone = M5.Speaker.tone(1320.0f, 250);
    delay(325);
    M5.Speaker.stop();

    if (!firstTone && !secondTone) {
        results.speaker = TestStatus::Fail;
        speakerAwaitingConfirmation = false;
        drawSpeaker();
        return;
    }

    speakerAwaitingConfirmation = true;
    drawSpeakerPrompt();
}

void runAll() {
    testCpu();
    testRam();
    testStorage();
    testWifi();
    testBluetooth();
}

void runCurrentPage() {
    switch (page) {
        case Page::Summary: runAll(); break;
        case Page::Cpu: testCpu(); break;
        case Page::Ram: testRam(); break;
        case Page::Storage: testStorage(); break;
        case Page::Wifi: testWifi(); break;
        case Page::Bluetooth: testBluetooth(); break;
        case Page::Speaker: testSpeaker(); return;
        default: break;
    }
    drawPage();
}
}  // namespace

void setup() {
    pocketsdk::Config config;
    config.internalMicrophone = false;
    config.internalSpeaker = true;
    Pocket.begin(config);
    runAll();
    drawSummary();
}

void loop() {
    Pocket.update();

    if (speakerAwaitingConfirmation) {
        if (Pocket.buttons().pressed(pocketsdk::Button::Face)) {
            results.speaker = TestStatus::Pass;
            speakerAwaitingConfirmation = false;
            drawSpeaker();
        } else if (Pocket.buttons().pressed(pocketsdk::Button::M5)) {
            results.speaker = TestStatus::Fail;
            speakerAwaitingConfirmation = false;
            drawSpeaker();
        }
        delay(2);
        return;
    }

    if (Pocket.buttons().pressed(pocketsdk::Button::Face)) {
        page = static_cast<Page>((static_cast<uint8_t>(page) + 1U) %
                                 static_cast<uint8_t>(Page::Count));
        drawPage();
    }

    if (Pocket.buttons().pressed(pocketsdk::Button::M5)) runCurrentPage();

    delay(2);
}
