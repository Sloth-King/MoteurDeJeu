#pragma once

#include <map>
#include <memory>
#include <typeindex> // for rtti use

class GameObject; // forward declaration
class Scene;

class Component {
    friend GameObject;
protected:
    GameObject* owner;
public:

    virtual ~Component() = default;
    Component(Component && v) = default;
    Component(const Component & v) = default;

    GameObject& getOwner();

    Scene& getScene();
    
    Component() = default;

    virtual void _onUpdate(float deltaTime);
    virtual void _onPhysicsUpdate(float deltaTime);

};