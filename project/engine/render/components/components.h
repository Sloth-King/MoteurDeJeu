#pragma once


#include "engine/core/component/component.h"

#include "engine/render/mesh/mesh.h"

class C_Mesh: public Component {
public:
    
    Mesh mesh;

    //C_Mesh(Mesh & mesh): mesh(mesh) {}
    C_Mesh() = default;
    
    virtual void _onUpdate(float deltaTime) override;

};
