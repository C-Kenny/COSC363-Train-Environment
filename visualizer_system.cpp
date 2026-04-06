#include "visualizer_system.h"

#include <cmath>
#include <fstream>

namespace {
const float kBandSmoothing = 0.18f;
const float kPulseDecayPerSecond = 2.0f;
const float kBeatCooldownSeconds = 0.18f;
}

VisualizerSystem::VisualizerSystem(const std::string& input_path)
    : input_path_(input_path),
      low_(0.0f),
      mid_(0.0f),
      high_(0.0f),
      pulse_(0.0f),
      beat_(false),
      using_external_input_(false),
      phase_(0.0f),
      beat_cooldown_(0.0f) {}

void VisualizerSystem::update(float dt_seconds) {
    if (dt_seconds < 0.0f) {
        dt_seconds = 0.0f;
    }

    float in_low = 0.0f;
    float in_mid = 0.0f;
    float in_high = 0.0f;
    float in_beat_hint = 0.0f;

    bool has_external = readBandsFromFile(in_low, in_mid, in_high, in_beat_hint);
    using_external_input_ = has_external;

    if (!has_external) {
        proceduralFallback(dt_seconds);
        in_low = low_;
        in_mid = mid_;
        in_high = high_;
    }

    low_ += (in_low - low_) * kBandSmoothing;
    mid_ += (in_mid - mid_) * kBandSmoothing;
    high_ += (in_high - high_) * kBandSmoothing;

    if (beat_cooldown_ > 0.0f) {
        beat_cooldown_ -= dt_seconds;
    }

    bool low_peak_beat = (low_ > 0.72f && beat_cooldown_ <= 0.0f);
    bool hinted_beat = (in_beat_hint > 0.5f);
    beat_ = hinted_beat || low_peak_beat;

    if (beat_) {
        pulse_ = 1.0f;
        beat_cooldown_ = kBeatCooldownSeconds;
    } else {
        pulse_ -= dt_seconds * kPulseDecayPerSecond;
        if (pulse_ < 0.0f) {
            pulse_ = 0.0f;
        }
    }
}

float VisualizerSystem::low() const { return low_; }
float VisualizerSystem::mid() const { return mid_; }
float VisualizerSystem::high() const { return high_; }
float VisualizerSystem::pulse() const { return pulse_; }
bool VisualizerSystem::beat() const { return beat_; }
bool VisualizerSystem::usingExternalInput() const { return using_external_input_; }

bool VisualizerSystem::readBandsFromFile(float& low, float& mid, float& high, float& beat_hint) {
    std::ifstream input(input_path_.c_str());
    if (!input.good()) {
        return false;
    }

    if (!(input >> low >> mid >> high)) {
        return false;
    }

    if (!(input >> beat_hint)) {
        beat_hint = 0.0f;
    }

    low = clamp01(low);
    mid = clamp01(mid);
    high = clamp01(high);
    beat_hint = clamp01(beat_hint);
    return true;
}

void VisualizerSystem::proceduralFallback(float dt_seconds) {
    phase_ += dt_seconds;
    low_ = 0.5f + 0.45f * std::sin(phase_ * 2.1f);
    mid_ = 0.5f + 0.45f * std::sin(phase_ * 3.4f + 1.2f);
    high_ = 0.5f + 0.45f * std::sin(phase_ * 5.6f + 2.4f);

    low_ = clamp01(low_);
    mid_ = clamp01(mid_);
    high_ = clamp01(high_);
}

float VisualizerSystem::clamp01(float v) {
    if (v < 0.0f) {
        return 0.0f;
    }
    if (v > 1.0f) {
        return 1.0f;
    }
    return v;
}
