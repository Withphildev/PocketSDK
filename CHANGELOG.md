# Changelog

All notable changes to PocketSDK are documented here.

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
