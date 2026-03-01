#include "LoadingScreen.hpp"
#include "src/Engine.hpp"

void LoadingScreen::draw () {
    glClear(GL_COLOR_BUFFER_BIT);
    
    Engine::pTextShader->use();

    // calculate posiiton
    int winW = Engine::pWindow->get_width();
    int winH = Engine::pWindow->get_height();
    // approximate values. in future must be provided by Text class
    int textH = 50;
    int textW = 280;
    float topBias = winH / 2.0f + textH / 2.0f;
    float leftBias = winW / 2.0f - textW / 2.0f;

    glm::mat4 projection = Engine::pCanvas->get_projection();
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(leftBias, topBias, 0.0f));

    Engine::pTextShader->set_matrix4("projection", projection * glm::scale(transform, glm::vec3(3.0f, -3.0f, 3.0f)));
    Engine::pTextShader->set_texture("theTexture", text->get_font()->get_texture()->getID());

    this->text->draw();

    glfwSwapBuffers(Engine::pWindow->get_glfw_window());
    glfwPollEvents();
}