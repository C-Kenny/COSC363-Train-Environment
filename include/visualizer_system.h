#ifndef VISUALIZER_SYSTEM_H
#define VISUALIZER_SYSTEM_H

#include <string>

class VisualizerSystem {
public:
    explicit VisualizerSystem(const std::string& input_path = "data/visualizer_input.txt");

    void update(float dt_seconds);

    float low() const;
    float mid() const;
    float high() const;
    float pulse() const;
    bool beat() const;
    bool usingExternalInput() const;

private:
    bool readBandsFromFile(float& low, float& mid, float& high, float& beat_hint);
    void proceduralFallback(float dt_seconds);
    static float clamp01(float v);

    std::string input_path_;

    float low_;
    float mid_;
    float high_;
    float pulse_;
    bool beat_;
    bool using_external_input_;
    float phase_;
    float beat_cooldown_;
};

#endif
