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
        //note to self : unique ptr is to make it so this object "owns" the child objects
        std::vector<std::unique_ptr<GameObject>> children; //enfants

        //nullptr i guess not to break the toings idk
        GameObject* parent = nullptr; //parent
        //mesh
        Mesh object_mesh;

        GameObject(Mesh mesh);

        //TODO
        void add_child();
};