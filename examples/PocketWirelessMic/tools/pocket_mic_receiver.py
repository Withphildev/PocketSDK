#!/usr/bin/env python3
"""Receive PocketWirelessMic UDP audio and save each push-to-talk clip as WAV."""

from __future__ import annotations

import argparse
import datetime as dt
import pathlib
import socket
import struct
import wave

MAGIC = 0x314D4250
VERSION = 1
FLAG_AUDIO = 1 << 0
FLAG_START = 1 << 1
FLAG_END = 1 << 2
HEADER = struct.Struct("<IHHIIHH")


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--bind", default="0.0.0.0", help="local address to bind")
    parser.add_argument("--port", type=int, default=33333)
    parser.add_argument(
        "--output", type=pathlib.Path, default=pathlib.Path("recordings")
    )
    args = parser.parse_args()

    args.output.mkdir(parents=True, exist_ok=True)
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.bind((args.bind, args.port))
    print(f"Pocket mic receiver listening on UDP {args.bind}:{args.port}")

    wav_file: wave.Wave_write | None = None
    wav_path: pathlib.Path | None = None
    expected_sequence: int | None = None

    while True:
        packet, sender = sock.recvfrom(2048)
        if len(packet) < HEADER.size:
            continue
        fields = HEADER.unpack_from(packet)
        magic, version, flags, sequence, sample_rate, sample_count, level = fields
        if magic != MAGIC or version != VERSION:
            continue

        if flags & FLAG_START:
            if wav_file is not None:
                wav_file.close()
            stamp = dt.datetime.now().strftime("%Y%m%d-%H%M%S-%f")
            wav_path = args.output / f"pocket-mic-{stamp}.wav"
            wav_file = wave.open(str(wav_path), "wb")
            wav_file.setnchannels(1)
            wav_file.setsampwidth(2)
            wav_file.setframerate(sample_rate)
            expected_sequence = sequence + 1
            print(f"recording {wav_path} from {sender[0]}")

        if flags & FLAG_AUDIO and wav_file is not None:
            expected_size = HEADER.size + sample_count * 2
            if len(packet) != expected_size:
                print(f"discarded malformed packet {sequence}")
                continue
            if expected_sequence is not None and sequence != expected_sequence:
                print(f"packet gap: expected {expected_sequence}, received {sequence}")
            wav_file.writeframesraw(packet[HEADER.size :])
            expected_sequence = sequence + 1
            bars = min(30, level // 800)
            print(f"\rlevel [{'#' * bars:<30}]", end="", flush=True)

        if flags & FLAG_END and wav_file is not None:
            wav_file.close()
            wav_file = None
            expected_sequence = None
            print(f"\nsaved {wav_path}")


if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print("\nstopped")
