#pragma once
#include "../cubemap/cubemap.h"
#include <common/shader.hpp>

static float skyboxVertices[] = {
    // positions          
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};

static std::string skyboxShaderVert = R"(
#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    TexCoords = aPos;
    gl_Position = projection * view * vec4(aPos, 1.0);
} )";

static std::string skyboxShaderFrag = R"(
#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{    
    FragColor = texture(skybox, TexCoords);
})";

struct Skybox {
    GLuint _VBO;
    GLuint _VAO;
    GLuint _shaderPID;

    CubeMap cubemap;

    Skybox(CubeMap && c){
        cubemap = std::move(c);

        synchronize();
        // gen VAO and bind all that shi together
    }

    Skybox& operator=(Skybox&& other){
        cubemap = std::move(other.cubemap);
        synchronize();
    }

    Skybox() = default;

    ~Skybox() {
        glDeleteBuffers(1, &_VBO);
        glDeleteProgram(_shaderPID);
        glDeleteVertexArrays(1, &_VAO);

    }

    void synchronize(){

        glGenVertexArrays(1, &_VAO);
        glBindVertexArray(_VAO);

        glGenBuffers(1, &_VBO);
        glBindBuffer(GL_ARRAY_BUFFER, _VBO);

        glBufferData(GL_ARRAY_BUFFER, 36 * sizeof(float), skyboxVertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);


        glEnableVertexAttribArray(0);

        _shaderPID = loadShaders(skyboxShaderVert, skyboxShaderFrag);
    }
    
    void render(const glm::mat4 & view, const glm::mat4 & projection){
        glUseProgram(_shaderPID);
        // https://learnopengl.com/Advanced-OpenGL/Cubemaps
        glBindVertexArray(_VAO);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap.getTextureId());
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

};


class Environment {

public:
    Skybox skybox;

    void render(const glm::mat4 & view, const glm::mat4 & projection){
        skybox.render(view, projection);
    }
};