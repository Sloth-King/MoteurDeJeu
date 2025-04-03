#pragma once


#include "TP1/engine/core/component/component.h"

#include "TP1/engine/render/mesh/mesh.h"

class C_Mesh: public Component {
public:
    
    Mesh mesh;

    //C_Mesh(Mesh & mesh): mesh(mesh) {}
    C_Mesh() = default;
    
    virtual void _onUpdate(float deltaTime) override;
};
