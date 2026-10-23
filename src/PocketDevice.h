#pragma once

#include <Arduino.h>

namespace pocketsdk {

struct DeviceCapabilities {
    bool display;
    bool buttons;
    bool battery;
    bool wifi;
    bool bluetooth;
    bool imu;
    bool microphone;
    bool speaker;
    bool psram;
};

struct BatterySnapshot {
    int level = -1;
    bool charging = false;
    uint32_t sampledAt = 0;
};

constexpr DeviceCapabilities stickS3Capabilities() {
    return {true, true, true, true, true, true, true, true, true};
}

}  // namespace pocketsdk
