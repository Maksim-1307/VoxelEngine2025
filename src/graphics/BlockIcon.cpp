#include "BlockIcon.hpp"

#include "src/graphics/Mesh.hpp"
#include "src/graphics/MeshRenderer.hpp"
#include "src/Engine.hpp"

BlockIcon::BlockIcon(int blockId, int size) {
    this->blockId = blockId;
    mesh = make_sptr<Mesh>(std::move(vertices), std::move(indices));
    renderer = new MeshRenderer(mesh, MeshType::SPRITE2D);
    make_texture(size);
}

void BlockIcon::draw() {
    renderer->draw();
}

void BlockIcon::make_texture(int size) {

    // Getting default values
    float clearColor[4];
    glGetFloatv(GL_COLOR_CLEAR_VALUE, clearColor);
    float viewPortSize[4];
    glGetFloatv(GL_VIEWPORT, viewPortSize);
    GLboolean wasBlendEnabled = glIsEnabled(GL_BLEND);
    
    // Create framebuffer
    GLuint fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    // Create texture
    texture = new Texture(size, size);

    // Attach texture to framebuffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->getID(), 0);

    // Create depth buffer
    GLuint rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, size, size);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    // Check if framebuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "Framebuffer is not complete!" << std::endl;
    }

    // Rendering in texture
    glViewport(0, 0, size, size);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Engine::pMeshShader->use();
    glm::mat4 projection = Engine::pCamera->getOrthographicProjection(size, size);
    glm::mat4 view = Engine::pCamera->getViewFromNullTo(glm::vec3(0.597164f, 0.584157f, 0.549688f));
    Engine::pMeshShader->set_matrix4("model", glm::mat4(1.0f));
    Engine::pMeshShader->set_matrix4("view", view);
    Engine::pMeshShader->set_matrix4("projection", projection);
    
    Engine::pMeshShader->set_texture("theTexture", Engine::pTexture->getID());
    uptr<Mesh> mesh = Engine::pBlockMeshBuilder->buildMesh(Block::getBlockByVoxelId(blockId));
    uptr<MeshRenderer> blockRenderer = make_uptr<MeshRenderer>(std::move(mesh), MeshType::MESH3D);

    blockRenderer->draw();

    // Return to the default framebuffer and values
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
    glViewport(viewPortSize[0], viewPortSize[1], viewPortSize[2], viewPortSize[3]);
    if (!wasBlendEnabled) glDisable(GL_BLEND);
}

Texture* BlockIcon::getTexture() {
    if (texture == nullptr) { 
        make_texture(32);
    }
    return texture;
}