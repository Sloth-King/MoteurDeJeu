

#include <vector>
#include <map>
#include "gameObject.h"

unsigned long  GameObject::next_id = 0;

void GameObject::addChild(GameObject && child){


    auto p = child.id;
    std::unique_ptr<GameObject> ptr = nullptr;

    if (child.parent != nullptr){
        ptr = std::move(child.parent->children[child.id]);
        child.parent->children.erase(p);
    } else {
        ptr = std::make_unique<GameObject>(std::move(child));

    }
    children[p] = std::move(ptr);
    children[p]->parent = this;
    if (scene) children[p]->__enterScene(scene);
}

