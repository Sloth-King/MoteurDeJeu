#include "engine/physics/components/components.h"
#include "engine/core/game/game.h"
#include "engine/core/scene/scene.h"

void C_Collider::_onEnterScene(){
    Scene& scene = getScene();
    if(scene.current_game){
        scene.current_game->physics_server.addObject(getOwner());
    }
};

void C_Collider::_onExitScene(){
    Scene& scene = getScene();
    if(scene.current_game){
        scene.current_game->physics_server.removeObject(getOwner());
    }
};

void C_RigidBody::_onEnterScene(){
    Scene& scene = getScene();
    if(scene.current_game){
        scene.current_game->physics_server.addObject(getOwner());
    }
};

void C_RigidBody::_onExitScene(){
    Scene& scene = getScene();
    if(scene.current_game){
        scene.current_game->physics_server.removeObject(getOwner());
    }
};

float C_RigidBody::inverseMass(){
    if(isStatic) return 0.0f;
    return 1.f/mass;
};