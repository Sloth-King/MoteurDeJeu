#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>


// Check out : https://winter.dev/articles/physics-engine for the major inspiration of this

// All the collision types we have
enum ColliderType{
    BASE, // FIXME : idk man supposed to be a placeholder
    SPHERE,
    PLANE
};

struct Collider {
    ColliderType type;
    Collider() : type(BASE) {}
    Collider(ColliderType type) : type(type) {
        std::cout << "regular ass " << type << std::endl;
    }
};

struct SphereCollider : public Collider {
    glm::vec3 center;
    float radius;

    SphereCollider(const glm::vec3& center, float radius)
        : Collider(SPHERE), center(center), radius(radius) {
            std::cout << "sphere type" << ((Collider*)this)->type << std::endl;
        }
};

struct PlaneCollider : public Collider {
    glm::vec3 normal;
    float distance;

    PlaneCollider(const glm::vec3& normal, float distance)
        : Collider(PLANE), normal(normal), distance(distance) {
            std::cout << "plane type" << ((Collider*)this)->type << std::endl;

        }
};
