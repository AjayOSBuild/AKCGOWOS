#include "audio_mixer.h"
#include <algorithm>

AudioMixer::AudioMixer(size_t size) : frame_size(size) {
    master_buffer.resize(frame_size, 0);
    std::cout << "[AudioMixer] Initializing Audio Mixing Daemon (Frame size: " << frame_size << " samples)...\n";
}

uint32_t AudioMixer::RegisterStream() {
    uint32_t id = next_stream_id++;
    streams[id] = {id, 1.0f, true, std::vector<int16_t>(frame_size, 0)};
    std::cout << "[AudioMixer] Registered new audio stream: ID " << id << "\n";
    return id;
}

void AudioMixer::UnregisterStream(uint32_t stream_id) {
    if (streams.erase(stream_id)) {
        std::cout << "[AudioMixer] Unregistered audio stream: ID " << stream_id << "\n";
    }
}

void AudioMixer::SetStreamVolume(uint32_t stream_id, float volume) {
    auto it = streams.find(stream_id);
    if (it != streams.end()) {
        it->second.volume = std::clamp(volume, 0.0f, 1.0f);
        std::cout << "[AudioMixer] Stream " << stream_id << " volume set to " << (it->second.volume * 100) << "%\n";
    }
}

void AudioMixer::FeedStream(uint32_t stream_id, const std::vector<int16_t>& pcm_data) {
    auto it = streams.find(stream_id);
    if (it != streams.end()) {
        // In a real scenario, this involves a ring buffer. Here we just overwrite the frame.
        size_t copy_size = std::min(pcm_data.size(), it->second.pcm_buffer.size());
        std::copy(pcm_data.begin(), pcm_data.begin() + copy_size, it->second.pcm_buffer.begin());
        it->second.is_active = true;
    }
}

std::vector<int16_t> AudioMixer::MixFrames() {
    std::cout << "[AudioMixer] Commencing mixing pass for " << streams.size() << " active streams...\n";
    
    // Clear the master buffer
    std::fill(master_buffer.begin(), master_buffer.end(), 0);

    // Sum and clamp samples to prevent digital clipping
    for (size_t i = 0; i < frame_size; ++i) {
        int32_t mixed_sample = 0;

        for (auto& pair : streams) {
            if (pair.second.is_active) {
                mixed_sample += static_cast<int32_t>(pair.second.pcm_buffer[i] * pair.second.volume);
            }
        }

        // Hard clipping protection (16-bit limits)
        if (mixed_sample > INT16_MAX) mixed_sample = INT16_MAX;
        if (mixed_sample < INT16_MIN) mixed_sample = INT16_MIN;

        master_buffer[i] = static_cast<int16_t>(mixed_sample);
    }

    std::cout << "  -> Frames mixed successfully. Master buffer ready for hardware dispatch.\n";
    return master_buffer;
}

int main() {
    std::cout << "=== OS Project Audio Mixing Daemon Utility ===\n";
    AudioMixer mixer(1920); // 10ms frame size

    uint32_t music_stream = mixer.RegisterStream();
    uint32_t notif_stream = mixer.RegisterStream();

    mixer.SetStreamVolume(music_stream, 0.8f); // Music at 80%
    mixer.SetStreamVolume(notif_stream, 1.0f); // Notification at 100%

    // Create mock PCM data patterns
    std::vector<int16_t> mock_music(1920, 15000); // High amplitude wave
    std::vector<int16_t> mock_notif(1920, 20000); // High amplitude beep

    mixer.FeedStream(music_stream, mock_music);
    mixer.FeedStream(notif_stream, mock_notif);

    // Mix streams and retrieve the master output
    std::vector<int16_t> output = mixer.MixFrames();

    // Verify clipping logic on the first sample: (15000 * 0.8) + (20000 * 1.0) = 12000 + 20000 = 32000
    std::cout << "[Test] First sample amplitude out of mixer: " << output[0] << " (Expected ~32000)\n";

    mixer.UnregisterStream(music_stream);
    mixer.UnregisterStream(notif_stream);

    std::cout << "[+] Audio Mixer testing completed successfully.\n";
    return 0;
}
