#pragma once

#include <iostream>
#include <algorithm>

class FPSCounter {
public:
    FPSCounter(){};
    void update(float deltaTime);

    uint get_fps() {
        return fps;
    }

private:
    uint fps = 0;
    uint minFps = 0;
    uint frames = 0;
    float totalTime = 0.0f;
};