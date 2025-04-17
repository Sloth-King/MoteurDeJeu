#pragma once


#include "engine/core/component/component.h"

#include "engine/render/mesh/mesh.h"
#include "engine/render/camera/camera.h"

class C_Mesh: public Component {
public:
    
    Mesh mesh;

    //C_Mesh(Mesh & mesh): mesh(mesh) {}
    C_Mesh() = default;
    
    virtual void _onUpdate(float deltaTime) override;

};

class C_Camera: public Component{
public:
    Camera camera;
    glm::vec3 offset = glm::vec3(0.0,0.0,0.0);

    C_Camera() = default;

    virtual void _onUpdate(float deltaTime) override;   

    virtual void _onEnterScene() override;
};


//TODO : Make a seperate class to simplify the code in components
class C_Movement: public Component {
    protected:
        float movement_speed;
    public:
        inline void moveWithWASD(float deltaTime);

        C_Movement(){
            movement_speed = 0.5f;
        }

        inline void setMoveSpeed(float movespeed){
            movement_speed = movespeed;
        }

        virtual void _onUpdate(float deltaTime) override;
};
