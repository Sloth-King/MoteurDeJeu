

#include <vector>
#include <map>
#include "gameObject.h"


void GameObject::setParent(GameObject & new_parent){
    
    std::unique_ptr<GameObject> ptr;

    if (parent != nullptr){
        ptr = std::move(parent->children[this]);
        parent->children.erase(this);
    } else {
        ptr = std::unique_ptr<GameObject>(this);
    }
    parent = &new_parent;
    scene = new_parent.scene;

    new_parent.children[this] = std::move(ptr);
}
