#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <GL/glew.h>
#include "engine/external/stb_image.h"




class Texture {
// https://learnopengl.com/Getting-started/Textures

protected:
    unsigned char * data; // maybe duplicated and we could free it but keep it just in case for now
    GLuint _texture_id;

public:
    int width, height;
    int nbChannels;

    Texture(const std::string path){
        loadTexture(path);
        
    }

    void loadTexture(const std::string path){
        data = stbi_load(path.c_str(), &width, &height, &nbChannels, 0);
        if (!data) std::cout << " TEXTURE MAL CHARGEE" << std::endl;
        //std::cout << "nbchannels: " << nbChannels << std::endl;

        synchronize();

    }
    GLuint getTextureId(){ return _texture_id;};

    void synchronize(){ // TODO unsynchronize if already synchronized. Otherwise this is a memory leak :)
        
        
        glGenTextures(1, &_texture_id);
        glBindTexture(GL_TEXTURE_2D, _texture_id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D); 
    }


    // IN CASE STRANGE THINGS HAPPEN: ALWAYS REMEMBER https://learnopengl.com/Getting-started/Textures
    void bind(GLuint slot = 0){
        glActiveTexture(GL_TEXTURE0+slot); // activate the texture unit first before binding texture
        glBindTexture(GL_TEXTURE_2D, _texture_id);
    }
};


