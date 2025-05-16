#pragma once
#include <set>
#include "engine/render/components/components.h"
class RenderingServer{
friend class Game;
    std::set<C_Mesh * > objects;

    void renderAll(){
        for (auto object: objects){
            object->render();
        }
    }

public:

    void addObject(C_Mesh * g ) noexcept {
        objects.insert(g);
    }

    void removeObject(C_Mesh * g )noexcept {
        objects.erase(g);
    }


};