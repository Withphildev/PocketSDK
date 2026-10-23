#pragma once

#include <M5Unified.h>

#include "PocketButtons.h"
#include "PocketDevice.h"
#include "PocketSettings.h"

namespace pocketsdk {

struct Config {
    uint32_t serialBaud = 115200;
    uint8_t rotation = 3;
    uint8_t brightness = 90;
    bool initializeSerial = true;
    bool internalMicrophone = true;
    bool internalSpeaker = true;
    bool externalOutputPower = false;
};

class PocketSDK {
public:
    bool begin(const Config &config = Config());
    void update(uint32_t now = millis());

    PocketButtons &buttons() { return buttons_; }
    const PocketButtons &buttons() const { return buttons_; }
    M5GFX &display() { return M5.Display; }
    const DeviceCapabilities &capabilities() const { return capabilities_; }
    const BatterySnapshot &battery() const { return battery_; }

private:
    static constexpr uint32_t kBatterySampleMs = 5000;
    PocketButtons buttons_;
    DeviceCapabilities capabilities_ = stickS3Capabilities();
    BatterySnapshot battery_;

    void sampleBattery(uint32_t now, bool force = false);
};

}  // namespace pocketsdk

extern pocketsdk::PocketSDK Pocket;
