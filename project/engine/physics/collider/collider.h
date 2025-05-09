#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>


// Check out : https://winter.dev/articles/physics-engine for the major inspiration of this

// All the collision types we have
enum ColliderType{
    SPHERE,
    PLANE
};

struct Collider {
    ColliderType type;
    //Collider(ColliderType type) : type(type) {}
};

// FIXME : We'll see later lmaoo
struct SphereCollider : public Collider {
    glm::vec3 center;
    float radius;

    SphereCollider(const glm::vec3& center, float radius)
        : Collider(SPHERE), center(center), radius(radius) {}
};

struct PlaneCollider : public Collider {
    glm::vec3 normal;
    float distance;

    PlaneCollider(const glm::vec3& normal, float distance)
        : Collider(PLANE), normal(normal), distance(distance) {}
};
