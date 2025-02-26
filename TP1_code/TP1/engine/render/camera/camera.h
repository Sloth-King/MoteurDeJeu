#pragma once
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


class Camera{

protected:
    glm::mat4 projection;


public:
    glm::mat4 transform;

    float width = 500;
    float height = 500;

    Camera(){
        transform = glm::mat4();

        projection = glm::perspective(
            glm::radians(45.0f),
            width / height,
            0.1f,
            100.0f
        );
    }

    inline glm::mat4 getProjectionMatrix(){
        return projection;
    }

    inline glm::mat4 getViewMatrix(){
        return transform; // TODO once return global transform once we have created the scene class
    }

    inline glm::mat4 getVP(){
        return getProjectionMatrix() * getViewMatrix();
    }
};