# Changelog

All notable changes to PocketSDK are documented here.

## Unreleased

### Added

- Compact future-session source of truth in `docs/BUILD_CONTEXT.md`
- PocketWirelessMic reference consumer with first-boot setup portal, 16 kHz
  push-to-talk UDP audio, documented packet format, and Python WAV receiver
- CI compilation for the PocketWirelessMic reference consumer
- PocketBuddy documented as the third independent PocketSDK consumer
- Dark Tower voice-client integration notes in `docs/POCKETBUDDY.md`
- PocketBuddy's authenticated local WebUI and setup-hotspot validation notes
- PocketBuddy v0.5 Wi-Fi QR setup and default-model picker notes
- PocketBuddy v0.6 staged-setup and enlarged-QR validation notes
- PocketBuddy v0.7 illustrated eye-pack integration notes
- PocketBuddy v0.7.1 static randomized-thinking gaze notes

### Architecture

- Kept wireless-mic Wi-Fi credentials, provisioning, UDP transport, and host
  receiver application-owned while reusing PocketSDK initialization, buttons,
  display, battery sampling, and namespaced settings
- Recorded PocketWirelessMic as the second application demonstrating the
  microphone/speaker exclusivity constraint; a core audio-session API remains
  deferred until both consumers migrate to and physically validate one API
- Kept Wi-Fi, HTTP, microphone/speaker lifecycle, buddy protocol, and face UI
  application-owned; one connected consumer is not enough evidence for a new
  core networking or audio abstraction
- Documented PocketBuddy's submit-and-poll pattern after live Dark Tower model
  latency exceeded the ESP32's safe single-request window
- Recorded the isolated Dark Tower deployment and the dedicated warm-model path;
  these remain application behavior rather than PocketSDK core abstractions
- Recorded the validated private Faster Whisper/Kokoro speech deployment and
  warm end-to-end StickS3 voice-loop timing
- Documented PocketBuddy's app-owned first-boot serial provisioning and merged
  `0x0000` factory image; no credential policy was added to PocketSDK core
- Recorded the first physical conversation findings and PocketBuddy's app-level
  wrapped-text and M5Unified WAV-playback corrections
- Documented bridge-side finalization of streaming WAV sizes required by the
  embedded playback path
- Recorded successful physical voice playback and the app-level speaker-volume
  increase for the StickS3 enclosure
- Validated maximum software volume on the physical StickS3 1 W speaker
- Documented PocketBuddy's app-owned WebUI, captive setup hotspot, mDNS, and
  write-only secret handling; one portal consumer is not enough evidence for a
  PocketSDK core provisioning abstraction
- Recorded physical WebUI loading, Save & Restart, and credential-retention
  validation on the StickS3
- Recorded the app-owned QR hotspot screen, `pocketbuddy` captive DNS name,
  and authenticated chat-model discovery; these remain outside PocketSDK core
  until another consumer proves the same requirements
- Recorded the physical v0.5 save-gate finding and v0.6 separation of Wi-Fi and
  AI-service readiness; staged provisioning remains application-owned
- Recorded PocketBuddy's nine-state transparent eye pack, randomized blink, and
  one randomly selected static gaze per thinking state as application-owned
  presentation behavior

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
