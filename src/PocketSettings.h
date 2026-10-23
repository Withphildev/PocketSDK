#pragma once

#include <Arduino.h>
#include <Preferences.h>

namespace pocketsdk {

class PocketSettings {
public:
    PocketSettings() = default;
    ~PocketSettings();

    PocketSettings(const PocketSettings &) = delete;
    PocketSettings &operator=(const PocketSettings &) = delete;

    bool begin(const char *nameSpace, bool readOnly = false);
    void end();
    bool ready() const { return ready_; }

    bool getBool(const char *key, bool fallback = false);
    uint8_t getUChar(const char *key, uint8_t fallback = 0);
    uint32_t getUInt(const char *key, uint32_t fallback = 0);
    String getString(const char *key, const String &fallback = String());

    bool putBool(const char *key, bool value);
    bool putUChar(const char *key, uint8_t value);
    bool putUInt(const char *key, uint32_t value);
    bool putString(const char *key, const String &value);
    bool remove(const char *key);

private:
    Preferences preferences_;
    bool ready_ = false;
    bool readOnly_ = false;
};

}  // namespace pocketsdk
