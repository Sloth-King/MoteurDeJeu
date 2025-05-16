#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <iostream>

// Check out : https://winter.dev/articles/physics-engine for the major inspiration of this

// All the collision types we have
enum ColliderType
{
    BASE, // FIXME : idk man supposed to be a placeholder
    SPHERE,
    PLANE,
    CUBE,
    // TODO : figure out a structure for the map (maybe seperate from cube?)
    RAY
};

struct Collider
{
    ColliderType type;
    Collider() : type(BASE) {}
    Collider(ColliderType type) : type(type) {}
};

struct SphereCollider : public Collider
{
    glm::vec3 center;
    float radius;

    SphereCollider(const glm::vec3 &center, float radius)
        : Collider(SPHERE), center(center), radius(radius) {}
};

struct PlaneCollider : public Collider
{
    glm::vec3 normal;
    float distance;

    PlaneCollider(const glm::vec3 &normal, float distance)
        : Collider(PLANE), normal(normal), distance(distance) {}
};

// IMPORTANT : These are AABBs so it assumes we are axis alligned
// We decided, worst case to allign all cubes to our chunk's axis regardless.
struct CubeCollider : public Collider
{
    glm::vec3 center;
    glm::vec3 halfSize;

    CubeCollider(const glm::vec3 &center, float halfSize)
        : Collider(CUBE), center(center), halfSize(halfSize) {}

    glm::vec3 min() const { return center - halfSize; }
    glm::vec3 max() const { return center + halfSize; }

    // So far just used for plane cube
    std::vector<glm::vec3> getVertices() const
    {
        return {
            center + glm::vec3(-halfSize.x, -halfSize.y, -halfSize.z),
            center + glm::vec3(halfSize.x, -halfSize.y, -halfSize.z),
            center + glm::vec3(-halfSize.x, halfSize.y, -halfSize.z),
            center + glm::vec3(halfSize.x, halfSize.y, -halfSize.z),
            center + glm::vec3(-halfSize.x, -halfSize.y, halfSize.z),
            center + glm::vec3(halfSize.x, -halfSize.y, halfSize.z),
            center + glm::vec3(-halfSize.x, halfSize.y, halfSize.z),
            center + glm::vec3(halfSize.x, halfSize.y, halfSize.z)};
    }
};
