#ifndef RENDERER_H
#define RENDERER_H

#include "world_state.h"

class Renderer {
public:
    static void applyCamera(const WorldState& state, float deg_to_rad);
};

#endif
