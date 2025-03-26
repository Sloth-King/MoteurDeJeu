// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>


// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

using namespace glm;

#include <common/shader.hpp>
#include <common/objloader.hpp>
#include <common/vboindexer.hpp>

#include "engine/includes/core.h"


using Utils::print;

int main( void )
{


    Camera camera;

    Game
    
    //Mesh mesh = Mesh::gen_tesselatedSquare(2, 2);
    Mesh mesh = Mesh::gen_tesselatedSquare(100, 100, 1.0, 1.0);
    mesh.rotate(-90, glm::vec3(1.0, 0.0, 0.0));
    mesh.setShader(vertex_shader_filename, fragment_shader_filename);

    // load textures
    Texture heightmap("/home/e20210002460/Master/Moteur_de_jeux/MoteurDeJeu/TP1_code/TP1/heightmap.jpg");
    Texture rock("/home/e20210002460/Master/Moteur_de_jeux/MoteurDeJeu/TP1_code/TP1/textures/rock.png");
    Texture grass("/home/e20210002460/Master/Moteur_de_jeux/MoteurDeJeu/TP1_code/TP1/textures/grass.png");
    Texture snow("/home/e20210002460/Master/Moteur_de_jeux/MoteurDeJeu/TP1_code/TP1/textures/snowrocks.png");

    mesh.addTexture(heightmap, "heightmap");
    mesh.addTexture(rock, "rock");
    mesh.addTexture(grass, "grass");
    mesh.addTexture(snow, "snow");

    print(mesh);

    // For speed computation
    double lastTime = glfwGetTime();
    int nbFrames = 0;

    do{

        // Measure speed
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // inputœ
        // -----
        processInput(window);


        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera.computeMatricesFromInputs();
        //std::cout << glm::to_string(camera.transform) << std::endl;

        mesh.render(camera.getVP());

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

        limit_fps(60);

    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0 );

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    //TODO add translations

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
