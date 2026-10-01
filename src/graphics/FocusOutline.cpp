#include "FocusOutline.hpp"

#include "src/Engine.hpp"
#include "src/graphics/Mesh.hpp"

void FocusOutline::update () {
    // Raycast to get AABBs of the focused block
    auto camera = Engine::pCamera;
    RaycastResult result = Engine::pTerrain->raycast(
        camera->position, 
        camera->front, 
        15.0f
    );
    if (!result.hit) {
        renderer = nullptr;
        return;
    }
    int x = std::floor(result.position.x);
    int y = std::floor(result.position.y);
    int z = std::floor(result.position.z);
    glm::vec3 off(x, y, z); // global coords of block
    std::vector<AABB> AABBs = Block::getAABBs(x, y, z, RAYCAST);

    // Update the renderer
    renderer = make_uptr<MeshRenderer>(build_mesh(AABBs, off), MeshType::LINE3D);
    
}

sptr<Mesh> FocusOutline::build_mesh(std::vector<AABB> AABBs, glm::vec3 off) {
    std::vector<GLfloat> vertices;
    std::vector<GLuint>  indices;

    vertices.reserve(AABBs.size() * 8 * 7); // 8 vertices * 7 float
    indices.reserve(AABBs.size() * 24); // 12 edges * 2 indexes

    // Cube indexes
    static const GLuint cubeEdges[24] = {
        0, 1,  1, 2,  2, 3,  3, 0,   // lower face
        4, 5,  5, 6,  6, 7,  7, 4,   // upper face
        0, 4,  1, 5,  2, 6,  3, 7    // vertical faces
    };

    // Outline color
    const GLfloat R = 0.0f, G = 0.0f, B = 0.0f, A = 1.0f;

    for (size_t i = 0; i < AABBs.size(); ++i) {
        const AABB& aabb = AABBs[i];
        const float indentation = 0.01f;
        glm::vec3 mn = aabb.min() + off - indentation;
        glm::vec3 mx = aabb.max() + off + indentation;

        // 8 vertices AABB in order:
        // 0:(min.x,min.y,min.z) 1:(max.x,min.y,min.z)
        // 2:(max.x,min.y,max.z) 3:(min.x,min.y,max.z)
        // 4:(min.x,max.y,min.z) 5:(max.x,max.y,min.z)
        // 6:(max.x,max.y,max.z) 7:(min.x,max.y,max.z)
        const glm::vec3 corners[8] = {
            { mn.x, mn.y, mn.z },
            { mx.x, mn.y, mn.z },
            { mx.x, mn.y, mx.z },
            { mn.x, mn.y, mx.z },
            { mn.x, mx.y, mn.z },
            { mx.x, mx.y, mn.z },
            { mx.x, mx.y, mx.z },
            { mn.x, mx.y, mx.z },
        };

        GLuint ib = static_cast<GLuint>(i * 8); // index bias: 8 vertices per AABB

        for (const glm::vec3& c : corners) {
            vertices.push_back(c.x);
            vertices.push_back(c.y);
            vertices.push_back(c.z);
            vertices.push_back(R);
            vertices.push_back(G);
            vertices.push_back(B);
            vertices.push_back(A);
        }

        for (int e = 0; e < 24; e += 2) {
            indices.push_back(ib + cubeEdges[e]);
            indices.push_back(ib + cubeEdges[e + 1]);
        }
    }

    return make_sptr<Mesh>(std::move(vertices), std::move(indices));
}

void FocusOutline::draw () {
    if (renderer) renderer->draw();
}