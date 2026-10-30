# PocketSDK agent guide

## Mission

PocketSDK is a thin M5StickS3 foundation shared by future Pocket applications.
Preserve direct access to M5Unified and M5GFX. Do not turn this repository into
a game engine, WebUI framework, or collection of application features.

Before changing the repository, read `docs/BUILD_CONTEXT.md` and the integration
document for the application being touched. Those files are the compact source
of truth for decisions already made and are intended to prevent repeated chat
discovery work.

## Supported target

- Verified target: M5Stack M5StickS3
- Framework: Arduino through PlatformIO
- C++ standard: GNU++17
- Platform: pioarduino ESP32 55.03.39
- M5Unified: 0.2.25

Do not claim support for another device until it has its own profile and has
been tested on physical hardware.

## Architecture rules

- Require two proven application consumers before adding a new abstraction.
- Keep optional modules out of the core.
- Never silently enable Wi-Fi, Bluetooth, the microphone, or the speaker.
- Applications own their feature policy, layouts, networking, and data models.
- Preserve the logical button names `Face` and `M5`; device profiles map them
  to physical controls.
- Keep initialization configurable because applications have different audio,
  power, and peripheral needs.
- Prefer additive, backward-compatible API changes before v1.0.

## Verification

Run:

```sh
pio run -d examples/HardwareDiagnostic
pio run -d examples/PocketClock
pio run -d examples/PocketWirelessMic
```

Hardware-dependent behavior must also be checked on an M5StickS3. A successful
compile does not prove button mapping, display orientation, battery reporting,
audio behavior, or IMU behavior.
