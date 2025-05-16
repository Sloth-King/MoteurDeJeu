#include "components.h"
#include "engine/core/gameObject/gameObject.h"
#include "engine/core/game/game.h"
#include "engine/core/scene/scene.h"
#include "engine/core/core_components/components.h"
#include <stdio.h>
void C_Mesh::render() {

    if (getOwner()->hasComponent<C_Transform>()){
        //getOwner()->getComponent<C_Transform>()->printGlobal();
        mesh.render(
            getScene().getCurrentCamera().getVP(), getScene().getCurrentCamera().getForwardVector(),
            getOwner()->getComponent<C_Transform>()->getGlobalTransformationMatrix()
        );
    
    } else {
        mesh.render(getScene().getCurrentCamera().getVP(), getScene().getCurrentCamera().getForwardVector(), glm::mat4(1.0));
    }
};


void C_Mesh::_onEnterScene(){
    Scene& scene = getScene();
    if(scene.current_game){
        scene.current_game->renderingServer.addObject(this);
    }
};

void C_Mesh::_onExitScene(){
    Scene& scene = getScene();
    if(scene.current_game){
        scene.current_game->renderingServer.removeObject(this);
    }
};


void C_Camera::_onLateUpdate(float deltaTime){

    if (getOwner()->hasComponent<C_Transform>()){
        camera.transform = glm::translate(getOwner()->getComponent<C_Transform>()->getGlobalTransformationMatrix() , offset);
    }
}

void C_Camera::_onEnterScene(){
    getOwner()->getScene()->setCurrentCamera(camera);
};

