#include "FPSCounter.hpp"
#include "src/Engine.hpp"

// must be called every frame
void FPSCounter::update(float deltaTime){
    frames++;
    totalTime += deltaTime;
    minFps = (uint)std::min((float)minFps, 1.0f / deltaTime);
    if (totalTime >= 1.0f)
    {
        fps = frames;
        std::cout << "fps: " << fps << " | " << minFps << std::endl;
        display(fps, minFps);
        frames = 0;
        minFps = fps;
        totalTime = 0.0f;
    }
}

void FPSCounter::display(uint fps, uint minFps) {
    std::string s = "fps: " + std::to_string(fps) + "\nmin: " + std::to_string(minFps);
    Engine::pFpsText->update(s);
}
