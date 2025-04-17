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

    Game game;

    game.init();

    Camera camera;
    camera.transform = glm::translate(camera.transform, glm::vec3(0, 0, 0));

    // gameobjects
    Mesh mesh1 = ResourceLoader::load_mesh_off("../game/resources/meshes/sphere1.off");

    GameObject object1;
    GameObject object2;

    object2.addComponent<C_Transform>();
    object2.getComponent<C_Transform>()->setScale(glm::vec3(0.2, 0.2, 0.2));
    object2.addComponent<C_Mesh>();
    object2.getComponent<C_Mesh>()->mesh = mesh1;
    object2.addComponent<C_Movement>();

    object2.addComponent<C_Camera>();
    object2.getComponent<C_Camera>()->camera = camera;
    object2.getComponent<C_Camera>()->offset = glm::vec3(0.0,0.0,5.0);

    // scene setup
    object1.addComponent<C_Transform>();
    object1.addComponent<C_voxelMesh>();
    object1.getComponent<C_voxelMesh>()->create_chunk();
    
    object1.addChild(std::move(object2));

    //game.setCurrentCamera(camera);

    game.current_scene.setRoot(std::move(object1)); // do last because it moves !

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
