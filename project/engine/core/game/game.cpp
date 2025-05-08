
#include "game.h"

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <thread>


// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

using namespace glm;

#include <common/shader.hpp>
#include <common/objloader.hpp>
#include <common/vboindexer.hpp>

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

inline void limit_fps(int FPS){
    static double last_time = glfwGetTime();

    double current_time = glfwGetTime();
    double ms = 1.0 / FPS;
    while (current_time - last_time <= ms){
        current_time = glfwGetTime();
    }

    last_time = current_time;
}

void Game::handleWindowResized(GLFWwindow* window, int width, int height){
    settings.windowWidth = width;
    settings.windowHeight = height;
    // important
    glViewport(0, 0, width, height);
    if (current_scene.current_camera){
        current_scene.current_camera->resize(width, height); // update projection matrix
    }
}

Game* current_game; // for this, because methods cant be glfw callbacks
void handleWindowResizedCallback(GLFWwindow* window, int width, int height){
    current_game->handleWindowResized(window, width, height);
}

void GLAPIENTRY
MessageCallback( GLenum source,
                 GLenum type,
                 GLuint id,
                 GLenum severity,
                 GLsizei length,
                 const GLchar* message,
                 const void* userParam )
{
    return ;//
  fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
           ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
            type, severity, message );
}


void Game::init()
{
    // Initialise GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        getchar();
        return ;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

    // Open a window and create its OpenGL context
    window = glfwCreateWindow( settings.windowWidth, settings.windowHeight, "Game", NULL, NULL);
    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window." );
        getchar();
        glfwTerminate();
        return ;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return ;
    }


    // During init, enable debug output
    glEnable              ( GL_DEBUG_OUTPUT );
    glDebugMessageCallback( MessageCallback, 0 );

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    glClearColor(0.5f, 0.5f, 0.5f, 0.0f);


    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    glEnable (GL_PROGRAM_POINT_SIZE);
    glDepthFunc(GL_LEQUAL);

    current_game = this;

    glfwSetFramebufferSizeCallback(window, handleWindowResizedCallback);
    glEnable(GL_CULL_FACE);

}

void Game::renderloop(){
    glfwMakeContextCurrent(window);
    while (running) renderUpdate();
}

void Game::start(){
    running = true;
    /*
    render_thread = std::thread(
        &Game::renderloop,
        this
    );
    */
    Game::renderloop(); // TODO
}

float deltaTime = 0.0f;
double lastFrame = 0.0f;
void Game::renderUpdate(){
    // Measure speed
    // per-frame time logic
    // --------------------
    double currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    

    current_scene.__engineUpdate(deltaTime);

    physicsUpdate(); // called here for now

    // Swap buffers
    glfwSwapBuffers(window);
    glfwPollEvents();

    //limit_fps(60);
}

float physDeltaTime = 0.0f;
double physLastFrame = 0.0f;

void Game::physicsUpdate(){
    // Measure speed
    // per-frame time logic
    // --------------------
    double currentFrame = glfwGetTime();
    physDeltaTime = currentFrame - physLastFrame;
    lastFrame = currentFrame;

    // ANDREw call ton machin ici !

    current_scene.__enginePhysicsUpdate(physDeltaTime);
}

void Game::stop(){
    running = false;
    render_thread.join();
    glfwTerminate();
}