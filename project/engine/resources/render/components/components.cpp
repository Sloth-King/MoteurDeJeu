#include "components.h"
#include "engine/core/gameObject/gameObject.h"
#include "engine/core/scene/scene.h"
#include "engine/core/core_components/components.h"
#include <stdio.h>
void C_Mesh::_onUpdate(float deltaTime) {

    if (getOwner()->hasComponent<C_Transform>()){
        //getOwner()->getComponent<C_Transform>()->printGlobal();
        mesh.render(
            getScene().getCurrentCamera().getVP(), getScene().getCurrentCamera().getForwardVector(),
            getOwner()->getComponent<C_Transform>()->getGlobalTransformationMatrix()
        );
    
    } else {
        mesh.render(getScene().getCurrentCamera().getVP(), getScene().getCurrentCamera().getForwardVector(), glm::mat4(1.0));
    }
};


//TODO : Make sure it works without having to call the prints
void C_Movement::moveWithWASD(float deltaTime){
    GLFWwindow* window = glfwGetCurrentContext();

    if (getOwner()->hasComponent<C_Transform>()){
    
        glm::vec3 movement_dir;

        if (glfwGetKey( window, GLFW_KEY_W ) == GLFW_PRESS){
            movement_dir = glm::vec3(0.0f , 0.0f , -1.0f) * movement_speed;
            getOwner()->getComponent<C_Transform>()->move(movement_dir * deltaTime);

            //printing
            std::cout << "forward" << std::endl;
            std::cout << "Local mat : " << std::endl;
            getOwner()->getComponent<C_Transform>()->printLocal();
            std::cout << "Global mat : " << std::endl;
            getOwner()->getComponent<C_Transform>()->printGlobal();
        }
        // Move backward
        if (glfwGetKey( window, GLFW_KEY_S ) == GLFW_PRESS){
            movement_dir = glm::vec3(0.0f , 0.0f , 1.0f) * movement_speed;
            getOwner()->getComponent<C_Transform>()->move(movement_dir * deltaTime);

            //printing
            std::cout << "backward" << std::endl; 
            std::cout << "Local mat : " << std::endl;
            getOwner()->getComponent<C_Transform>()->printLocal();
            std::cout << "Global mat : " << std::endl;
            getOwner()->getComponent<C_Transform>()->printGlobal();
        }
        // Strafe right
        if (glfwGetKey( window, GLFW_KEY_D ) == GLFW_PRESS){
            movement_dir = glm::vec3(1.0f , 0.0f , 0.0f) * movement_speed;
            getOwner()->getComponent<C_Transform>()->move(movement_dir * deltaTime);

            //printing
            std::cout << "right" << std::endl;
            std::cout << "Local mat : " << std::endl;
            getOwner()->getComponent<C_Transform>()->printLocal();
            std::cout << "Global mat : " << std::endl;
            getOwner()->getComponent<C_Transform>()->printGlobal();
        }
        // Strafe left
        if (glfwGetKey( window, GLFW_KEY_A ) == GLFW_PRESS){
            movement_dir = glm::vec3(-1.0f , 0.0f , 0.0f) * movement_speed;
            getOwner()->getComponent<C_Transform>()->move(movement_dir * deltaTime);

            //printing
            std::cout << "left" << std::endl; 
            std::cout << "Local mat : " << std::endl;
            getOwner()->getComponent<C_Transform>()->printLocal();
            std::cout << "Global mat : " << std::endl;
            getOwner()->getComponent<C_Transform>()->printGlobal();
        }

    
    }
};

void C_Camera::_onUpdate(float deltaTime){

    if (getOwner()->hasComponent<C_Transform>()){
        camera.transform = glm::translate(getOwner()->getComponent<C_Transform>()->getGlobalTransformationMatrix() , offset);
    }
}

void C_Camera::_onEnterScene(){
    getOwner()->getScene()->setCurrentCamera(camera);
};

void C_Movement::_onUpdate(float deltaTime){
    this->moveWithWASD(deltaTime);
};