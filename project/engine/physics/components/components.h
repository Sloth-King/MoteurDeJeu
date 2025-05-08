#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "engine/core/component/component.h"
#include "engine/physics/collider/collider.h"
#include <iostream>



// Physics and collision
class C_RigidBody : public Component{

    // FIXME : unsure if these need to be stored here or elsewhere
public:
    glm::vec3 linear_velocity; 
    glm::vec3 angular_velocity;
    float mass; 
};

// Collision without physics
// TODO
class C_StaticBody : public Component{

};

// Collision shape
// TODO 
class C_Collider : public Component{
    public:
    Collider collider;
    
};