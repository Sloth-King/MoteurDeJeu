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

struct Collider{
    ColliderType type;
};
 
// FIXME : We'll see later lmaoo
struct SphereCollider : public Collider{
    ColliderType type = SPHERE;
	glm::vec3 center;
	float radius;
};

struct PlaneCollider : public Collider{
    ColliderType type = PLANE;
    glm::vec3 normal;
    float distance;
};
