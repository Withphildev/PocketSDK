# Pocket family build context

Read this first when starting a new Codex session. It is deliberately compact:
the goal is to reuse decisions and commands instead of reconstructing the
project from chat history.

## Current hardware boundary

- PocketSDK's verified target is the M5Stack M5StickS3.
- The Lafvin ESP32-S3 DeskBuddy is a separate, standalone device and repository.
- Dark Tower is the server/host. A keyboard and mouse may connect to Dark Tower;
  they are not PocketSDK peripherals.
- PocketWirelessMic turns a StickS3 into an optional Wi-Fi push-to-talk mic.
- Do not claim support for another board without a device profile, compile test,
  and physical validation.

## Put code in the right place

PocketSDK owns only behavior already useful to multiple Pocket applications:

- M5Unified device initialization
- logical `Face` and `M5` buttons
- display access and defaults
- battery/charging snapshots
- device capabilities
- namespaced Preferences storage

Applications own feature policy and integrations:

- credentials, addresses, ports, tokens, and provisioning UX
- Wi-Fi, Bluetooth, HTTP, UDP, WebSockets, and server protocols
- recording policy and speaker/microphone switching
- screens, personalities, animation, and application data
- desktop/server companion programs

An application may live under `examples/` when it is a useful reference
consumer, but its application-specific behavior must not leak into core.

## Audio facts already discovered

- StickS3 microphone capture and speaker playback are mutually exclusive on the
  current M5Unified path.
- End the speaker before beginning microphone capture.
- End the microphone before playback, then restore the application's desired
  idle mode afterward.
- PocketBuddy and PocketWirelessMic both demonstrate this lifecycle. Do not add
  a core audio-session abstraction until both consumers can use the same API and
  it has been tested on physical hardware.

## Wireless microphone baseline

`examples/PocketWirelessMic` is the canonical starting point. It provides:

- 16,000 Hz, signed 16-bit, mono PCM
- 256 samples per UDP audio packet
- push-to-talk using the logical Face button
- first-boot access point and local setup page
- credentials stored in the app's `pwmic` NVS namespace
- a versioned 20-byte packet header
- a standard-library-only Python receiver that writes one WAV per utterance

No credentials or fixed LAN address belong in source control. The setup page
stores them on the device. Hold the logical M5 button during boot to reopen it.

## Build and validation

```sh
pio run -d examples/HardwareDiagnostic
pio run -d examples/PocketClock
pio run -d examples/PocketWirelessMic
python3 -m py_compile examples/PocketWirelessMic/tools/pocket_mic_receiver.py
```

Compilation is not physical validation. For audio changes also verify on a real
StickS3: setup-mode entry, Wi-Fi reconnect, button press/release, captured
audio, packet-loss reporting, and saved WAV playback.

## Flashing rule

PlatformIO's normal upload handles partition offsets. A merged factory image is
the artifact flashed manually at `0x0000`. The pinned pioarduino build creates
it as `examples/PocketWirelessMic/.pio/build/m5stack-sticks3/firmware.factory.bin`.
Never flash the standalone `firmware.bin` at `0x0000`; that image normally
belongs at `0x10000`.

## Session handoff checklist

When finishing meaningful work, update the relevant integration document and
`CHANGELOG.md`. Record exact compile results separately from physical results,
and never turn an untested assumption into a claimed hardware capability.
