# M5Burner listing copy

PocketSDK is a C++ library, so the M5Burner listing is for its ready-to-flash
hardware diagnostic rather than the library itself.

## Listing fields

- **Name:** PocketSDK Hardware Diagnostic
- **Version:** 0.1.0
- **Device type:** M5StickS3 / StickS3
- **GitHub:** https://github.com/Withphildev/PocketSDK
- **Firmware:** `PocketSDK-HardwareDiagnostic-v0.1.0-factory.bin`
- **Flash address:** `0x0000`

## Description

PocketSDK Hardware Diagnostic is a ready-to-flash test for the M5Stack
M5StickS3. It verifies the display, Face and M5 buttons, press and hold events,
PSRAM capability profile, battery level, and charging state.

Press either button to see its event. Hold either button to verify hold
detection. Battery information refreshes automatically.

PocketSDK is the open-source M5Unified foundation used by the Withphildev
Pocket family. Source, examples, documentation, and release files are available
on GitHub.

## Release evidence

The v0.1.0 HardwareDiagnostic and PocketClock images were both flashed and
verified on physical M5StickS3 hardware on 2026-10-23. Full results are in
`docs/HARDWARE_VALIDATION.md`.
