#include "components.h"
#include "engine/core/gameObject/gameObject.h"

const glm::mat4 & C_Transform::getLocalTransformationMatrix() const{

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

    return local;

}   
const glm::mat4 & C_Transform::getGlobalTransformationMatrix() const{


    if (!global_dirty && !local_dirty){
        return global;
    }


    // if dirty

    GameObjectData * parent = getOwner()->parent;

    if (parent && parent->hasComponent<C_Transform>()){
        
        global = parent->getComponent<C_Transform>()->getGlobalTransformationMatrix() * getLocalTransformationMatrix();
    } else { // if our parent has no transform or we're the root, just assume we're the base transform.
        global = getLocalTransformationMatrix();
    }

    global_inv = glm::inverse(global);
    
    global_dirty = false;

    return global;

}

const glm::mat4 & C_Transform::getGlobalInverse() const{
    getGlobalTransformationMatrix();
    return global_inv;
}

const glm::vec3 & C_Transform::getGlobalPosition() const{
    static glm::vec3 globalPosition;
    glm::mat4 globalMatrix = getGlobalTransformationMatrix();
    globalPosition = glm::vec3(globalMatrix[3][0], globalMatrix[3][1], globalMatrix[3][2]);
    return globalPosition;
}

const glm::vec3 & C_Transform::getGlobalScale() const{
    static glm::vec3 globalScale;
    glm::mat4 globalMatrix = getGlobalTransformationMatrix();
    globalScale = glm::vec3(
        glm::length(glm::vec3(globalMatrix[0])),
        glm::length(glm::vec3(globalMatrix[1])),
        glm::length(glm::vec3(globalMatrix[2]))
    );
    return globalScale;
}

//TODO
const glm::vec3 & C_Transform::getGlobalRotation() const{
    return glm::vec3(1.0f,1.0f,1.0f);
}


void C_Transform::setLocalDirty(){
    local_dirty = true;
    global_dirty = true;
    for (const auto & [ptr, object] : getOwner()->children){ // not ideal. waiting for a better idea. Maybe an apply_to_children(func f) method?
        GameObjectData & child = *object;
        if (child.hasComponent<C_Transform>()){
            child.getComponent<C_Transform>()->setGlobalDirty();
        }
    }
}

void C_Transform::setGlobalDirty(){
    global_dirty = true;

    for (const auto & [ptr, object] : getOwner()->children){ // not ideal. waiting for a better idea. Maybe an apply_to_children(func f) method?
        GameObjectData & child = *object;
        if (child.hasComponent<C_Transform>()){
            child.getComponent<C_Transform>()->setGlobalDirty();
        }
    }
}