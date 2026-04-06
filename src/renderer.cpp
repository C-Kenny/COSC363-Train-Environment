#include "renderer.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <cmath>

void Renderer::applyCamera(const WorldState& state, float deg_to_rad) {
    if (state.fps_mode) {
        if (state.cursor_locked) {
            glutSetCursor(GLUT_CURSOR_NONE);
        } else {
            glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
        }
        gluLookAt(
            state.camera_pov[0], state.camera_pov[1], state.camera_pov[2],
            state.camera_pov[0] + state.dist_cam * std::cos(state.cam_angle_x * deg_to_rad),
            state.camera_pov[1] + state.dist_cam * std::sin(state.cam_angle_y * deg_to_rad),
            state.camera_pov[2] + state.dist_cam * std::sin(state.cam_angle_x * deg_to_rad),
            0.0, 1.0, 0.0);
        if (state.cursor_locked) {
            glutWarpPointer(glutGet(GLUT_WINDOW_WIDTH) / 2, glutGet(GLUT_WINDOW_HEIGHT) / 2);
        }
        return;
    }

    gluLookAt(
        state.pan_radius * std::sin(state.pan_angle * deg_to_rad),
        state.pan_height,
        state.pan_radius * std::cos(state.pan_angle * deg_to_rad),
        0.0,
        state.pan_height,
        0.0,
        0.0,
        1.0,
        0.0);
}
