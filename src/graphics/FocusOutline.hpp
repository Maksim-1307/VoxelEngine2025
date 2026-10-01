#pragma once

#include "src/logic/pointers.hpp"
#include "src/graphics/Mesh.hpp"
#include "src/graphics/MeshRenderer.hpp"
#include "src/physics/AABB.hpp"

class FocusOutline {
public:
    static FocusOutline& instance() {
        static FocusOutline inst;
        return inst;
    }
    FocusOutline(const FocusOutline&) = delete;
    FocusOutline& operator=(const FocusOutline&) = delete;

    void draw();
    void update();
private:
    FocusOutline() {
        update();
    }
    ~FocusOutline() = default;
    sptr<Mesh> build_mesh(std::vector<AABB> AABBs, glm::vec3 off);

    uptr<MeshRenderer> renderer;
};