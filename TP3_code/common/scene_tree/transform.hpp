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
        glm::mat4 transformationMatrix = glm::mat4(1.0f);


    public:
        //TODO actually code these eventually
        //Source : Cours
        glm::vec3 apply(glm::vec4 p); //affine coords
        glm::vec3 applyToPoint(glm::vec3 p); //point in cartesian coords
        glm::vec3 applyToVector(glm::vec3 v); //in Cartesian coords    
        glm::vec3 applyToVersor(glm::vec3 v); //versor is a normalized vector - also in cartesian coords.

};
    