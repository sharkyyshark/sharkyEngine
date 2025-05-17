#pragma once

#include <string>

struct GLFWwindow;

namespace sharkyEngine{
    class Window{
    public:
        Window(int width, int height, const std::string& title);
        ~Window();

        // No copying
        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;

        // Main loop helpers
        void update();               // swap buffers and poll events
        bool shouldClose() const;    // check if the window should close

        // Resize tracking
        int getWidth() const;
        int getHeight() const;
        bool isResized() const;      // was the window resized?
        void resetResizedFlag();     // clear the resize flag

        // Process basic input (escape key to close)
        void processInput();

    private:
        GLFWwindow* handle = nullptr;
        int width;
        int height;
        bool resized = false;
    };
} 