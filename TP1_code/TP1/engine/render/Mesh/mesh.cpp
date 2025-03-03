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

#include "TP1/engine/core/resourceLoader/resourceLoader.h"
#include "TP1/engine/utils/utils.h"



GLuint TRI_GL_TYPE = GL_UNSIGNED_INT; // change with TRI_IDX_TYPE in mesh.h!


void Mesh::debug_draw(){

    glEnableClientState( GL_VERTEX_ARRAY );
    glPointSize(8.0);
    glColor3f(1.0, 0.0, 0.0);
    glVertexPointer( 3, GL_FLOAT, sizeof(glm::vec3), &vertices[0].x );
    glDrawArrays( GL_POINTS, 0, vertices.size() );
    glDisableClientState( GL_VERTEX_ARRAY );
    glFlush();
}

void Mesh::synchronize(){

    if (_synchronized){ // maybe we just want to resync
        unsynchronize();
    }
    std::cout << "TAILLE " << sizeof(Triangle) << std::endl;
    
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


    glDisableVertexAttribArray(0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    _synchronized = true;
}

void Mesh::render(glm::mat4 vpMatrix){

    if (!_synchronized){ // FIXME branch prediction may bottleneck a little here?
        synchronize();
    }
    glUseProgram(shaderPID);
    
    GLuint mvpUniformLocation = glGetUniformLocation(shaderPID, "MVP");

    vpMatrix = vpMatrix * transform; // transform the VP into MVP



    if (mvpUniformLocation != -1){
        glUniformMatrix4fv(mvpUniformLocation, 1, GL_FALSE, &vpMatrix[0][0]);
    }

    glBindVertexArray(_VAO);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);

    // Draw the triangles !

    glVertexAttribPointer(
        0,                  // attribute
        3,    // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,  // stride
        (void*)0            // array buffer offset
    );

    // Index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);

    glDrawElements(
                GL_TRIANGLES,      // mode
                triangles.size()*3,    // count
                TRI_GL_TYPE,   // type
                (void*)0           // element array buffer offset
                );
    

    glDisableVertexAttribArray(0);
    glBindVertexArray(0);
    glUseProgram(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::unsynchronize(){
    glDeleteBuffers(1, &_VBO);
    glDeleteBuffers(1, &_EBO);
    glDeleteProgram(shaderPID);
    glDeleteVertexArrays(1, &_VAO);

}

void Mesh::setShader(std::string vertex_shader, std::string fragment_shader){
    shaderPID = LoadShaders(vertex_shader.c_str(), fragment_shader.c_str());
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