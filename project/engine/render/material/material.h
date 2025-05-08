#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <string>
#include <map>
#include <utility>
#include <vector>
#include <GL/glew.h>
#include <common/shader.hpp>

#include "engine/render/texture/texture.h"


class Material {
    GLuint _shaderPID;

    std::map< std::string, Texture > uniformTextures;
    std::map< std::string, glm::mat4 > uniformMat4s;
    std::map< std::string, int > uniformInts;
    std::map< std::string, float > uniformFloats;


    // not secured. What if someone add same name in different types?
    template<typename T>
    void addUniform(std::string name, T v);

    void bind() const; // TODO prendre les matrices en arg

    void setShaderFromSource(std::string shaderCode){
        // TODO
        // ajouter le calcul des matrices de base etc
    }

     void setShaderFromFile(std::string path){
        setShaderFromSource("le code loadé à partir du path");
    }
};