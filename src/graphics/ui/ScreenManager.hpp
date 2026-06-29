#pragma once
#include "IScreen.hpp"
#include "src/logic/pointers.hpp"

class ScreenManager {
private:
    static uptr<IScreen> currentScreen = nullptr;
public:
    static void change_screen() {

    }
    static void update () {

    }
    static void draw () {
        
    }
};