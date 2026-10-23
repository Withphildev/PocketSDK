# M5Burner listing copy

PocketSDK is a C++ library, so the M5Burner listing is for its ready-to-flash
hardware diagnostic rather than the library itself.

## Listing fields

- **Name:** PocketCheck
- **Version:** 0.3.0
- **Device type:** M5StickS3 / StickS3
- **GitHub:** https://github.com/Withphildev/PocketSDK
- **Firmware:** `PocketCheck-v0.3.0-factory.bin`
- **Cover:** `docs/assets/pocketsdk-hardware-diagnostic-cover.png`
- **Flash address:** `0x0000`

## Description

PocketCheck is a ready-to-flash hardware diagnostic for the M5Stack M5StickS3,
powered by PocketSDK. Version 0.3.0 tests CPU information, RAM and PSRAM, flash
storage and a temporary NVS write/read cycle, Wi-Fi scanning, Bluetooth Low
Energy scanning, and the internal speaker.

Use the Face button to move through detailed result pages. Use the M5 button to
retest the current page, or rerun the automatic suite from the summary page.
The speaker remains silent until its page is selected and M5 is pressed. Wi-Fi
never connects or saves credentials, BLE never pairs, and temporary test data
is removed immediately.

PocketSDK is the open-source M5Unified foundation used by the Withphildev
Pocket family. Source, examples, documentation, and release files are available
on GitHub.

## Release evidence

PocketCheck v0.3.0 was flashed and its complete suite, including the manual
speaker test, was verified on physical M5StickS3 hardware on 2026-10-23. Full
results are in
`docs/HARDWARE_VALIDATION.md`.
