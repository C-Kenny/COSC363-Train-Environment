#ifndef INPUT_CONTROLLER_H
#define INPUT_CONTROLLER_H

#include "world_state.h"

class InputController {
public:
    explicit InputController(WorldState& state);

    void handleMovementKey(unsigned char key, float deg_to_rad);
    void togglePanMode();
    void releaseCursorLock();
    void lockCursor();
    void toggleCursorLock();
    void mouseMove(
        int x,
        int y,
        float mouse_rotation,
        float deg_to_rad,
        bool cursor_locked,
        int center_x,
        int center_y);

private:
    WorldState& state_;
};

#endif
