#include "gameObject.hpp"
#include "transform.hpp"
#include "mesh.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Random useful links :
// The whole logic tbh : https://learnopengl.com/Guest-Articles/2021/Scene/Scene-Graph
// emplace_back : https://stackoverflow.com/questions/4303513/push-back-vs-emplace-back

void GameObject::addChild(GameObject *child)
{
    children.emplace_back(child);   // APPARENTLY this doesn't make temporary copies unlike push_back
    children.back()->parent = this; // Access the newly added child's parent, and set it as this GO
}

void GameObject::updateSelfAndChildren()
{
    if (parent)
    {
        transform.transformationMatrix = parent->transform.transformationMatrix * transform.getLocalModelMatrix();
    }
    else
    {
        transform.transformationMatrix = transform.getLocalModelMatrix();
    }

    for (auto &&child : children)
    {
        child->updateSelfAndChildren();
    }
}

void GameObject::renderScene(glm::mat4 vpMatrix)
{
    // Render the current object's mesh if it exists
    glm::mat4 modelMatrix;
    if (parent)
    {
        modelMatrix = (parent->transform.transformationMatrix) * transform.getLocalModelMatrix();
    }
    else
    {
        modelMatrix = transform.getLocalModelMatrix();
    }
    object_mesh.render(vpMatrix, modelMatrix);

    // Recursively render all child objects
    for (auto &&child : children)
    {
        child->renderScene(vpMatrix);
    }
}

// Add after your existing triangle methods

float GameObject::getHeightAtPosition(glm::vec3 position, GameObject& terrain)
{
    Transform tempTransform;
    tempTransform.setPosition(position);
    
    Mesh pointMesh;
    pointMesh.vertices.push_back(position);
    pointMesh.triangles.push_back(Triangle(0, 0, 0));
    
    GameObject pointObject(pointMesh, "temp_point");
    pointObject.transform = tempTransform;
    
    Triangle closestTriangle = pointObject.detectClosestTriangle(terrain);
    
    glm::vec3 v0 = terrain.object_mesh.vertices[closestTriangle.v[0]];
    glm::vec3 v1 = terrain.object_mesh.vertices[closestTriangle.v[1]];
    glm::vec3 v2 = terrain.object_mesh.vertices[closestTriangle.v[2]];
    
    glm::vec3 normal = glm::normalize(glm::cross(v1 - v0, v2 - v0));
    
    if (normal.y > 0.7f) {
        return (v0.y + v1.y + v2.y) / 3.0f;
    } else {
        float d = -glm::dot(normal, v0);
        
        if (std::abs(normal.y) < 0.01f) {
            return (v0.y + v1.y + v2.y) / 3.0f;
        }
    
        return (-normal.x * position.x - normal.z * position.z - d) / normal.y;
    }
}

// Maybe pass this from Triangle return type to int to return an index actually
Triangle GameObject::detectClosestTriangle(GameObject collisionObject)
{
    Mesh collisionMesh = collisionObject.object_mesh;

    Triangle closestTriangle;
    float minDistance = FLT_MAX;

    // FIXME : Definitely inoptimal for real time
    // Iterate over all the triangles of our object and the collision object to find the min distance and the proper triangle we're on or something
    for (const Triangle &ourT : this->object_mesh.triangles)
    {
        for (const Triangle &theirT : collisionMesh.triangles)
        {
            float distance = calculateTriangleDistance(ourT, this->object_mesh , theirT , collisionMesh);

            if (distance < minDistance)
            {
                minDistance = distance;
                closestTriangle = theirT;
            }
        }
    }
    return closestTriangle;
}

// Mayve move this later to triangle or mesh idk
float GameObject::calculateTriangleDistance(const Triangle &t1, const Mesh &mesh1, const Triangle &t2, const Mesh &mesh2)
{
    float minDist = std::numeric_limits<float>::max();

    // Compare each vertex of triangle 1 with each vertex of triangle 2
    for (int i = 0; i < 3; i++)
    {
        // Get actual vertex position from the mesh using the index stored in the triangle
        const glm::vec3 &vert1 = mesh1.vertices[t1.v[i]];
        
        for (int j = 0; j < 3; j++)
        {
            // Get actual vertex position from the second mesh
            const glm::vec3 &vert2 = mesh2.vertices[t2.v[j]];
            
            // Calculate distance between actual vertex positions
            float dist = glm::length(vert1 - vert2);
            minDist = std::min(minDist, dist);
        }
    }
    return minDist;
}