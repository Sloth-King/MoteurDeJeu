#pragma once
// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera{

protected:
    glm::mat4 projection;
    
public:
    glm::mat4 transform; // !! is actually the inverse view matrix

    float width = 500;
    float height = 500;

    Camera(){
        transform = glm::mat4(1.0f);

        projection = glm::perspective(
            glm::radians(45.0f),
            width / height,
            0.1f,
            100.0f
        );
    }

    inline glm::mat4 getProjectionMatrix(){
        return projection;
    }

    inline glm::mat4 getViewMatrix(){
        return transform; // TODO return global transform once we have created the scene class
    }

    inline glm::mat4 getVP(){
        //view = glm::inverse(getViewMatrix());
        return getProjectionMatrix() * getViewMatrix();
    }

    float mouseSpeed = 2.0;
    float speed = 1.0;

    void computeMatricesFromInputs(){

        // glfwGetTime is called only once, the first time this function is called
        static double lastTime = glfwGetTime();

        // Compute time difference between current and last frame
        double currentTime = glfwGetTime();
        float deltaTime = float(currentTime - lastTime);

        // Get mouse position
        static double xpos_last, ypos_last;
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        // Reset mouse position for next frame
        //glfwSetCursorPos(window, 1024/2, 768/2);

        // Compute new orientation

        glm::mat4 transposed_transform = glm::transpose(transform);

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS){
            float horizontalAngle = mouseSpeed * (xpos - xpos_last) / 1024.0;
            float verticalAngle   = mouseSpeed * (ypos - ypos_last) / 768.0;

            // in local space
            transform = glm::rotate(transform, horizontalAngle, glm::vec3(transposed_transform[1]));
            transform = glm::rotate(transform, verticalAngle, glm::vec3(transposed_transform[0]));

        }


        lastTime = currentTime;
        xpos_last = xpos;
        ypos_last = ypos;

        // Move forward

        if (glfwGetKey( window, GLFW_KEY_UP ) == GLFW_PRESS){
            transform = glm::translate(
                transform,
                glm::vec3(transposed_transform[2]) * deltaTime * speed
            );
        }
        // Move backward
        if (glfwGetKey( window, GLFW_KEY_DOWN ) == GLFW_PRESS){
            transform = glm::translate(
                transform,
                glm::vec3(-transposed_transform[2]) * deltaTime * speed
            );
        }
        // Strafe right
        if (glfwGetKey( window, GLFW_KEY_RIGHT ) == GLFW_PRESS){
            transform = glm::translate(
                transform,
                glm::vec3(-transposed_transform[0]) * deltaTime * speed
            );
        }
        // Strafe left
        if (glfwGetKey( window, GLFW_KEY_LEFT ) == GLFW_PRESS){
            transform = glm::translate(
                transform,
                glm::vec3(transposed_transform[0]) * deltaTime * speed
            );
        }
	}
};