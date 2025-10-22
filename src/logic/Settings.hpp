#pragma once

class Settings {
public:
    static constexpr int LOAD_DISTANCE = 5;
    static constexpr bool RECURSIVE_LIGHTING = false;
    static constexpr bool HARD_LOADING = false;
    static constexpr int WINDOW_WIDTH = 640;
    static constexpr int WINDOW_HEIGHT = 480;
    static constexpr bool MOUSE_CONTROL = true;
    static constexpr float MOUSE_SENSITIVITY = 20.0f;
    static constexpr float BLOCK_BREAKING_DELAY = 0.3f;
    static constexpr float BLOCK_PLACIND_DELAY = 0.2f;
    static constexpr float PLAYER_SPEED = 5.0f;
};
