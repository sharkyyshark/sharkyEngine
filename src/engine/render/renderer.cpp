#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <sharkyEngine/renderer.h>

namespace sharkyEngine{
    Renderer::Renderer() : sphere("/home/someone/Projects/c++/sharkyEngine/assets/shaders/shader.vs", 
                                  "/home/someone/Projects/c++/sharkyEngine/assets/shaders/shader.fs", 
                                  "/home/someone/Projects/c++/sharkyEngine/assets/textures/vhs.jpg") {}

    void Renderer::renderScene(){
        // Clear screen
        glClearColor(0.08627450980392157f, 0.08627450980392157f, 0.08627450980392157f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // TODO: render scene here
        sphere.draw(); 
    }
}