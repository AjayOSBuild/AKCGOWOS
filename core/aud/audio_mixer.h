#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include <cstdint>

struct AudioStream {
    uint32_t stream_id;
    float volume; // 0.0 to 1.0
    bool is_active;
    std::vector<int16_t> pcm_buffer; // 16-bit PCM data
};

class AudioMixer {
private:
    uint32_t next_stream_id = 1;
    size_t frame_size; // Number of samples per mix pass
    std::unordered_map<uint32_t, AudioStream> streams;
    std::vector<int16_t> master_buffer;

public:
    // Defaults to 1920 samples (10ms of 48kHz Stereo)
    AudioMixer(size_t frame_size = 1920);

    // Registers a new client audio stream and returns its ID
    uint32_t RegisterStream();

    // Removes an audio stream from the mixing pool
    void UnregisterStream(uint32_t stream_id);

    // Adjusts the volume multiplier for a specific stream
    void SetStreamVolume(uint32_t stream_id, float volume);

    // Feeds raw PCM data from an application into its stream buffer
    void FeedStream(uint32_t stream_id, const std::vector<int16_t>& pcm_data);

    // Mixes all active stream buffers into the master buffer and returns it
    std::vector<int16_t> MixFrames();
};
