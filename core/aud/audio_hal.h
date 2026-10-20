#pragma once
#include <iostream>
#include <string>
#include <vector>

struct AudioFormat {
    uint32_t sample_rate;
    uint8_t channels;
    uint8_t bit_depth;
};

class AudioHAL {
private:
    std::string device_name;
    bool is_open;
    AudioFormat current_format;

public:
    AudioHAL();

    // Queries the PCI/USB buses for connected sound cards
    void EnumerateDevices() const;

    // Claims the hardware device and negotiates playback parameters
    bool OpenDevice(const std::string& name, const AudioFormat& format);

    // Writes raw interleaved PCM frames to the hardware ring buffer
    bool WritePCM(const std::vector<uint8_t>& buffer) const;

    // Releases hardware locks and powers down the DAC
    void CloseDevice();
};
