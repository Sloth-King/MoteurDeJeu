#include "components.h"
#include "engine/core/gameObject/gameObject.h"
#include "engine/core/scene/scene.h"
#include "engine/core/core_components/components.h"
#include <stdio.h>
void C_Mesh::_onUpdate(float deltaTime) {

    
    if (getOwner().hasComponent<C_Transform>()){
        mesh.render(
            getScene().getCurrentCamera().getVP(),
            getOwner().getComponent<C_Transform>()->getGlobalTransformationMatrix()
        );
    
    } else {
        mesh.render(getScene().getCurrentCamera().getVP(), glm::mat4(1.0));
    }
};

