#include "input_controller.h"

#include <algorithm>
#include <cctype>
#include <cmath>

InputController::InputController(WorldState& state) : state_(state) {}

void InputController::handleMovementKey(unsigned char key, float deg_to_rad) {
    const unsigned char normalized = static_cast<unsigned char>(std::tolower(key));
    const float move_speed = 6.0f;

    switch (normalized) {
        case 'w':
            state_.camera_pov[0] += move_speed * std::cos(state_.cam_angle_x * deg_to_rad);
            state_.camera_pov[2] += move_speed * std::sin(state_.cam_angle_x * deg_to_rad);
            break;
        case 'a':
            state_.camera_pov[0] += move_speed * std::sin(state_.cam_angle_x * deg_to_rad);
            state_.camera_pov[2] -= move_speed * std::cos(state_.cam_angle_x * deg_to_rad);
            break;
        case 's':
            state_.camera_pov[0] -= move_speed * std::cos(state_.cam_angle_x * deg_to_rad);
            state_.camera_pov[2] -= move_speed * std::sin(state_.cam_angle_x * deg_to_rad);
            break;
        case 'd':
            state_.camera_pov[0] -= move_speed * std::sin(state_.cam_angle_x * deg_to_rad);
            state_.camera_pov[2] += move_speed * std::cos(state_.cam_angle_x * deg_to_rad);
            break;
    }
}

void InputController::togglePanMode() {
    if (state_.fps_mode) {
        state_.fps_mode = false;
        return;
    }

    for (int i = 0; i < 9; ++i) {
        state_.camera_pov[i] = state_.camera_pov_reset[i];
    }
    state_.fps_mode = true;
    state_.cursor_locked = true;
}

void InputController::releaseCursorLock() {
    state_.cursor_locked = false;
    state_.mouse_relative_initialized = false;
}

void InputController::lockCursor() {
    state_.cursor_locked = true;
    state_.mouse_relative_initialized = false;
}

void InputController::toggleCursorLock() {
    state_.cursor_locked = !state_.cursor_locked;
    state_.mouse_relative_initialized = false;
}

void InputController::mouseMove(
    int x,
    int y,
    float mouse_rotation,
    float deg_to_rad,
    bool cursor_locked,
    int center_x,
    int center_y) {
    (void)deg_to_rad;

    int delta_mouse_x = 0;
    int delta_mouse_y = 0;

    if (cursor_locked) {
        if (!state_.mouse_relative_initialized) {
            state_.mouse_relative_initialized = true;
            return;
        }

        // Locked mode uses true relative movement from screen center.
        delta_mouse_x = x - center_x;
        delta_mouse_y = y - center_y;

        // Ignore occasional large warping spikes from window re-entry/focus transitions.
        if (std::abs(delta_mouse_x) > 250 || std::abs(delta_mouse_y) > 250) {
            return;
        }
    } else {
        state_.mouse_relative_initialized = false;
        delta_mouse_x = x - state_.previous_x;
        delta_mouse_y = y - state_.previous_y;
        state_.previous_x = x;
        state_.previous_y = y;
    }

    if (delta_mouse_x == 0 && delta_mouse_y == 0) {
        return;
    }

    const float sensitivity_base = mouse_rotation / 40.0f;
    const float sensitivity_x = 0.20f * sensitivity_base;
    const float sensitivity_y = 0.16f * sensitivity_base;

    // Camera angles are stored in degrees in WorldState.
    state_.cam_angle_x += static_cast<float>(delta_mouse_x) * sensitivity_x;
    if (state_.cam_angle_x >= 360.0f) state_.cam_angle_x -= 360.0f;
    if (state_.cam_angle_x < 0.0f) state_.cam_angle_x += 360.0f;

    state_.cam_angle_y -= static_cast<float>(delta_mouse_y) * sensitivity_y;

    const float min_pitch_deg = -85.0f;
    const float max_pitch_deg = 85.0f;
    state_.cam_angle_y = std::max(min_pitch_deg, std::min(max_pitch_deg, state_.cam_angle_y));
}
