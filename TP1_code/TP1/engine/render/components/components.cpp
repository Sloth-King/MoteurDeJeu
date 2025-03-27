#include "components.h"
#include "TP1/engine/core/gameObject/gameObject.h"
#include "TP1/engine/core/scene/scene.h"
#include "TP1/engine/core/core_components/components.h"
#include <stdio.h>
void C_Mesh::_onUpdate(float deltaTime) {
    printf("hellooo\n");
    if (getOwner().hasComponent<C_Transform>()){

        mesh.render(
            getScene().getCurrentCamera().getVP(),
            getOwner().getComponent<C_Transform>()->getGlobalTransformationMatrix()
        );

    } else {
        mesh.render(getScene().getCurrentCamera().getVP(), glm::mat4(1.0));
    }
};

