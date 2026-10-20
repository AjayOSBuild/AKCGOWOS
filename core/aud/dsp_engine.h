#pragma once
#include <iostream>
#include <vector>
#include <cstdint>
#include <string>

enum class EqProfile {
    FLAT,
    V_SHAPE,
    BASS_SHELF
};

class DspEngine {
private:
    EqProfile current_profile;

    // Simulates a biquad filter pass (e.g., Peaking EQ, Low Shelf, High Shelf)
    void ApplyBiquadFilter(std::vector<int16_t>& buffer, float frequency, float gain_db, float q_factor) const;

public:
    DspEngine();

    // Sets the active global equalizer profile
    void SetProfile(EqProfile profile);

    // Processes a master PCM frame through the DSP pipeline before hardware dispatch
    void ProcessMasterFrame(std::vector<int16_t>& buffer) const;
    
    // Returns a string representation of the current profile
    std::string GetProfileName() const;
};
