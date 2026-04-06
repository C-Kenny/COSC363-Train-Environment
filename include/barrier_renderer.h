#ifndef BARRIER_RENDERER_H
#define BARRIER_RENDERER_H

#include "world_state.h"

class BarrierRenderer {
public:
    static void drawBarrierSurface(float cutoff_z_barrier, unsigned int texture_id);
    static void drawBarrierBody(float cutoff_z_barrier, unsigned int texture_id);
    static void drawTrafficLight(bool stop_light, float cutoff_z_barrier);
    static void drawAnimatedBarrierArm(float barrier_theta, float cutoff_z_barrier, unsigned int texture_id);
};

#endif
