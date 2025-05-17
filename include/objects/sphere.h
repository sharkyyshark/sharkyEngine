#pragma once

#include <glad/glad.h>
#include <vector>
#include <string>
#include <shaderclass/shader.h>

namespace sharkyEngine{
    class SphereMesh{
    private:
        GLuint VAO, VBO, EBO;
        GLsizei indexCount;
        Shader sphereShader;
        GLuint textureID;

    public:
        SphereMesh(const std::string& vsPath, const std::string& fsPath, const std::string& texturePath);
        ~SphereMesh();
        void draw();
    };
}