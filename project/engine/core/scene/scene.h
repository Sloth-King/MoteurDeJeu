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

#include "engine/core/resourceLoader/resourceLoader.h"

#include "engine/core/gameObject/gameObject.h"
#include "engine/utils/utils.h"
#include "engine/render/camera/camera.h"

class Scene{

    Camera debugCamera;
//private:
public:
    Camera* current_camera = nullptr;
    std::unique_ptr< GameObject > root = nullptr;

public:

    Scene() = default;

    Camera& getCurrentCamera(){
        return *current_camera;
    }
    void setCurrentCamera(Camera& c){
        current_camera = &c;
    }

    void setRoot(GameObject && g){ // async for now, probably not a good idea but we'll manage that later
        if (root) root->__exitScene();

        root = std::make_unique<GameObject>(std::move(g));
        root->__enterScene(this);
    }
    
    void __engineUpdate(float deltaTime){

        if (!current_camera){
            std::cout << "[SCENE] No camera detected. Defaulting to degubCamera." << std::endl;
            setCurrentCamera(debugCamera);
        }
        

        if (root){
            current_camera->computeMatricesFromInputs();
            root->__engineUpdate(deltaTime);
        }
    }

    void __enginePhysicsUpdate(float deltaTime){
        if (root)
            root->__enginePhysicsUpdate(deltaTime);
    }
};