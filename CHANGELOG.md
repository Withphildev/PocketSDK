# Changelog

All notable changes to PocketSDK are documented here.

## PocketCheck 0.3.0 — 2026-10-23

### Added

- Manual internal-speaker diagnostic using 880 Hz and 1320 Hz tones
- On-device PASS or FAIL confirmation after tone playback
- Speaker status on the diagnostic summary

### Behavior

- The speaker test never runs automatically at startup
- Audio plays only after the user opens the Speaker page and presses M5
- A human confirmation is required because firmware cannot determine whether
  the sound was physically audible

### Validated

- Both tones, confirmation controls, and summary status passed on a physical
  M5StickS3

## PocketCheck 0.2.0 — 2026-10-23

### Added

- CPU model, revision, core-count, and clock reporting
- Heap and PSRAM capacity reporting
- Flash information and a temporary NVS write/read/remove test
- Wi-Fi radio scan with network count, strongest signal, and channel
- Bluetooth Low Energy scan with device count and strongest signal
- Summary and detail pages with per-test reruns

### Privacy and safety

- Wi-Fi scans without connecting or saving credentials
- BLE scans without pairing
- The storage probe is removed immediately after the round-trip test
- Radios are disabled after their scans complete

### Validated

- The complete suite and navigation controls passed on a physical M5StickS3

## [0.1.0] — 2026-10-23

### Added

- M5StickS3 initialization through M5Unified
- Logical Face and M5 button events for press, release, and hold
- Suppression of false button events when a button is held during boot
- Battery and charging snapshots
- M5StickS3 capability profile
- Namespaced Preferences storage helper
- HardwareDiagnostic and PocketClock examples
- Local and GitHub Actions compile verification

### Validated

- HardwareDiagnostic passed all display, input, hold, and battery checks on a
  physical M5StickS3
- PocketClock passed its display, brightness, input, and battery checks on a
  physical M5StickS3 without SDK source changes

[0.1.0]: https://github.com/Withphildev/PocketSDK/releases/tag/v0.1.0
