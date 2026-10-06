#pragma once
#include "src/logic/pointers.hpp"
#include "src/graphics/Mesh.hpp"
#include "src/graphics/MeshRenderer.hpp"
#include "src/graphics/Texture.hpp"

class Crosshair {
    uptr<Mesh> mesh = nullptr;
    uptr<MeshRenderer> renderer = nullptr;

    void init() {
        // Square mesh
        std::vector<GLfloat> vertices = {
            -0.5f, -0.5f, 0, 1,
            0.5f, -0.5f, 1, 1,
            -0.5f, 0.5f, 0, 0,
            0.5f, 0.5f, 1, 0
        };
        std::vector<GLuint> indices = {
            3, 1, 0,
            2, 3, 0
        };
        mesh = make_uptr<Mesh>(std::move(vertices), std::move(indices));
        renderer = make_uptr<MeshRenderer>(std::move(mesh), MeshType::SPRITE2D);
        texture = make_uptr<Texture>("res/textures/crosshair.png", true);
    }

public:
    uptr<Texture> texture = nullptr;
    
    Crosshair() {
        init();
    }
    static Crosshair& get_instance() {
        static Crosshair instance;
        return instance;
    }
    Crosshair(const Crosshair&) = delete;
    Crosshair& operator=(const Crosshair&) = delete;

    void draw() {
        if (!texture || !renderer) return;
        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ZERO);
        renderer->draw();
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
};