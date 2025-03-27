#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "TP1/engine/core/component/component.h"

class C_Transform: public Component {

private:
    glm::mat4 local = glm::mat4(1.0f);
    glm::mat4 global = glm::mat4(1.0f);

    bool local_dirty = false;
    bool global_dirty = false;

    glm::vec3 pos = { 0.0f, 0.0f, 0.0f };
    glm::vec3 eulerRot = { 0.0f, 0.0f, 0.0f };
    glm::vec3 scale = { 1.0f, 1.0f, 1.0f };

public:

    const glm::mat4 & getLocalTransformationMatrix();
    const glm::mat4 & getGlobalTransformationMatrix();

    void setLocalDirty();

    void setGlobalDirty();

    // local funcs
    void move(glm::vec3 speed_per_coord){
        pos += speed_per_coord;
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
};



