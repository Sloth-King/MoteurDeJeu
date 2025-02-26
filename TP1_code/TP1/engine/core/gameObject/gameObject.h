

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <set>
#include <iostream>
#include <memory>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "TP1/engine/utils/utils.h"
#include "TP1/engine/render/camera/camera.h"

class GameObject{

    GameObject * parent; // itself in case it's root of a scene
    
    std::set< std::unique_ptr<GameObject> > children;

    bool hidden = false;
    // bool deactivated = false;

    bool isInScene(){
        GameObject * ancestor = parent;

        while (ancestor != nullptr && ancestor != ancestor->parent){
            ancestor = ancestor->parent;
        }
        return (ancestor != nullptr);
    }

    void setParent(GameObject & new_parent);



    // we assume that the transform of such a node is identity
    void getGlobalTransform() const { return glm::mat4();}

    bool isRoot() const { return parent == this; };

}

class Object3D: GameObject{
    glm::mat4 transform;


    
    void getGlobalTransform() const {

    }

}