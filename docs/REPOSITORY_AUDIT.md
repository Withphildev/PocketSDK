# Pocket repository audit

Reviewed on 2026-10-23:

- `Withphildev/PocketLab-StickS3`
- `Withphildev/PocketBoy-S3`
- `Withphildev/PocketPal-S3`
- `Withphildev/PocketQuestMini-S3`
- `Withphildev/PocketDice-S3`

## Reuse matrix

| Concern | Lab | Boy | Pal | Quest Mini | Dice | SDK decision |
|---|---:|---:|---:|---:|---:|---|
| M5Unified initialization | Yes | Yes | Yes | Yes | Yes | Core |
| Rotation 3 landscape display | Yes | Yes | Yes | Yes | Yes | Core default |
| Brightness configuration | Yes | Yes | Yes | Yes | Yes | Core config |
| Disable external output | Yes | Yes | Yes | Yes | Yes | Core config |
| Face/M5 buttons | Yes | Yes | Yes | Yes | Yes | Core logical input |
| Battery reporting | Yes | Yes | App-specific | Yes | No | Small snapshot API |
| Preferences/NVS | No | No | Yes | No | No | Optional helper only |
| LittleFS | Yes | Yes | App-specific | Yes | Yes | Not core yet |
| Wi-Fi access point/WebUI | Yes | Yes | Yes | Yes | Yes | Candidate module, deferred |
| IMU | No | No | Yes | Yes | Planned | Deferred |
| Microphone/audio | No | Emulator audio | Yes | Disabled | No | Deferred |
| Game state/rendering | No | Emulator | Pet | RPG | Dice | Application-specific |
| Multiplayer/WebSockets | No | No | No | No | Yes | Application-specific for now |

## Configuration findings

All five projects use the same pioarduino ESP32 platform release,
`esp32-s3-devkitc-1`, 8 MB flash, OPI PSRAM, USB CDC, and the
`ARDUINO_M5STICK_S3` define. Four use QIO flash mode; PocketDice currently uses
DIO. PocketQuest Mini additionally pins M5Unified 0.2.25 and M5GFX 0.2.32.

PocketSDK adopts the common QIO/OPI configuration and pins M5Unified 0.2.25.
Partition tables and filesystems stay with applications because their storage
budgets differ substantially.

## Button findings

PocketDice uses M5Unified `BtnA` and `BtnB`. PocketLab, PocketBoy, PocketPal,
and PocketQuest Mini directly read GPIO 11 and GPIO 12. PocketQuest Mini has
the strongest reusable implementation: debounced edge events, a hold event,
and boot-held suppression. PocketSDK uses that behavior behind logical `Face`
and `M5` names.

## Deliberately deferred

- Wi-Fi/AP/WebUI: common need, but each portal currently carries substantial
  application policy and asset handling.
- LittleFS: partition layouts and safety requirements differ.
- Battery estimation: PocketLab's filtered estimator is sophisticated and
  useful, but not yet proven as the correct policy for every application.
- IMU, microphone, speaker: device access is proven, but application behavior
  and resource conflicts differ.
- Game, animation, sprites, saves, multiplayer: domain-specific today.

## Extraction rule

Existing projects remain unchanged. A component moves into PocketSDK only when
two applications can consume it without SDK source edits or application policy
leaking into the library.
