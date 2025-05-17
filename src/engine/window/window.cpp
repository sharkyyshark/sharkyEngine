#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <sharkyEngine/window.h>
#include <iostream>

namespace sharkyEngine{
    static void framebufferResizeCallback(GLFWwindow* win, int w, int h){
        glViewport(0, 0, w, h);
    }

    Window::Window(int w, int h, const std::string& title): width(w), height(h){
        if(!glfwInit()){
            std::cerr << "failed to init glfw" << std::endl;
            std::abort();
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        handle = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
        if(!handle){
            std::cerr << "failed to create window" << std::endl;
            glfwTerminate();
        }

        glfwMakeContextCurrent(handle);
        glfwSetWindowUserPointer(handle, this);
        glfwSetFramebufferSizeCallback(handle, framebufferResizeCallback);

        if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
            std::cerr << "failed to init glad" << std::endl;
            glfwDestroyWindow(handle);
            glfwTerminate();
        }
    }

    Window::~Window(){
        if (handle)
            glfwDestroyWindow(handle);
        glfwTerminate();
    }

    void Window::update(){
        glfwSwapBuffers(handle);
        glfwPollEvents();
    }

    bool Window::shouldClose() const{
        return glfwWindowShouldClose(handle);
    }

    int Window::getWidth() const{
        return width;
    }

    int Window::getHeight() const{
        return height;
    }

    bool Window::isResized() const{
        return resized;
    }

    void Window::resetResizedFlag(){
        resized = false;
    }

    void Window::processInput() {
        if(glfwGetKey(handle, GLFW_KEY_ESCAPE) == GLFW_PRESS){
            std::cout << "goodbye cruel world\n";
            glfwSetWindowShouldClose(handle, true);
        }
    }
} 
