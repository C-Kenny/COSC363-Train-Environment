#ifndef ANIMATION_SYSTEM_H
#define ANIMATION_SYSTEM_H

#include "world_state.h"

class AnimationSystem {
public:
    explicit AnimationSystem(WorldState& state);

    void panStep();
    void linearTrainStep(float track_length);
    void barrierStep();
    void refreshStopLight();

private:
    WorldState& state_;
};

#endif
