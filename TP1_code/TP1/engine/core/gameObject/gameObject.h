

#pragma once
// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <map>
#include <iostream>
#include <memory>
#include <typeindex>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "TP1/engine/utils/utils.h"

#include "TP1/engine/core/component/component.h"


template<typename T> // https://stackoverflow.com/questions/65969224/c-templates-and-inheritance-can-templates-be-more-selective
concept DerivedFromComponent = std::is_base_of_v<Component,T>;

class Scene;
class GameObject{
    friend Scene;

public:
    GameObject * parent; // null in case it's root of a scene
    
    std::map< GameObject*, std::unique_ptr<GameObject> > children;

    std::map<std::type_index, std::unique_ptr<Component> > components;

    Scene * scene;
// public

    bool hidden = false;
    // bool deactivated = false;

    GameObject() = default;

    void setParent(GameObject & new_parent);

    // we assume that the transform of such a node is identity

    bool isRoot() const { return (bool)parent ; };

    Scene & getScene() const {return *scene;};


    template <DerivedFromComponent T>
    bool hasComponent() const {
        return components.contains(std::type_index(typeid(T)));
    }

    template <DerivedFromComponent T>
    T* getComponent(){
        return dynamic_cast<T*>(components[std::type_index(typeid(T))].get());
    }
    /*
    void addComponent(Component && c){
        c.owner = this;
        components[std::type_index(typeid(c))] = std::make_unique<Component>(std::move(c));
    }*/

    // copies the component instead
    template <DerivedFromComponent T>
    void addComponent(){
        components[std::type_index(typeid(T))] = std::make_unique<T>();
        components[std::type_index(typeid(T))]->owner = this;
    }

    void __engineUpdate(float deltaTime){

        for (const auto & [ptr, owning_ptr] : children)
            owning_ptr->__engineUpdate(deltaTime);
        
        for (const auto & [ti, comp] : components)
            comp->_onUpdate(deltaTime);
        
    }

    void __enginePhysicsUpdate(float deltaTime){

        for (const auto & [ptr, owning_ptr] : children)
            owning_ptr->__enginePhysicsUpdate(deltaTime);

        for (const auto & [ti, comp] : components)
            comp->_onPhysicsUpdate(deltaTime);

    }


};