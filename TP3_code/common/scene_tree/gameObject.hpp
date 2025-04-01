#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "transform.hpp"
#include "mesh.hpp"
#include <memory>

class GameObject{
    public:
        Transform transform;

        std::vector<GameObject*> children = {}; //enfants

        GameObject* parent = nullptr; //parent

        Mesh object_mesh;

        std::string object_name;

        GameObject(Mesh mesh, std::string name, GameObject* parent = nullptr) {
            object_mesh = mesh;
            object_name = name;
            this->parent = parent;
            if (parent) {
                parent->addChild(this);
            }
        }

        void addChild(GameObject* child);
        void updateSelfAndChildren();
        void renderScene(glm::mat4 vpMatrix);
};