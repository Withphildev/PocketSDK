#pragma once

#include <Arduino.h>

namespace pocketsdk {

enum class Button : uint8_t { Face = 0, M5 = 1 };

class PocketButtons {
public:
    void begin();
    void update(uint32_t now = millis());

    bool pressed(Button button) const;
    bool released(Button button) const;
    bool held(Button button) const;
    bool down(Button button) const;

private:
    struct State {
        uint8_t pin = 0;
        bool raw = false;
        bool down = false;
        bool holdSent = false;
        bool suppressUntilRelease = false;
        bool pressedEvent = false;
        bool releasedEvent = false;
        bool heldEvent = false;
        uint32_t changedAt = 0;
        uint32_t pressedAt = 0;
    };

    static constexpr uint32_t kDebounceMs = 30;
    static constexpr uint32_t kHoldMs = 1000;
    State states_[2];

    static size_t index(Button button);
    void update(State &state, uint32_t now);
};

}  // namespace pocketsdk
