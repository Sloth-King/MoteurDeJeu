#pragma once //ifndef is my opp

#include <glm/glm.hpp>


//Source : https://learnopengl.com/Guest-Articles/2021/Scene/Scene-Graph
//Im using this tutorial yippee
class Transform
{
    private:
        /*SPACE INFORMATION*/
        //Local space information
        glm::vec3 pos = { 0.0f, 0.0f, 0.0f };
        glm::vec3 eulerRot = { 0.0f, 0.0f, 0.0f };
        glm::vec3 scale = { 1.0f, 1.0f, 1.0f };

        //Global space information concatenate in matrix
        //TODO : Implement it maybe 


    public:
        glm::mat4 transformationMatrix = glm::mat4(1.0f);
        glm::mat4 getLocalModelMatrix();
        void move(glm::vec3 speed_per_coord); //to call each frame
        void rotate(glm::vec3 speed_per_coord);

        void setPosition(glm::vec3 newpos){
            pos = newpos;
        }
        void setScale(glm::vec3 newScale){
            scale = newScale;
        }
        void setRotation(glm::vec3 newRot){
            eulerRot = newRot;
        }

};
    