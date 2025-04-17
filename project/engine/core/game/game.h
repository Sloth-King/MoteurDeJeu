

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


struct GameSettings {
    int windowWidth;
    int windowHeight;
    float windowAspect;
    float fov;
};

class Game {

friend void handleWindowResizedCallback(GLFWwindow* window, int width, int height);
protected:

    void renderloop();

    std::thread render_thread;

    GLFWwindow* window;
    bool running = false;

    void handleWindowResized(GLFWwindow* window, int width, int height);


public:
    // updated automatically when a camera is added or the window is changed
    GameSettings settings;

    inline bool isRunning() const { return running;}

    GLFWwindow* getWindow() {
        return window;
    }

    void setCurrentCamera(Camera& c){
        c.resize(settings.windowWidth, settings.windowHeight);
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