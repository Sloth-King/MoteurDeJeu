#include "component.h"
#include "engine/core/gameObject/gameObject.h"
#include <stdio.h>
#include "engine/core/scene/scene.h"
#include "engine/core/game/game.h"

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

void Component::_onEnterScene() {

    std::cout << "entered scene" << std::endl;

    Scene& scene = getScene();
    if (scene.current_game) {
        if(getOwner().hasComponent<C_Collider>()){
            scene.current_game->physics_server.addObject(&getOwner());
        }
    }
    
}
void Component::_onExitScene() {
    Scene& scene = getScene();
    if (scene.current_game) {
        if(getOwner().hasComponent<C_Collider>()){
            scene.current_game->physics_server.removeObject(&getOwner());
        }
    }
}

void Component::_onUpdate(float deltaTime) {};
void Component::_onPhysicsUpdate(float deltaTime) {};