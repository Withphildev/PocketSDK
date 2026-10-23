#include "PocketSettings.h"

namespace pocketsdk {

PocketSettings::~PocketSettings() { end(); }

bool PocketSettings::begin(const char *nameSpace, bool readOnly) {
    end();
    if (nameSpace == nullptr || nameSpace[0] == '\0') return false;
    ready_ = preferences_.begin(nameSpace, readOnly);
    readOnly_ = ready_ && readOnly;
    return ready_;
}

void PocketSettings::end() {
    if (ready_) preferences_.end();
    ready_ = false;
    readOnly_ = false;
}

bool PocketSettings::getBool(const char *key, bool fallback) {
    return ready_ ? preferences_.getBool(key, fallback) : fallback;
}

uint8_t PocketSettings::getUChar(const char *key, uint8_t fallback) {
    return ready_ ? preferences_.getUChar(key, fallback) : fallback;
}

uint32_t PocketSettings::getUInt(const char *key, uint32_t fallback) {
    return ready_ ? preferences_.getUInt(key, fallback) : fallback;
}

String PocketSettings::getString(const char *key, const String &fallback) {
    return ready_ ? preferences_.getString(key, fallback) : fallback;
}

bool PocketSettings::putBool(const char *key, bool value) {
    return ready_ && !readOnly_ && preferences_.putBool(key, value) > 0;
}

bool PocketSettings::putUChar(const char *key, uint8_t value) {
    return ready_ && !readOnly_ && preferences_.putUChar(key, value) > 0;
}

bool PocketSettings::putUInt(const char *key, uint32_t value) {
    return ready_ && !readOnly_ && preferences_.putUInt(key, value) > 0;
}

bool PocketSettings::putString(const char *key, const String &value) {
    return ready_ && !readOnly_ && preferences_.putString(key, value) > 0;
}

bool PocketSettings::remove(const char *key) {
    return ready_ && !readOnly_ && preferences_.remove(key);
}

}  // namespace pocketsdk
