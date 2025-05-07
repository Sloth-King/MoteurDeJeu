

#pragma once
// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <map>
#include <iostream>
#include <memory>
#include <typeindex>
#include <cassert>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "engine/utils/utils.h"

#include "engine/core/component/component.h"


template<typename T> // https://stackoverflow.com/questions/65969224/c-templates-and-inheritance-can-templates-be-more-selective
concept DerivedFromComponent = std::is_base_of_v<Component,T>;

class Scene;
class GameObject{
    friend Scene;

    static unsigned long next_id;

    unsigned long id = 0;

public:
    GameObject * parent = nullptr; // null in case it's root of a scene
    
    std::map< unsigned long, std::unique_ptr<GameObject> > children = std::map< unsigned long, std::unique_ptr<GameObject> >();

    std::map<std::type_index, std::unique_ptr<Component> > components = std::map<std::type_index, std::unique_ptr<Component> >();

    Scene * scene = nullptr;

    GameObject( GameObject&& rcOther )
    : children( std::move(rcOther.children) )
    , components( std::move(rcOther.components) )
    , scene(rcOther.scene)
    , id(rcOther.id)
    {
        for (const auto & [id, owning_ptr] : children)
            owning_ptr->parent = this;
        
        for (const auto & [ti, comp] : components)
            comp->owner = this;
    }

    bool hidden = false;
    // bool deactivated = false;

    GameObject(): id(next_id++) {};

    GameObject* addChild(GameObject && child);

    bool isRoot() const { return (bool)parent; };

    Scene * getScene() const {
        assert(scene != nullptr);
        return scene;
    }

    void __enterScene(Scene * s){
        scene = s;
        for (const auto & [id, owning_ptr] : children)
            owning_ptr->__enterScene(s);

        for (const auto & [ti, comp] : components)
            comp->_onEnterScene();

    }

    void __exitScene(){
        for (const auto & [id, owning_ptr] : children)
            owning_ptr->__exitScene();

        for (const auto & [ti, comp] : components)
            comp->_onExitScene();
        
        scene = nullptr;

    }


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
    T* addComponent(){
        components[std::type_index(typeid(T))] = std::make_unique<T>();
        components[std::type_index(typeid(T))]->owner = this;
        return dynamic_cast<T*>(components[std::type_index(typeid(T))].get());
    }

    void __engineUpdate(float deltaTime){
        
        for (const auto & [id, owning_ptr] : children)
            owning_ptr->__engineUpdate(deltaTime);
        
        for (const auto & [ti, comp] : components)
            comp->_onUpdate(deltaTime);

    }

    void __enginePhysicsUpdate(float deltaTime){

        for (const auto & [id, owning_ptr] : children)
            owning_ptr->__enginePhysicsUpdate(deltaTime);

        for (const auto & [ti, comp] : components)
            comp->_onPhysicsUpdate(deltaTime);
    }


};