#include "PocketSDK.h"

namespace pocketsdk {

bool PocketSDK::begin(const Config &config) {
    if (config.initializeSerial) Serial.begin(config.serialBaud);

    auto m5Config = M5.config();
    m5Config.serial_baudrate = config.serialBaud;
    m5Config.internal_mic = config.internalMicrophone;
    m5Config.internal_spk = config.internalSpeaker;
    m5Config.output_power = config.externalOutputPower;
    M5.begin(m5Config);

    M5.Display.setRotation(config.rotation);
    M5.Display.setBrightness(config.brightness);
    M5.Power.setExtOutput(config.externalOutputPower);
    buttons_.begin();
    sampleBattery(millis(), true);

    return M5.getDisplayCount() > 0;
}

void PocketSDK::update(uint32_t now) {
    M5.update();
    buttons_.update(now);
    sampleBattery(now);
}

void PocketSDK::sampleBattery(uint32_t now, bool force) {
    if (!force && now - battery_.sampledAt < kBatterySampleMs) return;
    battery_.level = M5.Power.getBatteryLevel();
    battery_.charging = M5.Power.isCharging() == m5::Power_Class::is_charging;
    battery_.sampledAt = now;
}

}  // namespace pocketsdk

pocketsdk::PocketSDK Pocket;
