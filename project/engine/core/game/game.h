

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

    Game(){
        current_scene.current_game = this;
    }

    inline bool isRunning() const { return running;}

    GLFWwindow* getWindow() {
        return window;
    }

    void setCurrentCamera(Camera& c){
        c.resize(settings.windowWidth, settings.windowHeight);
        current_scene.setCurrentCamera(c);
    }

    Scene current_scene;

    Scene * setScene(Scene && scene){ // TODO call enter and exitscene if comp added at runtime
        current_scene.__unloaded();
        current_scene = std::move(scene);
        current_scene.current_game = this;
        current_scene.__loaded();
        return &current_scene;
    }


    void start();

    void init();

    void renderUpdate();

    void physicsUpdate();

    void stop();

    void waitGameStopped(){
        while (running) {}
    }
};