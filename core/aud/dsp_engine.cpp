#include "dsp_engine.h"

DspEngine::DspEngine() : current_profile(EqProfile::FLAT) {
    std::cout << "[DSP] Initializing Digital Signal Processing Engine...\n";
}

void DspEngine::SetProfile(EqProfile profile) {
    current_profile = profile;
    std::cout << "[DSP] Active Equalizer Profile set to: " << GetProfileName() << "\n";
}

std::string DspEngine::GetProfileName() const {
    switch (current_profile) {
        case EqProfile::FLAT: return "Flat (Reference)";
        case EqProfile::V_SHAPE: return "V-Shape (Dynamic)";
        case EqProfile::BASS_SHELF: return "Bass Shelf (Low-End Focus)";
        default: return "Unknown";
    }
}

void DspEngine::ApplyBiquadFilter(std::vector<int16_t>& buffer, float frequency, float gain_db, float q_factor) const {
    // In a production environment, this applies recursive IIR filter coefficients 
    // to the PCM samples. For our simulation, we log the DSP parameter adjustments.
    std::cout << "  -> [Filter] Freq: " << frequency << "Hz | Gain: " 
              << (gain_db > 0 ? "+" : "") << gain_db << "dB | Q: " << q_factor << "\n";
}

void DspEngine::ProcessMasterFrame(std::vector<int16_t>& buffer) const {
    if (current_profile == EqProfile::FLAT) {
        std::cout << "[DSP] Bypassing EQ (Flat Profile active)...\n";
        return;
    }

    std::cout << "[DSP] Processing " << buffer.size() << " samples with " << GetProfileName() << "...\n";

    if (current_profile == EqProfile::V_SHAPE) {
        // Boost lows, cut lower-mids to remove mud, boost highs for clarity
        ApplyBiquadFilter(buffer, 80.0f, +4.0f, 0.7f);    // Low shelf boost
        ApplyBiquadFilter(buffer, 250.0f, -3.0f, 1.2f);   // 250 Hz attenuation for clarity
        ApplyBiquadFilter(buffer, 8000.0f, +5.0f, 0.7f);  // High shelf boost
    } 
    else if (current_profile == EqProfile::BASS_SHELF) {
        // Emphasize sub and mid-bass while keeping mids clean
        ApplyBiquadFilter(buffer, 100.0f, +6.0f, 0.7f);   // Strong bass shelf
        ApplyBiquadFilter(buffer, 250.0f, -2.5f, 1.0f);   // 250 Hz cut to prevent bass bleed
    }

    std::cout << "  -> DSP pipeline execution complete.\n";
}

int main() {
    std::cout << "=== OS Project DSP & Equalizer Utility ===\n";
    DspEngine dsp;
    
    // Simulating a 10ms frame of stereo PCM audio
    std::vector<int16_t> master_buffer(1920, 1000); 

    std::cout << "\n--- Testing V-Shape Profile ---\n";
    dsp.SetProfile(EqProfile::V_SHAPE);
    dsp.ProcessMasterFrame(master_buffer);

    std::cout << "\n--- Testing Bass Shelf Profile ---\n";
    dsp.SetProfile(EqProfile::BASS_SHELF);
    dsp.ProcessMasterFrame(master_buffer);

    std::cout << "\n[+] DSP Engine testing completed successfully.\n";
    return 0;
}
