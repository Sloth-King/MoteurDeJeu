

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <map>
#include <iostream>
#include <memory>
#include <thread>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "engine/utils/utils.h"
#include "engine/core/scene/scene.h"




class Game {
protected:

    void renderloop();

    std::thread render_thread;

    GLFWwindow* window;
    bool running = false;

public:

    inline bool isRunning() const { return running;}

    GLFWwindow* getWindow() {
        return window;
    }

    void setCurrentCamera(Camera& c){
        c.window = window;
        current_scene.setCurrentCamera(c);
    }

    Scene current_scene;

    void start();

    void init();

    void render_update();

    //void fixed_update();

    void stop();

    void waitGameStopped(){
        while (running) {}
    }
};