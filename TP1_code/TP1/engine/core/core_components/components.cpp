#include "components.h"
#include "TP1/engine/core/gameObject/gameObject.h"

const glm::mat4 & C_Transform::getLocalTransformationMatrix(){

    if (!local_dirty){
        return local;
    }

    // if dirty

    glm::mat4 rotations = glm::rotate(glm::mat4(1.0f),
                            glm::radians(eulerRot.x),
                            glm::vec3(1.0f, 0.0f, 0.0f));
    rotations = glm::rotate(rotations,
                            glm::radians(eulerRot.y),
                            glm::vec3(0.0f, 1.0f, 0.0f));
    rotations = glm::rotate(rotations,
                            glm::radians(eulerRot.z),
                            glm::vec3(0.0f, 0.0f, 1.0f));

    local = glm::translate(glm::mat4(1.0f), pos) *
            rotations *
            glm::scale(glm::mat4(1.0f), scale);
    
    local_dirty = false;

}
const glm::mat4 & C_Transform::getGlobalTransformationMatrix(){

    if (!global_dirty){
        return global;
    }

    // if dirty

    GameObject * parent = getOwner().parent;

    if (parent && parent->hasComponent<C_Transform>()){
        global = parent->getComponent<C_Transform>()->getGlobalTransformationMatrix() * getLocalTransformationMatrix();
    } else { // if our parent has no transform or we're the root, just assume we're the base transform.
        global = getLocalTransformationMatrix();
    }
    
    global_dirty = false;

}


void C_Transform::setLocalDirty(){
    local_dirty = true;

    for (const auto & [ptr, owning_ptr] : getOwner().children){ // not ideal. waiting for a better idea. Maybe an apply_to_children(func f) method?
        GameObject & child = *owning_ptr;
        if (child.hasComponent<C_Transform>()){
            child.getComponent<C_Transform>()->setGlobalDirty();
        }
    }
}

void C_Transform::setGlobalDirty(){
    global_dirty = true;

    for (const auto & [ptr, owning_ptr] : getOwner().children){ // not ideal. waiting for a better idea. Maybe an apply_to_children(func f) method?
        GameObject & child = *owning_ptr;
        if (child.hasComponent<C_Transform>()){
            child.getComponent<C_Transform>()->setGlobalDirty();
        }
    }
}