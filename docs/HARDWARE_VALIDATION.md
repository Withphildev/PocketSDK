# M5StickS3 hardware validation

## 2026-10-23 — v0.1 foundation

The `HardwareDiagnostic` factory image was compiled from commit
`378e0b75e65e600a8085a49d5281d826a0498f5a`, flashed at address `0x0000`, and
run on a physical M5Stack M5StickS3.

| Check | Result |
|---|---|
| Factory image flashes and boots | Pass |
| Display initializes in landscape orientation | Pass |
| Face button press event | Pass |
| M5 button press event | Pass |
| Face button hold event | Pass |
| M5 button hold event | Pass |
| Battery level is displayed | Pass |
| Charging/battery state is displayed | Pass |

The owner reported all checks passing. This validates the first PocketSDK
hardware profile and its initial input, display, and battery behavior.

Compilation was also verified locally and by GitHub Actions. Physical
validation remains required after changes to the device profile, button
handling, initialization sequence, or battery API.

## 2026-10-23 — PocketClock independent consumer

The `PocketClock` factory image was compiled from commit
`e5278e51dea49ce110391e5d6025c48a655bde12`, flashed at address `0x0000`, and
run on the same physical M5Stack M5StickS3.

| Check | Result |
|---|---|
| Factory image flashes and boots | Pass |
| PocketClock display renders correctly | Pass |
| Face button toggles display brightness | Pass |
| M5 button toggles seconds display | Pass |
| Battery level is displayed | Pass |

The owner reported that everything worked. PocketClock required no PocketSDK
source changes, validating the v0.1 API as a reusable application foundation.

## 2026-10-23 — PocketCheck v0.2.0

The `PocketCheck-v0.2.0-factory.bin` image was flashed at address `0x0000` and
run on a physical M5Stack M5StickS3.

| Check | Result |
|---|---|
| Factory image flashes and boots | Pass |
| CPU model, revision, core count, and clock test | Pass |
| Heap and PSRAM test | Pass |
| Flash information and temporary NVS write/read/remove test | Pass |
| Wi-Fi radio scan without connection or saved credentials | Pass |
| Bluetooth Low Energy scan without pairing | Pass |
| Face button cycles through detailed result pages | Pass |
| M5 button reruns the selected test and full suite | Pass |

The owner reported that all tests worked. This validates PocketCheck v0.2.0 on
physical M5StickS3 hardware. It does not add automatic radio behavior to the
PocketSDK core; Wi-Fi and BLE remain application-controlled features.

## 2026-10-23 — PocketCheck v0.3.0 speaker test

The `PocketCheck-v0.3.0-factory.bin` image was flashed at address `0x0000` and
run on a physical M5Stack M5StickS3.

| Check | Result |
|---|---|
| Existing v0.2 automatic diagnostic suite | Pass |
| Speaker test remains silent until explicitly selected | Pass |
| 880 Hz test tone is audible | Pass |
| 1320 Hz test tone is audible | Pass |
| Face button records manual PASS confirmation | Pass |
| Speaker result appears in the summary | Pass |

The owner reported that the speaker test passed. Because audio output cannot be
verified by the microcontroller itself, PocketCheck records a human-confirmed
result after playing the two tones.
