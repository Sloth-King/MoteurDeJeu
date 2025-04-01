#include "transform.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>


//https://learnopengl.com/Guest-Articles/2021/Scene/Scene-Graph
glm::mat4 Transform::getLocalModelMatrix()
{
    {
        const glm::mat4 transformX = glm::rotate(glm::mat4(1.0f),
                                                 glm::radians(eulerRot.x),
                                                 glm::vec3(1.0f, 0.0f, 0.0f));
        const glm::mat4 transformY = glm::rotate(glm::mat4(1.0f),
                                                 glm::radians(eulerRot.y),
                                                 glm::vec3(0.0f, 1.0f, 0.0f));
        const glm::mat4 transformZ = glm::rotate(glm::mat4(1.0f),
                                                 glm::radians(eulerRot.z),
                                                 glm::vec3(0.0f, 0.0f, 1.0f));

        // Y * X * Z
        const glm::mat4 roationMatrix = transformY * transformX * transformZ;

        // translation * rotation * scale (also know as TRS matrix)
        return glm::translate(glm::mat4(1.0f), pos) *
               roationMatrix *
               glm::scale(glm::mat4(1.0f), scale);
    }
}

void Transform::move(glm::vec3 speed_per_coord){
    pos += speed_per_coord;
}

void Transform::rotate(glm::vec3 speed_per_coord){
    eulerRot += speed_per_coord;
}
