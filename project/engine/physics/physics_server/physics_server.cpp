#include "engine/physics/physics_server/physics_server.h"
#include "engine/core/core_components/components.h"

// Sphere Vs Sphere intersection
intersectionData spheresIntersection(const Collider *a, const Collider *b){
    const SphereCollider* sphere_a = static_cast<const SphereCollider*>(a);
    const SphereCollider* sphere_b = static_cast<const SphereCollider*>(b);

    intersectionData res;

    glm::vec3 aCenter = sphere_a->center;
    glm::vec3 bCenter= sphere_b->center;

    float aRadius = sphere_a->radius;
    float bRadius = sphere_b->radius; 

    glm::vec3 ab = bCenter - aCenter;

    float distance = glm::length(ab);

    if (distance < 0.0001f || distance > aRadius + bRadius){
        res.isIntersection = false;
        return res;
    }

    glm::vec3 normal = glm::normalize(ab);
    glm::vec3 intersectionA = aCenter + normal * aRadius;
    glm::vec3 intersectionB = bCenter - normal * bRadius;

    res.isIntersection = true;
    res.intersectionNormal = normal;
    res.intersectionPointA = intersectionA;
    res.intersectionPointB = intersectionB;

    return res;
}   


std::vector<intersectionData> PhysicsServer::computeCollision(){
    std::vector<intersectionData> intersectionList;

    // Check this out for this
    // https://winter.dev/articles/physics-engine

    using FindContactFunc = intersectionData(*)(const Collider*, const Collider*);   
    
    static const FindContactFunc functionTables[1][1] = {
            {spheresIntersection}
    };

    for(const auto& objectA : Objects){

        if(!objectA->hasComponent<C_Collider>() || !objectA->hasComponent<C_RigidBody>()) continue;

        for(const auto& objectB : Objects){
            intersectionData intersection;

            if(!objectB->hasComponent<C_Collider>() || !objectB->hasComponent<C_RigidBody>()) continue;
            
            Collider* colliderA = &objectA->getComponent<C_Collider>()->collider;
            Collider* colliderB = &objectB->getComponent<C_Collider>()->collider;

            intersection = functionTables[colliderA->type][colliderB->type](colliderA, colliderB);
            intersection.objectA = objectA;
            intersection.objectB = objectB;

            intersectionList.push_back(intersection);
        }
    }    
    return intersectionList;
}

void PhysicsServer::solveCollisions(std::vector<intersectionData> collisions){
    for(const auto& collision : collisions){
        glm::vec3 moveVector = collision.intersectionPointB - collision.intersectionPointA;
        collision.objectA->getComponent<C_Transform>()->move(moveVector);
    }
}
