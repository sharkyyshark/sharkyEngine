#include <glad/glad.h>  
#include <GLFW/glfw3.h>

#include <sharkyEngine/renderer.h>
#include <sharkyEngine/engine.h>

namespace sharkyEngine{
    Engine::Engine(int width, int height, const std::string& title) 
    : window(width, height, title) {}

    void Engine::run(){
        while (!window.shouldClose()){
            window.processInput();

            renderer.renderScene();

            window.update();
        }
    }
} 
