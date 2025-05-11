#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "engine/core/component/component.h"
#include "engine/physics/collider/collider.h"
#include <iostream>



// Physics and collision
class C_RigidBody : public Component{

    public:
    glm::vec3 linear_velocity;  // Velocity for xyz directions
    glm::vec3 angular_velocity; // Velocity for when we handle rotations
    glm::vec3 acceleration;     // Acceleration 
    float damping = 0.99f;      // Damping to simulate drag naively
    float mass = 1.0f;                 // Mass of our object (maybe switch to inverse mass)

    // Constructor 
    C_RigidBody(glm::vec3 initial_velocity = glm::vec3(0.0f) , float m = 1.0f) 
    : linear_velocity(initial_velocity), mass(m) {}

    void setVelocity(glm::vec3 velocity){
        linear_velocity = velocity;
    }
};

// Collision without physics
// TODO
class C_StaticBody : public Component{

};

// Collision shape 
class C_Collider : public Component{
    public:
    Collider collider;
    
};