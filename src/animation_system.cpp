#include "animation_system.h"

AnimationSystem::AnimationSystem(WorldState& state) : state_(state) {}

void AnimationSystem::panStep() {
    if (state_.pan_angle >= 360.0f) {
        state_.pan_angle = 0.0f;
    }
    if (!state_.fps_mode) {
        state_.pan_angle += 0.5f;
    }
}

void AnimationSystem::linearTrainStep(float track_length) {
    float half_track_length = track_length / 2.0f - 40.0f;

    if (state_.train1_decreasing) {
        if (state_.train1_location_x <= -half_track_length) {
            state_.train1_decreasing = false;
        } else {
            state_.train1_location_x -= state_.train1_speed;
            state_.train0_location_x += state_.train0_speed;
        }
        return;
    }

    if (state_.train1_location_x > half_track_length) {
        state_.train1_decreasing = true;
    } else {
        state_.train1_location_x += state_.train1_speed;
        state_.train0_location_x -= state_.train0_speed;
    }
}

void AnimationSystem::barrierStep() {
    if (state_.barrier_theta >= 90.0f) {
        state_.barrier_lowering = true;
    } else if (state_.barrier_theta <= 0.0f) {
        state_.barrier_lowering = false;
    }

    if (state_.barrier_lowering) {
        state_.barrier_theta -= 0.5f;
    } else {
        state_.barrier_theta += 0.5f;
    }

    state_.stop_light = state_.barrier_theta <= 45.0f;
}

void AnimationSystem::refreshStopLight() {
    state_.stop_light = state_.barrier_theta <= 45.0f;
}
