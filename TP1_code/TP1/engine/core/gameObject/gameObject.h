

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



class GameObject{

    GameObject * parent; // itself in case it's root of a scene
    
    std::map< GameObject*, std::unique_ptr<GameObject> > children;

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
    glm::mat4 getGlobalTransform() const { return glm::mat4();}

    bool isRoot() const { return parent == this; };

};

class Object3D: GameObject{
    glm::mat4 transform;
    
    glm::mat4 getGlobalTransform() const {
    }

};