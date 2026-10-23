#include "PocketButtons.h"

namespace pocketsdk {
namespace {
constexpr uint8_t kFacePin = 11;
constexpr uint8_t kM5Pin = 12;
}

size_t PocketButtons::index(Button button) {
    return button == Button::Face ? 0U : 1U;
}

void PocketButtons::begin() {
    states_[0].pin = kFacePin;
    states_[1].pin = kM5Pin;

    for (auto &state : states_) {
        pinMode(state.pin, INPUT_PULLUP);
        state.raw = digitalRead(state.pin) == LOW;
        state.down = state.raw;
        state.suppressUntilRelease = state.down;
        state.changedAt = millis();
        state.pressedAt = state.changedAt;
    }
}

void PocketButtons::update(uint32_t now) {
    for (auto &state : states_) update(state, now);
}

void PocketButtons::update(State &state, uint32_t now) {
    state.pressedEvent = false;
    state.releasedEvent = false;
    state.heldEvent = false;

    const bool raw = digitalRead(state.pin) == LOW;
    if (raw != state.raw) {
        state.raw = raw;
        state.changedAt = now;
    }

    if (now - state.changedAt >= kDebounceMs && state.down != state.raw) {
        state.down = state.raw;
        if (state.down) {
            state.pressedAt = now;
            state.holdSent = false;
            if (!state.suppressUntilRelease) state.pressedEvent = true;
        } else {
            if (!state.suppressUntilRelease) state.releasedEvent = true;
            state.suppressUntilRelease = false;
        }
    }

    if (state.down && !state.holdSent && !state.suppressUntilRelease &&
        now - state.pressedAt >= kHoldMs) {
        state.holdSent = true;
        state.heldEvent = true;
    }
}

bool PocketButtons::pressed(Button button) const {
    return states_[index(button)].pressedEvent;
}

bool PocketButtons::released(Button button) const {
    return states_[index(button)].releasedEvent;
}

bool PocketButtons::held(Button button) const {
    return states_[index(button)].heldEvent;
}

bool PocketButtons::down(Button button) const {
    return states_[index(button)].down;
}

}  // namespace pocketsdk
