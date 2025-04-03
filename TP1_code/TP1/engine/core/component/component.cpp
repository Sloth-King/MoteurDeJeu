#include "component.h"
#include "TP1/engine/core/gameObject/gameObject.h"
#include <stdio.h>
GameObject& Component::getOwner(){
    if (owner){
        return *owner;
    }
}

Scene& Component::getScene(){
    if (owner){
        return *owner->getScene();
    }
}

void Component::_onUpdate(float deltaTime) {};
void Component::_onPhysicsUpdate(float deltaTime) {};