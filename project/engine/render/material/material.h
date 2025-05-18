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



class Mesh;
class Material {
friend Mesh;
    GLuint _shaderPID = 0;

    std::map< std::string, Texture > uniformTextures;
    std::map< std::string, glm::mat4 > uniformMat4s;
    std::map< std::string, int > uniformInts;
    std::map< std::string, float > uniformFloats;


public:
    // not secured. What if someone adds same name in different types?

    void setUniform(const std::string name, Texture v);
    void setUniform(const std::string name, glm::mat4 v);
    void setUniform(const std::string name, int v);
    void setUniform(const std::string name, float v);




    void bind() const; // TODO prendre les matrices en arg

    void setShaderFromSource(const std::string shaderVert, std::string shaderFrag);
;

    void setShaderFromFile(std::string path){
        setShaderFromSource("le code loadé à partir du path", "");
    }
};




// used for now with deferred rendering. Additional args wont change much
class MaterialPBR: private Material {
friend Mesh;

public:
    MaterialPBR() = default;

    MaterialPBR(const Texture albedo, const Texture & normal, const Texture & roughness, const Texture & metallic){
        setUniform("tex_albedo", albedo);
        setUniform("tex_normal", normal);
        setUniform("tex_roughness", roughness);
        setUniform("tex_metallic", metallic);
    }

};