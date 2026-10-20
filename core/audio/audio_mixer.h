#pragma once
#include <string>
#include <map>
#include <algorithm>

namespace NexusOS {
namespace Audio {

struct AudioStream {
    int stream_id;
    std::string app_name;
    int volume; // 0-100
    bool muted;
};

class AudioMixer {
private:
    std::map<int, AudioStream> streams;
    int master_volume = 50;
    bool master_muted = false;
    int next_stream_id = 1;

public:
    int GetMasterVolume() const { return master_volume; }
    
    void SetMasterVolume(int vol) { 
        master_volume = std::clamp(vol, 0, 100); 
    }
    
    bool IsMasterMuted() const { return master_muted; }
    void ToggleMasterMute() { master_muted = !master_muted; }

    int RegisterStream(const std::string& app_name) {
        int id = next_stream_id++;
        streams[id] = {id, app_name, 100, false};
        return id;
    }

    bool SetStreamVolume(int stream_id, int vol) {
        if (streams.find(stream_id) != streams.end()) {
            streams[stream_id].volume = std::clamp(vol, 0, 100);
            return true;
        }
        return false;
    }
    
    int GetEffectiveStreamVolume(int stream_id) {
        if (master_muted || streams.find(stream_id) == streams.end() || streams[stream_id].muted) {
            return 0;
        }
        // Calculate the hardware output level based on app volume + master volume limit
        return (streams[stream_id].volume * master_volume) / 100;
    }
};

} // namespace Audio
} // namespace NexusOS
