#pragma once

#include <Arduino.h>

namespace pocket_wireless_mic {

constexpr uint32_t kMagic = 0x314D4250;  // "PBM1" in little-endian memory.
constexpr uint16_t kVersion = 1;
constexpr uint32_t kSampleRate = 16000;
constexpr size_t kSamplesPerPacket = 256;
constexpr uint16_t kDefaultPort = 33333;

enum PacketFlags : uint16_t {
    kAudio = 1u << 0,
    kStart = 1u << 1,
    kEnd = 1u << 2,
};

struct __attribute__((packed)) PacketHeader {
    uint32_t magic;
    uint16_t version;
    uint16_t flags;
    uint32_t sequence;
    uint32_t sampleRate;
    uint16_t sampleCount;
    uint16_t level;
};

static_assert(sizeof(PacketHeader) == 20, "Unexpected wireless mic header size");

}  // namespace pocket_wireless_mic
