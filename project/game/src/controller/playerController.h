#pragma once
#include "engine/includes/core.h"




class C_PlayerController: public Component {
public:

    float movement_speed = 5.0;


    void inputCallback(const InputEvent & e){
        /*
        if (e == "move_right"){
            std::cout << "move_right" << std::endl;
        }
        else if (e == "move_left"){
            std::cout << "move_left" << std::endl;
        }
        else if (e == "move_forward"){
            std::cout << "move_forward" << std::endl;
        }
        else if (e == "move_backwards"){
            std::cout << "move_backwards" << std::endl;
        }
        else if (e == "move_up"){
            std::cout << "move_up" << std::endl;
        }
        else if (e == "move_down"){
            std::cout << "move_down" << std::endl;
        }
        */
    }

    C_PlayerController(){
        Input::addInputListener(
            [& /*capture ambient object ref*/](const InputEvent & e){
                inputCallback(e);
            }
        );
    }

    virtual void _onUpdate(float delta) override{

        delta *= movement_speed;

        if (!getOwner()->hasComponent<C_Transform>()) return;

        auto* transform = getOwner()->getComponent<C_Transform>();

        if (Input::isInputPressed("move_right")){
            transform->move(glm::vec3(1, 0, 0) * delta);
        }
        if (Input::isInputPressed("move_left")){
            transform->move(glm::vec3(-1, 0, 0) * delta);
        }
        if (Input::isInputPressed("move_forward")){
            transform->move(glm::vec3(0, 0, -1) * delta);
        }
        if (Input::isInputPressed("move_backwards")){
            transform->move(glm::vec3(0, 0, 1) * delta);
        }
        if (Input::isInputPressed("move_up")){
            transform->move(glm::vec3(0, 1, 0) * delta);
        }
        if (Input::isInputPressed("move_down")){
            transform->move(glm::vec3(0, -1, 0) * delta);
        }
    }
};