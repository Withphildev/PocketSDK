# PocketCheck

PocketCheck is the ready-to-flash M5StickS3 diagnostic powered by PocketSDK.

## Controls

- **Face:** move to the next result page
- **M5 on a detail page:** rerun that diagnostic
- **M5 on the summary page:** rerun the complete suite

## v0.2.0 diagnostics

| Diagnostic | What a pass means |
|---|---|
| CPU | The chip reports a model, at least one core, and a nonzero clock speed |
| RAM / PSRAM | Internal heap and external PSRAM are present and report free memory |
| Storage | Flash information is available and a temporary NVS value can be written, read back, and removed |
| Wi-Fi | A blocking station-mode scan completes without an error |
| Bluetooth | The NimBLE radio stack initializes and a BLE scan cycle completes |

A radio scan can pass with zero nearby devices. The pass indicates that the
scan completed; the device count indicates whether any advertisements were
heard in the current environment.

## Privacy behavior

PocketCheck does not connect to a Wi-Fi network, save credentials, display
network names, pair with Bluetooth devices, or retain scan results. Wi-Fi and
BLE are disabled after their tests. The NVS storage probe is removed after it
is read back.
