#include "../test_framework.h"
#include "../../core/audio/audio_mixer.h"

using namespace NexusOS::Audio;

TEST(AS_MasterVolumeControl) {
    AudioMixer mixer;
    mixer.SetMasterVolume(75);
    ASSERT_EQ(75, mixer.GetMasterVolume());
    
    mixer.SetMasterVolume(150); // Should clamp to 100
    ASSERT_EQ(100, mixer.GetMasterVolume());
    
    mixer.SetMasterVolume(-20); // Should clamp to 0
    ASSERT_EQ(0, mixer.GetMasterVolume());
    
    mixer.ToggleMasterMute();
    ASSERT_TRUE(mixer.IsMasterMuted());
}

TEST(AS_StreamMixing) {
    AudioMixer mixer;
    mixer.SetMasterVolume(50);
    
    int sid = mixer.RegisterStream("Media Player");
    ASSERT_TRUE(mixer.SetStreamVolume(sid, 80));
    
    // Effective hardware volume should be 80 * 50 / 100 = 40
    ASSERT_EQ(40, mixer.GetEffectiveStreamVolume(sid));
    
    mixer.ToggleMasterMute(); // Global mute
    ASSERT_EQ(0, mixer.GetEffectiveStreamVolume(sid));
}

int main() {
    return TestRegistry::RunAll();
}
