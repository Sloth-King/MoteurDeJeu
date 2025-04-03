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
#include "engine/includes/components.h"

using Utils::print;

int main( void )
{

    std::string vertex_shader_filename = "/home/e20210002460/Master/Moteur_de_jeux/MoteurDeJeu/TP1_code/TP1/vertex_shader.glsl";

    std::string fragment_shader_filename = "/home/e20210002460/Master/Moteur_de_jeux/MoteurDeJeu/TP1_code/TP1/fragment_shader.glsl";
    
    Game game;

    game.init();

    Mesh mesh = Mesh::gen_tesselatedSquare(20, 20);
    //Mesh mesh = ResourceLoader::load_mesh_off("/home/e20210002460/Master/Moteur_de_jeux/MoteurDeJeu/TP1_code/TP1/sphere1.off");
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
    


    Camera camera;

    game.setCurrentCamera(camera);


    // gameobjects

    GameObject sphere1;
    //GameObject sphere2;

    // components

    //C_Mesh mesh_comp(mesh);

    //C_Transform transform_comp;

    // scene setup
    sphere1.addComponent<C_Transform>();
    sphere1.addComponent<C_Mesh>();
    sphere1.getComponent<C_Mesh>()->mesh = mesh;

    game.current_scene.setRoot(std::move(sphere1));



    game.start();

    game.waitGameStopped();
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
