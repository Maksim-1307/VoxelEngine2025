#include "FPSCounter.hpp"

// must be called every frame
void FPSCounter::update(float deltaTime){
    frames++;
    totalTime += deltaTime;
    minFps = (uint)std::min((float)minFps, 1.0f / deltaTime);
    if (totalTime >= 1.0f)
    {
        fps = frames;
        std::cout << "fps: " << fps << " | " << minFps << std::endl;
        frames = 0;
        minFps = fps;
        totalTime = 0.0f;
    }
}


