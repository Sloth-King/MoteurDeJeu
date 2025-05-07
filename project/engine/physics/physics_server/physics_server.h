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
    GameObject* objectA;
    GameObject* objectB;
};

// Largely inspired by this
// https://winter.dev/articles/physics-engine

class PhysicsServer{
private:
    std::set<GameObject*> Objects;
    glm::vec3 gravity = glm::vec3(0.0,0.0,0.0);

public:
    void addObject(GameObject pb);                                  // Add an object to the phys world
    void removeObject(GameObject pb);                               // Remove an object from the phys world
    std::vector<intersectionData> computeCollision();               // Compute all intersections in our world
    void solveCollisions(std::vector<intersectionData> collisions); // Resolve collisions
    void step(float deltaTime);                                     // Per frame calculations

};