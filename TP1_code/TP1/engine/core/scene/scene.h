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

#include "TP1/engine/core/gameObject/gameObject.h"
#include "TP1/engine/utils/utils.h"
#include "TP1/engine/render/camera/camera.h"

class Scene{
//private:
public:
    Camera* current_camera;
    std::unique_ptr< GameObject > root;

public:

    Scene() = default;

    Camera& getCurrentCamera(){
        return *current_camera;
    }
    void setCurrentCamera(Camera& c){
        current_camera = &c;
    }

    void setRoot(GameObject && g){
        root = std::make_unique<GameObject>(std::move(g));
        root->scene = this;
    }
    
    void __engineUpdate(float deltaTime){
        if (current_camera && root){
            current_camera->computeMatricesFromInputs();
            root->__engineUpdate(deltaTime);
        }
    }

    void __enginePhysicsUpdate(float deltaTime){
        if (root)
            root->__enginePhysicsUpdate(deltaTime);
    }
};