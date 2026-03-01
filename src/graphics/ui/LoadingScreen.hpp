#pragma once
#include "IScreen.hpp"
#include "src/graphics/Text.hpp"
#include "src/logic/pointers.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class LoadingScreen : public IScreen {
private:
    float prevClearColor[4];
    uptr<Text> text = nullptr;
public:
    LoadingScreen(){
        std::string s = "Voxel Game";
        text = make_uptr<Text>(s);
    }
    ~LoadingScreen(){}

    float screenColor[4] = {0,0,0,0};

    void on_enter () override {
        glGetFloatv(GL_COLOR_CLEAR_VALUE, prevClearColor);
        glClearColor(screenColor[0], screenColor[1], screenColor[2], screenColor[3]);
    }

    void on_exit () override {
        glClearColor(prevClearColor[0], prevClearColor[1], prevClearColor[2], prevClearColor[3]);
    }

    void draw () override;

    void update () override {}
};