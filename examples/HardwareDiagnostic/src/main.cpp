#include <PocketSDK.h>

namespace {
uint32_t lastDraw = 0;

void draw() {
    auto &display = Pocket.display();
    const auto &battery = Pocket.battery();
    const auto &caps = Pocket.capabilities();

    display.fillScreen(TFT_BLACK);
    display.setTextColor(TFT_CYAN, TFT_BLACK);
    display.setTextSize(2);
    display.setCursor(6, 6);
    display.print("PocketSDK");

    display.setTextColor(TFT_WHITE, TFT_BLACK);
    display.setTextSize(1);
    display.setCursor(6, 32);
    display.printf("Display: %dx%d", display.width(), display.height());
    display.setCursor(6, 46);
    display.printf("Battery: %d%% %s", battery.level,
                   battery.charging ? "charging" : "battery");
    display.setCursor(6, 60);
    display.printf("PSRAM: %s", caps.psram ? "profile yes" : "profile no");
    display.setCursor(6, 82);
    display.print("Face: mark   M5: mark");
    display.setCursor(6, 100);
    display.print("Hold either for HOLD");
}

void showEvent(const char *label, uint16_t color) {
    auto &display = Pocket.display();
    display.fillRect(6, 116, 228, 16, TFT_BLACK);
    display.setTextColor(color, TFT_BLACK);
    display.setCursor(6, 118);
    display.print(label);
}
}  // namespace

void setup() {
    pocketsdk::Config config;
    config.internalMicrophone = false;
    config.internalSpeaker = false;
    Pocket.begin(config);
    draw();
}

void loop() {
    Pocket.update();

    if (Pocket.buttons().pressed(pocketsdk::Button::Face)) showEvent("FACE PRESSED", TFT_GREEN);
    if (Pocket.buttons().pressed(pocketsdk::Button::M5)) showEvent("M5 PRESSED", TFT_YELLOW);
    if (Pocket.buttons().held(pocketsdk::Button::Face)) showEvent("FACE HOLD", TFT_MAGENTA);
    if (Pocket.buttons().held(pocketsdk::Button::M5)) showEvent("M5 HOLD", TFT_ORANGE);

    if (millis() - lastDraw >= 5000) {
        lastDraw = millis();
        draw();
    }
    delay(2);
}
