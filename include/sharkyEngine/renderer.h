#pragma once

#include <objects/sphere.h>
#include <shaderclass/shader.h>

namespace sharkyEngine{
    class Renderer{
        private:
        SphereMesh sphere;

        public:
        Renderer();
        void renderScene();
        void applyTransormations();
        void applyTexture();
    };
}