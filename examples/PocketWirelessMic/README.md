# PocketWirelessMic

PocketWirelessMic is a ready-to-build PocketSDK reference consumer. It turns an
M5StickS3 into a Wi-Fi push-to-talk microphone without putting networking or
credential policy into the SDK core.

## Behavior

- Hold the front/Face button to capture 16 kHz, signed 16-bit mono PCM.
- Release it to finish the utterance.
- Audio is sent as small versioned UDP packets to a configured receiver.
- The included Python receiver writes one standard WAV file per utterance.
- Hold the side/M5 button during boot to reopen setup mode.

UDP favors low latency over guaranteed delivery. The receiver reports sequence
gaps so packet loss is visible. This transport is appropriate for a trusted LAN;
it is neither encrypted nor authenticated and must not be exposed to the public
internet.

## Build

From the PocketSDK repository root:

```sh
pio run -d examples/PocketWirelessMic
```

Normal PlatformIO upload:

```sh
pio run -d examples/PocketWirelessMic -t upload
```

## First boot

1. Start the device. It creates the Wi-Fi network `Pocket-Mic-Setup`.
2. Join that network and open <http://192.168.4.1>.
3. Enter the normal Wi-Fi credentials and receiver LAN address.
4. Keep UDP port `33333` unless the receiver uses another value.
5. Save. Credentials remain in the device's `pwmic` NVS namespace and are not
   compiled into firmware.

## Receiver

The receiver uses only the Python standard library:

```sh
python3 examples/PocketWirelessMic/tools/pocket_mic_receiver.py
```

Allow incoming network access if the operating system asks. Recordings are
written beneath `recordings/` by default. Use `--output`, `--bind`, or `--port`
to override those values.

## Packet format, version 1

Every datagram starts with this packed, little-endian, 20-byte header:

| Offset | Type | Field |
|---:|---|---|
| 0 | `uint32` | magic `0x314D4250` (`PBM1` in memory) |
| 4 | `uint16` | protocol version |
| 6 | `uint16` | flags: audio `1`, start `2`, end `4` |
| 8 | `uint32` | sequence number |
| 12 | `uint32` | sample rate |
| 16 | `uint16` | sample count |
| 18 | `uint16` | RMS level |

Audio packets append `sample count × 2` bytes of signed little-endian PCM.
Start and end packets have no payload.

## Flashing at `0x0000`

The pinned pioarduino build creates the merged image at
`.pio/build/m5stack-sticks3/firmware.factory.bin` inside this example. That is
the one-file image to flash manually at `0x0000`. The nearby `firmware.bin` is
application-only and normally belongs at `0x10000`; do not place it at `0x0000`.

## Validation status

The original standalone wireless-mic prototype compiled and its host receiver
passed a local packet test. This PocketSDK-integrated reference must be compiled
in CI and physically checked on an M5StickS3 before being described as hardware
validated.
