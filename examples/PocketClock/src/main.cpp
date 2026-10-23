#include <PocketSDK.h>

namespace {
constexpr uint8_t kNormalBrightness = 90;
constexpr uint8_t kDimBrightness = 20;

bool dimmed = false;
bool showSeconds = true;
uint32_t lastRenderedSecond = UINT32_MAX;

void drawClock(uint32_t elapsedSeconds) {
    const uint32_t hours = (elapsedSeconds / 3600U) % 100U;
    const uint32_t minutes = (elapsedSeconds / 60U) % 60U;
    const uint32_t seconds = elapsedSeconds % 60U;
    const auto &battery = Pocket.battery();
    auto &display = Pocket.display();

    char timeText[16];
    if (showSeconds) {
        snprintf(timeText, sizeof(timeText), "%02lu:%02lu:%02lu",
                 static_cast<unsigned long>(hours),
                 static_cast<unsigned long>(minutes),
                 static_cast<unsigned long>(seconds));
    } else {
        snprintf(timeText, sizeof(timeText), "%02lu:%02lu",
                 static_cast<unsigned long>(hours),
                 static_cast<unsigned long>(minutes));
    }

    display.fillScreen(TFT_BLACK);
    display.setTextDatum(top_center);
    display.setTextColor(TFT_CYAN, TFT_BLACK);
    display.setTextSize(2);
    display.drawString("PocketClock", display.width() / 2, 8);

    display.setTextDatum(middle_center);
    display.setTextColor(TFT_WHITE, TFT_BLACK);
    display.setTextSize(showSeconds ? 3 : 4);
    display.drawString(timeText, display.width() / 2, 64);

    display.setTextDatum(bottom_left);
    display.setTextSize(1);
    display.setTextColor(TFT_LIGHTGREY, TFT_BLACK);
    display.drawString("Face: brightness", 5, display.height() - 5);

    display.setTextDatum(bottom_right);
    char batteryText[20];
    snprintf(batteryText, sizeof(batteryText), "%d%%%s", battery.level,
             battery.charging ? "+" : "");
    display.drawString(batteryText, display.width() - 5, display.height() - 5);
}
}  // namespace

void setup() {
    pocketsdk::Config config;
    config.internalMicrophone = false;
    config.internalSpeaker = false;
    config.brightness = kNormalBrightness;
    Pocket.begin(config);
    drawClock(0);
}

void loop() {
    Pocket.update();

    if (Pocket.buttons().pressed(pocketsdk::Button::Face)) {
        dimmed = !dimmed;
        Pocket.display().setBrightness(dimmed ? kDimBrightness : kNormalBrightness);
    }

    if (Pocket.buttons().pressed(pocketsdk::Button::M5)) {
        showSeconds = !showSeconds;
        lastRenderedSecond = UINT32_MAX;
    }

    const uint32_t elapsedSeconds = millis() / 1000U;
    if (elapsedSeconds != lastRenderedSecond) {
        lastRenderedSecond = elapsedSeconds;
        drawClock(elapsedSeconds);
    }

    delay(2);
}
