#pragma once

#include <set>
#include <glm/vec3.hpp>
#include "engine/physics/components/components.h"
#include "engine/core/gameObject/gameObject.h"

struct intersectionData{
    bool isIntersection;
    glm::vec3 intersectionPointA;
    glm::vec3 intersectionPointB;
    glm::vec3 intersectionNormal;
    GameObjectData* objectA;
    GameObjectData* objectB;
};

// Largely inspired by this
// https://winter.dev/articles/physics-engine

class PhysicsServer{
private:
    std::set<GameObjectData*> Objects;
    glm::vec3 gravity = glm::vec3(0.0,9.8,0.0);

public:
    void addObject(GameObjectData* go);                                  // Add an object to the phys world
    void removeObject(GameObjectData* go);                               // Remove an object from the phys world
    std::vector<intersectionData> computeCollisions();              // Compute all collisions in our world
    void solveCollisions(std::vector<intersectionData> collisions); // Resolve collisions
    void applyPhysics(float deltatime);                             // Apply physics calculations to the object
    void step(float deltaTime);                                     // Per frame calculations

};