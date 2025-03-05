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

void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;



// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

//rotation
float angle = 0.;
float zoom = 1.;
/*******************************************************************************/

using Utils::print;


inline void limit_fps(int FPS){
    static double last_time = glfwGetTime();

    double current_time = glfwGetTime();
    double ms = 1.0 / FPS;
    while (current_time - last_time <= ms){
        current_time = glfwGetTime();
    }

    last_time = current_time;
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


int main( void )
{
    // Initialise GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        getchar();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

    // Open a window and create its OpenGL context
    window = glfwCreateWindow( 1024, 768, "TP1 - GLFW", NULL, NULL);
    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window." );
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }
    // During init, enable debug output
    glEnable              ( GL_DEBUG_OUTPUT );
    glDebugMessageCallback( MessageCallback, 0 );

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    // Hide the mouse and enable unlimited mouvement
    //  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set the mouse at the center of the screen
    glfwPollEvents();
    glfwSetCursorPos(window, 1024/2, 768/2);

    // Dark blue background
    glClearColor(0.5f, 0.5f, 0.5f, 0.0f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    glEnable (GL_PROGRAM_POINT_SIZE);

    std::cout << "yolo" << std::endl;
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    // Cull triangles which normal is not towards the camera
    //glEnable(GL_CULL_FACE);

    /*****************TODO***********************/
    // Get a handle for our "Model View Projection" matrices uniforms

    /****************************************/

    std::string filename("/home/e20210002460/Master/Moteur_de_jeux/MoteurDeJeu/TP1_code/TP1/chair.off");
    // std::cout << "\n\n\n\n\n\n" << std::endl;

    std::string vertex_shader_filename = "/home/e20210002460/Master/Moteur_de_jeux/MoteurDeJeu/TP1_code/TP1/vertex_shader.glsl";
    std::string fragment_shader_filename = "/home/e20210002460/Master/Moteur_de_jeux/MoteurDeJeu/TP1_code/TP1/fragment_shader.glsl";


    Camera camera;
    
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
