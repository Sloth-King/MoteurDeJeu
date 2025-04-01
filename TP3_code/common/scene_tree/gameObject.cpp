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
