# PocketSDK

PocketSDK is a small, reusable foundation for the Withphildev Pocket family of
M5Stack applications.

The first release targets the **M5StickS3 only**. Its purpose is to centralize
the hardware setup and small pieces of device behavior that have already been
proven across PocketLab, PocketBoy, PocketPal, PocketQuest Mini, PocketDice,
and PocketBuddy.
It is not a game engine or an attempt to hide M5Unified.

## PocketSDK v0.1 scope

- Verified PlatformIO configuration for the M5StickS3
- Device initialization through M5Unified
- Logical Face and M5 button events, including press, release, and hold
- Display rotation and brightness defaults
- Battery level and charging snapshots
- M5StickS3 capability profile
- Namespaced Preferences storage helper
- Hardware diagnostic example
- PocketClock example
- PocketWirelessMic reference consumer and host receiver

Networking, WebSockets, WebUI hosting, game systems, animation, and asset
pipelines remain application-level concerns until a second proven use case
shows that they belong in the SDK.

For future Codex sessions, start with [`docs/BUILD_CONTEXT.md`](docs/BUILD_CONTEXT.md).
It records stable hardware facts, repository boundaries, commands, and
decisions that should not need to be rediscovered. The ready-to-build wireless
microphone reference is in
[`examples/PocketWirelessMic`](examples/PocketWirelessMic).

PocketBuddy is the first connected voice-client consumer. It intentionally
keeps Wi-Fi credentials, HTTP, recording policy, the Dark Tower protocol, and
its face UI in the application while reusing PocketSDK initialization, display,
battery, and logical button behavior. See [`docs/POCKETBUDDY.md`](docs/POCKETBUDDY.md).

## Quick start

```cpp
#include <PocketSDK.h>

void setup() {
    Pocket.begin();
    Pocket.display().fillScreen(TFT_BLACK);
}

void loop() {
    Pocket.update();

    if (Pocket.buttons().pressed(pocketsdk::Button::Face)) {
        Pocket.display().drawString("Hello Pocket!", 120, 67);
    }
}
```

See [`examples/HardwareDiagnostic`](examples/HardwareDiagnostic),
[`examples/PocketClock`](examples/PocketClock), and
[`examples/PocketWirelessMic`](examples/PocketWirelessMic) for complete
examples and reference consumers.

## PocketCheck

The hardware diagnostic example is published as **PocketCheck** for the
M5StickS3. PocketCheck v0.3.0 tests CPU information, RAM and PSRAM, flash and a
temporary NVS write/read cycle, Wi-Fi scanning, Bluetooth Low Energy scanning,
and the internal speaker. Wi-Fi does not connect or save credentials, BLE does
not pair, and the temporary storage value is removed after the test.

Use the Face button to move through detailed result pages. Use the M5 button to
retest the current page, or to rerun the automatic suite from the summary page.
The speaker test runs only when selected and requires human confirmation. See
[`docs/POCKETCHECK.md`](docs/POCKETCHECK.md) for test semantics.

## PlatformIO dependency

During development, use a local checkout:

```ini
lib_deps =
    m5stack/M5Unified@0.2.25
    symlink://../PocketSDK
```

Once releases are tagged, applications should pin a version:

```ini
lib_deps =
    m5stack/M5Unified@0.2.25
    https://github.com/Withphildev/PocketSDK.git#v0.1.0
```

## Design rules

1. Add an abstraction only after it has at least two real consumers.
2. Keep direct M5Unified/M5GFX access available for unusual hardware work.
3. Support one verified hardware profile before claiming portability.
4. Pin dependencies and releases so SDK updates do not silently break apps.
5. Keep optional features out of the core library.

## Status

The v0.1 StickS3 foundation and both bundled examples passed physical-hardware
validation on 2026-10-23. PocketCheck v0.3.0 also passed its CPU, RAM/PSRAM,
storage, Wi-Fi, BLE, and manual speaker diagnostics on physical M5StickS3
hardware.

PocketClock is the second independent consumer and PocketBuddy is the third.
They demonstrate that applications can reuse PocketSDK without requiring core
SDK source changes. See the
[hardware validation record](docs/HARDWARE_VALIDATION.md) for the tested
behaviors. No existing Pocket application has been migrated; the current
projects remain reference implementations and regression targets.

## License

MIT
