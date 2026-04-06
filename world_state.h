#ifndef WORLD_STATE_H
#define WORLD_STATE_H

struct WorldState {
    float camera_pov[9] = {0.0f, 28.0f, 200.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f};
    float camera_pov_reset[9] = {0.0f, 28.0f, 200.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f};
    float cam_angle_x = 90.0f;
    float cam_angle_y = 30.0f;
    float dist_cam = 150.0f;
    float pan_radius = 800.0f;
    float pan_height = 70.0f;
    float pan_angle = 0.0f;
    bool fps_mode = true;
    bool cursor_locked = true;

    bool stop_light = true;

    float train0_location_x = 0.0f;
    float train0_location_z = 0.0f;
    float train0_speed = 3.0f;
    bool train0_decreasing = false;

    float train1_location_x = 0.0f;
    float train1_location_z = 0.0f;
    float train1_speed = 3.0f;
    bool train1_decreasing = false;

    int previous_x = 0;
    int previous_y = 0;
    bool mouse_relative_initialized = false;

    float barrier_theta = 1.0f;
    bool barrier_lowering = false;
};

#endif
