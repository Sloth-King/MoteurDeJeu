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
    vec4 pos = projection * view * vec4(aPos, 1.0);
    gl_Position = pos.xyww; // where the magic occurs
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

    bool _synchronized = false;

    Skybox(CubeMap && c){
        cubemap = std::move(c);

        synchronize();
        // gen VAO and bind all that shi together
    }

    Skybox& operator=(Skybox&& other){
        cubemap = std::move(other.cubemap);
        other._synchronized = false;
        synchronize();
        return *this;
    }

    Skybox(Skybox && other){
        *this = std::move(other);
    }

    Skybox() = default;

    ~Skybox() {
        glDeleteBuffers(1, &_VBO);
        glDeleteProgram(_shaderPID);
        glDeleteVertexArrays(1, &_VAO);

    }

    void synchronize(){
        _synchronized = true;

        glGenVertexArrays(1, &_VAO);
        glBindVertexArray(_VAO);
        glEnableVertexAttribArray(0);

        glGenBuffers(1, &_VBO);
        glBindBuffer(GL_ARRAY_BUFFER, _VBO);

        glBufferData(GL_ARRAY_BUFFER, 36 * sizeof(float), &skyboxVertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);


        _shaderPID = loadShaders(skyboxShaderVert, skyboxShaderFrag);

        glDisableVertexAttribArray(0);
    }
    
    void render(glm::mat4 view, const glm::mat4 & projection){

        if (!_synchronized) synchronize();

        std::cout << _synchronized << std::endl;

        // https://learnopengl.com/Advanced-OpenGL/Cubemaps
        // removes translation
        view[3] = glm::vec4(0);

        glUseProgram(_shaderPID);

        glUniformMatrix4fv(
            glGetUniformLocation(_shaderPID, "view"),
            1,
            GL_FALSE,
            &view[0][0]
        );

        glUniformMatrix4fv(
            glGetUniformLocation(_shaderPID, "projection"),
            1,
            GL_FALSE,
            &projection[0][0]
        );

        cubemap.bind(0);

        glUniform1i(
            glGetUniformLocation(_shaderPID, "skybox"),
            0
        );

        glBindVertexArray(_VAO);
        glEnableVertexAttribArray(0);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glDisableVertexAttribArray(0);
        glBindVertexArray(0);
        glUseProgram(0);
    }
};


class Environment {
 // pour l'instant seulement skybox mais peut changer

public:
    Skybox skybox;

    Environment() = default;

    Environment(Environment && c){
        skybox = std::move(c.skybox);
    }

    Environment& operator=(Environment&& other){
        skybox = std::move(other.skybox);
        return *this;
    }

    void render(const glm::mat4 & view, const glm::mat4 & projection){
        skybox.render(view, projection);
    }
};