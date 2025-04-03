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


#include "src/voxel/voxel.h"

using Utils::print;

int main( void )
{

    std::string path_prefix_from_build = "../game/";

    std::string vertex_shader_filename = path_prefix_from_build + "resources/shaders/vertex_shader.glsl";

    std::string fragment_shader_filename = path_prefix_from_build + "resources/shaders/fragment_shader.glsl";
    
    Game game;

    game.init();

    Mesh mesh = Mesh::gen_tesselatedSquare(20, 20);
    //Mesh mesh = ResourceLoader::load_mesh_off("/home/e20210002460/Master/Moteur_de_jeux/MoteurDeJeu/TP1_code/sphere1.off");
    mesh.rotate(-90, glm::vec3(1.0, 0.0, 0.0));

    mesh.setShader(vertex_shader_filename, fragment_shader_filename);

    // load textures
    
    Texture heightmap(path_prefix_from_build + "resources/textures/heightmap.jpg");
    Texture rock(path_prefix_from_build + "resources/textures/rock.png");
    Texture grass(path_prefix_from_build + "resources/textures/grass.png");
    Texture snow(path_prefix_from_build + "resources/textures/snowrocks.png");

    mesh.addTexture(heightmap, "heightmap");
    mesh.addTexture(rock, "rock");
    mesh.addTexture(grass, "grass");
    mesh.addTexture(snow, "snow");
    


    Camera camera;
    camera.transform = glm::translate(camera.transform, glm::vec3(-10, 10, 0));

    game.setCurrentCamera(camera);


    std::function<int(glm::vec3)> f = [](glm::vec3 v){
        if (sin(v.y) + 0.9 <=0){
            return 1;
        }
        else{
            return 0;
        }
    };

    // gameobjects

    GameObject object1;

    // scene setup
    object1.addComponent<C_Transform>();

    object1.addComponent<C_Mesh>();
    object1.getComponent<C_Mesh>()->mesh = mesh;

    object1.addComponent<C_voxelMesh>();
    object1.getComponent<C_voxelMesh>()->create(20, 10, 20, 0.00000, f);

    game.current_scene.setRoot(std::move(object1));



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
