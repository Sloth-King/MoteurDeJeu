#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "engine/core/component/component.h"
#include <iostream>

class C_Transform: public Component {

private:

    glm::mat4 local = glm::mat4(1.0f);
    glm::mat4 global = glm::mat4(1.0f);

    bool local_dirty = false;
    bool global_dirty = true;

    glm::vec3 pos = { 0.0f, 0.0f, 0.0f };
    glm::vec3 eulerRot = { 0.0f, 0.0f, 0.0f };
    glm::vec3 scale = { 1.0f, 1.0f, 1.0f };

    void setLocalDirty();

    void setGlobalDirty();

public:
    const glm::mat4 & getLocalTransformationMatrix();
    const glm::mat4 & getGlobalTransformationMatrix();


    // local funcs
    void move(glm::vec3 speed_per_coord){
        pos += speed_per_coord;
        std::cout << "position (" << pos.x << "," << pos.y << "," << pos.z << ")" << std::endl; 
        setLocalDirty();
    }

    void rotate(glm::vec3 speed_per_coord){
        eulerRot += speed_per_coord;
        setLocalDirty();
    }

    void setPosition(glm::vec3 newpos){
        pos = newpos;
        setLocalDirty();
    }
    void setScale(glm::vec3 newScale){
        scale = newScale;
        setLocalDirty();
    }
    void setRotation(glm::vec3 newRot){
        eulerRot = newRot;
        setLocalDirty();
    }

    virtual void _onUpdate(float deltaTime) override{
        //pos[0] += 0.1 * deltaTime;
    }

    // global funcs TDOO
    /*
    void moveGlobal(glm::vec3 speed_per_coord){
        pos += speed_per_coord;
        global = true;
    }

    void rotateGlobal(glm::vec3 speed_per_coord){
        eulerRot += speed_per_coord;
        local_dirty = true;
    }

    void setPositionGlobal(glm::vec3 newpos){
        pos = newpos;
        local_dirty = true;
    }
    void setScaleGlobal(glm::vec3 newScale){
        scale = newScale;
        local_dirty = true;
    }
    void setRotationGlobal(glm::vec3 newRot){
        eulerRot = newRot;
        local_dirty = true;
    }*/

    void printGlobal(){
        auto g = getGlobalTransformationMatrix();

        for (int i = 0; i < 4; ++i){
            std::cout << "| " << g[0][i] << ", " << g[1][i] << ", " << g[2][i] << ", " << g[3][i] << " |" << std::endl;
        }
    }
    void printLocal(){
        auto g = getLocalTransformationMatrix();

        for (int i = 0; i < 4; ++i){
            std::cout << "| " << g[0][i] << ", " << g[1][i] << ", " << g[2][i] << ", " << g[3][i] << " |" << std::endl;
        }
    }
};



