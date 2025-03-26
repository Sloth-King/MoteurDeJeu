

#pragma once
// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <map>
#include <iostream>
#include <memory>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "TP1/engine/utils/utils.h"
#include "TP1/engine/core/scene/scene.h"


class GameObject{

    GameObject * parent; // itself in case it's root of a scene
    
    std::map< GameObject*, std::unique_ptr<GameObject> > children;

    Scene * scene;

    bool hidden = false;
    // bool deactivated = false;

    glm::mat4 transform = glm::mat4(1.0);
    Mesh mesh;

    bool isInScene(){
        GameObject * ancestor = parent;

        while (ancestor != nullptr && ancestor != ancestor->parent){
            ancestor = ancestor->parent;
        }
        return (ancestor != nullptr);
    }

    void setParent(GameObject & new_parent);

    // we assume that the transform of such a node is identity

    bool isRoot() const { return parent == this; };

    

    void __engineUpdate(float deltaTime){

        for (const auto & [ptr, owning_ptr] : children)
            owning_ptr->__engineUpdate();
        
        _update();

    }

    void __enginePhysicsUpdate(float deltaTime){

        for (const auto & [ptr, owning_ptr] : children)
            owning_ptr->__enginePhysicsUpdate();
        
        _physicsUpdate();
    }

    virtual void  _update(float deltaTime) {};
    virtual void  _physicsUpdate(float deltaTime) {};

};