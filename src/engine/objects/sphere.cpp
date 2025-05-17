#include <objects/sphere.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <cmath>

namespace sharkyEngine{
    SphereMesh::SphereMesh(const std::string& vsPath,
                        const std::string& fsPath,
                        const std::string& texturePath) 
                        : sphereShader(vsPath.c_str(), fsPath.c_str()){

        // 1) Generate sphere vertices & strip indices
        const int X_SEG = 64, Y_SEG = 64;
        const float PI = 3.14159265359f;
        std::vector<float> verts;
        std::vector<int> inds;
        for(int y = 0; y <= Y_SEG; ++y){
            for(int x = 0; x <= X_SEG; ++x){
                float xSeg = (float)x / X_SEG;
                float ySeg = (float)y / Y_SEG;
                float xPos = std::cos(xSeg * 2*PI) * std::cos(ySeg*PI - PI/2);
                float yPos = std::sin(ySeg*PI - PI/2);
                float zPos = std::sin(xSeg * 2*PI) * std::cos(ySeg*PI - PI/2);

                verts.push_back(xPos);
                verts.push_back(yPos);
                verts.push_back(zPos);
                verts.push_back(xSeg);
                verts.push_back(ySeg);
            }
        }

        bool odd = false;
        for(int y = 0; y < Y_SEG; ++y){
            if(!odd){
                for (int x = 0; x <= X_SEG; ++x) {
                    inds.push_back(y*(X_SEG+1) + x);
                    inds.push_back((y+1)*(X_SEG+1) + x);
                }
            } 
            else{
                for(int x = X_SEG; x >= 0; --x){
                    inds.push_back((y+1)*(X_SEG+1) + x);
                    inds.push_back(y*(X_SEG+1) + x);
                }
            }
            odd = !odd;
        }
        indexCount = (GLsizei)inds.size();
        GLsizeiptr vertByteCount = static_cast<GLsizeiptr>(verts.size() * sizeof(float));
        GLsizeiptr indexByteCount = static_cast<GLsizeiptr>(inds.size() * sizeof(int));

        // 2) Upload to GPU
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertByteCount, verts.data(), GL_STATIC_DRAW);

        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexByteCount, inds.data(), GL_STATIC_DRAW);

        // pos (3 floats) + tex (2 floats)
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));

        glBindVertexArray(0);

        // 3) Load texture
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        int w,h,n;
        stbi_set_flip_vertically_on_load(true);
        unsigned char* data = stbi_load(texturePath.c_str(), &w, &h, &n, 0);
        if (data){
            GLenum fmt = (n == 4 ? GL_RGBA : GL_RGB);
            glTexImage2D(GL_TEXTURE_2D, 0, static_cast<int>(fmt), w, h, 0, fmt, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        } 
        else
            std::cerr << "Failed to load texture: " << texturePath << "\n";
        stbi_image_free(data);

        // Tell shader which texture unit
        sphereShader.use();
        sphereShader.setInt("texture1", 0);
    }

    SphereMesh::~SphereMesh() {
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        glDeleteVertexArrays(1, &VAO);
        glDeleteTextures(1, &textureID);
    }

    void SphereMesh::draw() {
        float time = static_cast<float>(glfwGetTime());
        sphereShader.use();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(90.0f), 800.0f / 600.0f, 0.1f, 100.0f);
        sphereShader.setMat4("projection", projection);
        
        //camera
        glm::mat4 view;
        const float radius = 20.0f;
        float camX = glm::sin(time) * radius;
        float camZ = glm::cos(time) * radius;
        view = glm::lookAt(glm::vec3(camX, 70.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0)); 
        sphereShader.setMat4("view", view);

        glm::mat4 model = glm::mat4(1.0f);
        glBindVertexArray(VAO);

        for(int i = 0; i < 10; i++){
            model = glm::mat4(1.0f);
            float orbitSpeed = glm::radians(30.0f);
            float selfRotateSpeed = glm::radians(60.0f);
            float radius = 15 * static_cast<float>(i);
                
            if(i > 0)
                orbitSpeed = glm::radians(180.0f / (float)i);

            model = glm::rotate(model, time * orbitSpeed, glm::vec3(0.0f, 1.0f, 0.0f));
            if(i == 0)
                model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));

            else{
                model = glm::translate(model, glm::vec3(radius, 0.0f, 0.0f));
                model = glm::rotate(model, time * selfRotateSpeed, glm::vec3(1.0f, 0.5f, 0.5f));
                model = glm::scale(model, glm::vec3(1.0f * static_cast<float>(i), 
                                                    1.0f * static_cast<float>(i), 
                                                    1.0f * static_cast<float>(i)));
            } 
            sphereShader.setMat4("model", model);  
            glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
        }
        glBindVertexArray(0);
    }
} 