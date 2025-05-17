#pragma once

#include <sharkyEngine/window.h>
#include <sharkyEngine/renderer.h>
#include <string>

namespace sharkyEngine {
    class Engine{
    private:
        Window window;
        Renderer renderer;

    public:
        Engine(int width, int height, const std::string& title);
        ~Engine() = default;

        // No copying
        Engine(const Engine&) = delete;
        Engine& operator = (const Engine&) = delete;

        // Start the main loop
        void run();
    };
} 