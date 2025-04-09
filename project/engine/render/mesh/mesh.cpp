#include "mesh.h"

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <common/shader.hpp>
#include <common/objloader.hpp>
#include <common/vboindexer.hpp>

#include "engine/core/resourceLoader/resourceLoader.h"
#include "engine/utils/utils.h"


GLuint TRI_GL_TYPE = GL_UNSIGNED_INT; // change with TRI_IDX_TYPE in mesh.h!


void Mesh::synchronize() const {

    if (_synchronized){ // maybe we just want to resync
        unsynchronize();
    }
    
    glGenVertexArrays(1, &_VAO);
    glBindVertexArray(_VAO);

    glGenBuffers(1, &_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);


    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

    // Generate a buffer for the indices as well

    glGenBuffers(1, &_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, triangles.size() * sizeof(Triangle), &triangles[0] , GL_STATIC_DRAW);


    glBindBuffer(GL_ARRAY_BUFFER, _VBO);


    glGenBuffers(1, &_UV);
    glBindBuffer(GL_ARRAY_BUFFER, _UV);
    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0] , GL_STATIC_DRAW);

    glGenBuffers(1, &_NORMALS);
    glBindBuffer(GL_ARRAY_BUFFER, _NORMALS);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0] , GL_STATIC_DRAW);



    glDisableVertexAttribArray(0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);



    // textures

    // TODO synchronize texture here aswell but not now since texture still can't "unsychronize"

    /*
    glUseProgram(shaderPID);
    
    for (auto t: textures){
        glUniform1i(
            glGetUniformLocation(shaderPID, t.second.c_str()),
            t.first.getTextureId()
        );
    }*/

    _synchronized = true;
}

void Mesh::render(const glm::mat4 & vpMatrix, glm::vec3 fv, const glm::mat4 & outside_transform) const{

    if (vertices.empty()) return;

    if (!_synchronized){ // FIXME branch prediction may bottleneck a little here? idk
        synchronize();
    }
    glUseProgram(shaderPID);

    int i = 0;
    for (auto t: textures){
        t.first.bind(i);
        glUniform1i(
            glGetUniformLocation(shaderPID, t.second.c_str()),
            i
        );
        ++i;
    }
    
    // todo just tranfer a struct to the gpu with all interesting data
    GLuint mvpUniformLocation = glGetUniformLocation(shaderPID, "MVP");
    GLuint camForardLocation = glGetUniformLocation(shaderPID, "view");
    GLuint modelLocation = glGetUniformLocation(shaderPID, "Model");

    glm::mat4 MVP = vpMatrix * outside_transform * transform; // transform the VP into MVP



    if (mvpUniformLocation != -1){
        glUniformMatrix4fv(mvpUniformLocation, 1, GL_FALSE, &MVP[0][0]);
    }

    if (camForardLocation != -1){
        glUniform3fv(camForardLocation, 1, &fv[0]);
    }
    if (modelLocation != -1){
        glUniformMatrix4fv(modelLocation, 1, GL_FALSE,  &transform[0][0]);
    }

    glBindVertexArray(_VAO);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);

    // Positions attribute buffer

    glVertexAttribPointer(
        0,                  // attribute
        3,    // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,  // stride
        (void*)0            // array buffer offset
    );


    // Uvs attribute buffer
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, _UV);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0);




    // normals
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, _NORMALS);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);


    // Index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
    
    glDrawElements(
                GL_TRIANGLES,      // mode
                triangles.size()*3,    // count
                TRI_GL_TYPE,   // type
                (void*)0           // element array buffer offset
                );
    

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glBindVertexArray(0);
    glUseProgram(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::unsynchronize() const {
    glDeleteBuffers(1, &_VBO);
    glDeleteBuffers(1, &_EBO);
    glDeleteBuffers(1, &_UV);
    glDeleteBuffers(1, &_NORMALS);
    glDeleteProgram(shaderPID);
    glDeleteVertexArrays(1, &_VAO);
    
    _synchronized = false;

}

void Mesh::setShader(std::string vertex_shader, std::string fragment_shader){
    shaderPID = loadShadersFromFileGLSL(vertex_shader.c_str(), fragment_shader.c_str());
}

void Mesh::recomputeNormals () {
    normals.resize(vertices.size());
    for (unsigned int i = 0; i < vertices.size (); i++)
        normals[i] = glm::vec3(0.0, 0.0, 0.0);
    for (unsigned int i = 0; i < triangles.size (); i++) {
        glm::vec3 e01 = vertices[triangles[i].v[1]] -  vertices[triangles[i].v[0]];
        glm::vec3 e02 = vertices[triangles[i].v[2]] -  vertices[triangles[i].v[0]];
        glm::vec3 n = glm::cross(e01, e02);
        n = glm::normalize(n);
        for (unsigned int j = 0; j < 3; j++)
            normals[triangles[i].v[j]] += n;
    }
    for (unsigned int i = 0; i < vertices.size (); i++)
        normals[i] = glm::normalize(normals[i]);
}



GLuint Mesh::base_shader = 0;

const std::string base_shader_vert = R"(#version 330 core

    layout(location = 0) in vec3 vertices_position_modelspace;
    layout(location = 1) in vec2 raw_uv;
    layout(location = 2) in vec3 raw_normals;

    uniform mat4 Model;
    uniform mat4 MVP;

    out vec2 uv;
    out vec3 normal;

    void main(){

            uv = raw_uv;
            mat4 norm_transform = transpose(inverse(Model));
            normal = (norm_transform * vec4(raw_normals, 0)).xyz;


            vec4 pos = vec4(vertices_position_modelspace,1);

            gl_Position = MVP * pos;
    })";

const std::string base_shader_frag = R"(#version 330 core

    uniform vec3 view;

    in vec2 uv;
    in vec3 normal;

    out vec3 color;
    void main(){


        color = vec3(0.8, 0.6, 0.2) * clamp(dot(view, normal)+0.2, 0.0, 1.0);
    }
    )";

void Mesh::createDefaultShader(){
    Mesh::base_shader = loadShaders(base_shader_vert, base_shader_frag);
}