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
