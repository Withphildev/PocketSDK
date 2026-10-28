# PocketBuddy integration

PocketBuddy turns the M5StickS3 into a small face, microphone, speaker, and
button client for Phil's Dark Tower AI server. It is PocketSDK's third
independent application consumer and its first networked voice client.

## PocketSDK responsibilities

- M5StickS3 and M5Unified initialization
- Display access
- Logical Face and M5 button events
- Battery snapshots
- Device capability declaration

## Application responsibilities

- Wi-Fi credentials and connection policy
- Dark Tower URL, authentication token, and HTTP requests
- Microphone/speaker switching and recording duration
- WAV transport and playback
- Expressions, layout, personality, and conversation protocol
- Local WebUI, setup hotspot, captive DNS, authentication, and provisioning

This boundary follows the repository rule requiring two proven application
consumers before adding an abstraction. PocketBuddy provides the first evidence
for connected audio behavior, so no networking or audio-session class has been
added to PocketSDK core yet.

## Audio constraint discovered

The StickS3's current M5Unified audio path treats microphone capture and speaker
playback as mutually exclusive. PocketBuddy explicitly ends the speaker before
recording, ends the microphone before playback, and restores the microphone
after playback. A future shared helper should be considered only after a second
application proves the same lifecycle and physical-device testing confirms it.

## Validation status

The bridge has host-side automated tests. Firmware compilation and physical
record/playback validation remain required after the PocketBuddy source is
placed in its release repository and a StickS3 is connected.

The first PocketBuddy compile audit found that the README's future `v0.1.0`
dependency example is not yet resolvable because GitHub currently has `main`
but no `v0.1.0` tag. PocketBuddy pins verified commit
`a9dc0dcfa556ed965c98bd11daa22c3b094fd452` until the release tag exists.

The release firmware uses application-owned USB serial provisioning for Wi-Fi
and the Dark Tower token, storing them in NVS after flashing. This keeps the
single `0x0000` factory image free of credentials and does not add provisioning
policy to PocketSDK core.

PocketBuddy now also embeds an authenticated responsive WebUI. It exposes
Wi-Fi, OpenAI-compatible bridge, model, voice, volume, identity, and personality
settings, with an automatic setup hotspot when station Wi-Fi is unavailable.
Secret fields are write-only from the browser: the firmware returns only
boolean saved-state flags, and blank submissions retain existing credentials.
The portal, captive DNS, mDNS, and settings schema remain application-owned.
PocketBuddy is still the only proven portal consumer, so this does not yet meet
PocketSDK's two-consumer threshold for a core provisioning abstraction.

The physical StickS3 passed live page loading, authenticated configuration
reads, Save & Restart, and post-restart credential-retention checks. Firmware
v0.4 also displays the setup hotspot credentials on first boot and the numeric
LAN WebUI address during normal operation.

Firmware v0.5 adds an application-owned Wi-Fi QR setup screen and captive DNS
for `http://pocketbuddy` while the phone or computer is joined directly to the
StickS3 hotspot. The identical WebUI remains available after station-mode
startup. The bridge also exposes an authenticated OpenAI-compatible model list;
the device proxies that list to a default-model picker and excludes obvious
embedding-only models. These QR, captive-portal, and model-discovery behaviors
remain outside PocketSDK core pending evidence from a second consumer.

Physical v0.5 onboarding confirmed that the Wi-Fi QR and captive WebUI worked,
but revealed that the application rejected a Wi-Fi-only first save while its AI
token was still blank. Firmware v0.6 separates Wi-Fi readiness from AI-service
readiness, permits staged setup, connects to the LAN after the first save, and
uses larger QR modules. This remains PocketBuddy application policy rather than
a PocketSDK storage or provisioning change.

Firmware v0.7 adds an application-owned illustrated eye system derived from the
PocketBuddy expression reference. Nine transparent 240×80 PNG layers cover
calm, listening, thinking directions, happy frames, concern, offline, and blink.
The application schedules randomized calm/happy blinks. On entry to the
thinking state it randomly selects the left or right gaze and holds that image,
avoiding continuous display redraws. This does not change PocketSDK display
ownership or add an animation abstraction to the SDK.

Initial physical conversation testing found two application-level issues: long
replies were drawn as one clipped line, and the hand-written PCM extraction path
did not produce audible playback even though Dark Tower returned valid WAV.
PocketBuddy now word-wraps replies over three lines and passes the complete WAV
buffer to M5Unified's `playWav()` implementation with automatic channel
selection. Neither fix changes PocketSDK core.

USB playback diagnostics then showed that Kokoro's streaming WAV used
`0xFFFFFFFF` placeholders for the RIFF and data lengths. Desktop players accept
that convention, but the embedded playback path rejected it. The application
bridge now replaces both placeholders with the completed buffer lengths before
caching the reply. This remains an application transport concern rather than a
PocketSDK abstraction.

Physical playback then succeeded. The first audible voice was too quiet for the
small enclosure, so PocketBuddy raised its application-owned speaker volume
from 200 to M5Unified's maximum software level of 255. Physical listening will
determine whether a lower value is preferable to avoid distortion. Physical
listening subsequently confirmed clear, acceptable voice output at 255 on the
StickS3's 1 W speaker.

## Slow-server behavior discovered

A live `gemma4:26b` bridge request to Dark Tower exceeded 180 seconds before a
first response. PocketBuddy therefore submits work quickly and polls a job URL
with short requests instead of holding one ESP32 HTTP connection open. This is
application protocol behavior, not a PocketSDK core concern, but it is an
important design constraint for future connected Pocket applications.

## Dark Tower deployment result

PocketBuddy's isolated bridge is deployed at `192.168.0.172:5810` with Docker's
`unless-stopped` restart policy. A dedicated `gemma4:e4b` buddy route measured
about 265 seconds on its first cold load but roughly 0.17 seconds when warm. The
bridge requests a 30-minute keep-alive and disables internal thinking for its
short spoken replies; neither setting changes Nova/OpenClaw's model route.

The final authenticated submit/poll validation completed in 446 ms and returned
a normal text reply.

Speech now runs behind the bridge on its private Compose network. A GPU-backed
Faster Whisper service transcribes 16 kHz mono WAV, while a CPU Kokoro service
synthesizes 24 kHz mono PCM WAV. Neither speech service exposes a LAN port. The
English transcription model is warmed by its container health check after
startup and kept resident; its measured footprint is about 512 MiB of GPU
memory. A warm full voice loop completed in 1.891 seconds. Physical StickS3
validation remains the final gate.
