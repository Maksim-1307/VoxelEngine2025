#pragma once

#define GLM_FORCE_CTOR_INIT

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "src/logic/Settings.hpp"

struct WindowArgs
{
    std::string name = "OpenGL Window";
    GLuint width = Settings::WINDOW_WIDTH;
    GLuint height = Settings::WINDOW_HEIGHT;
    bool isResizable = true;
    glm::vec3 clearColor = glm::vec3(0.0f, 0.0f, 0.0f);
};

class Engine;

class Window
{

public:
    Window(WindowArgs args);
    ~Window();

    bool should_close();

    GLFWwindow *get_glfw_window()
    {
        return GLFWwindow;
    }

    GLuint get_width()
    {
        return this->width;
    }
    GLuint get_height()
    {
        return this->height;
    }
    void set_width(GLuint value)
    {
        this->width = value;
    }
    void set_height(GLuint value)
    {
        this->height = value;
    }
    float get_ratio()
    {
        return (float)this->width / this->height;
    }
    void set_mouse_lock(bool active) {
        if (active)
            glfwSetInputMode(this->GLFWwindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        else
            glfwSetInputMode(this->GLFWwindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    void set_clear_color(glm::vec3 color) {
        this->clearColor = color;
        glClearColor(clearColor.x, clearColor.y, clearColor.z, 1.0f);
    }
private:
    GLuint width;
    GLuint height;
    glm::vec3 clearColor;

    GLFWwindow *GLFWwindow;
};

void windowSizeCallback(GLFWwindow *pWindow, int width, int height);
