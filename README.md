# PocketSDK

PocketSDK is a small, reusable foundation for the Withphildev Pocket family of
M5Stack applications.

The first release targets the **M5StickS3 only**. Its purpose is to centralize
the hardware setup and small pieces of device behavior that have already been
proven across PocketLab, PocketBoy, PocketPal, PocketQuest Mini, and PocketDice.
It is not a game engine or an attempt to hide M5Unified.

## v0.1 scope

- Verified PlatformIO configuration for the M5StickS3
- Device initialization through M5Unified
- Logical Face and M5 button events, including press, release, and hold
- Display rotation and brightness defaults
- Battery level and charging snapshots
- M5StickS3 capability profile
- Namespaced Preferences storage helper
- Hardware diagnostic example
- PocketClock example

Networking, WebSockets, WebUI hosting, game systems, animation, and asset
pipelines remain application-level concerns until a second proven use case
shows that they belong in the SDK.

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

See [`examples/HardwareDiagnostic`](examples/HardwareDiagnostic) and
[`examples/PocketClock`](examples/PocketClock) for complete examples.

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
validation on 2026-10-23. Display initialization, orientation, both buttons,
press and hold events, brightness control, and battery reporting were verified
on an M5StickS3.

PocketClock is the second independent consumer. It demonstrates that an
application can reuse PocketSDK without requiring SDK source changes. See the
[hardware validation record](docs/HARDWARE_VALIDATION.md) for the tested
behaviors. No existing Pocket application has been migrated; the current
projects remain reference implementations and regression targets.

## License

MIT
