#pragma once

#pragma once

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "engine/includes/core.h"
#include "engine/includes/components.h"

#include "src/mapManager/mapManager.h"
#include "src/controller/playerController.h"



using Utils::print;

static std::string path_prefix_from_build = "../game/";


void addKeybind(){
    Input::addKeybind(
        "move_right",
        KeySpec(
            BINDTYPE_KEYBOARD,
            GLFW_KEY_D
        )
    );
    Input::addKeybind(
        "move_left",
        KeySpec(
            BINDTYPE_KEYBOARD,
            GLFW_KEY_A
        )
    );

    Input::addKeybind(
        "move_forward",
        KeySpec(
            BINDTYPE_KEYBOARD,
            GLFW_KEY_W
        )
    );
    Input::addKeybind(
        "move_backwards",
        KeySpec(
            BINDTYPE_KEYBOARD,
            GLFW_KEY_S
        )
    );



    Input::addKeybind(
        "move_down",
        KeySpec(
            BINDTYPE_KEYBOARD,
            GLFW_KEY_LEFT_SHIFT
        )
    );
    Input::addKeybind(
        "move_up",
        KeySpec(
            BINDTYPE_KEYBOARD,
            GLFW_KEY_SPACE
        )
    );

}

GameObject createPlayer(){

    const glm::vec3 cameraOffset(1.3, 0.0, -3.5);

    GameObject player;

    Mesh submarine = ResourceLoader::load_mesh_obj(path_prefix_from_build + "resources/meshes/sousmarin_v2.obj");
    submarine.material = Handle<MaterialPBR>(
        Texture(path_prefix_from_build + "resources/textures/submarine.jpg")
    );

    auto* playerTransform = player->addComponent<C_Transform>();
    playerTransform->setScale(glm::vec3(0.07, 0.07, 0.07));
    playerTransform->move(glm::vec3(0, 0.2, 0));

    player->addComponent<C_Camera>() -> offset = cameraOffset;

    player->addComponent<C_Mesh>()->mesh = submarine;

    player->addComponent<C_PlayerController>();
    
    return player;
}


Environment createEnvironment(const std::string & skybox_path = "resources/textures/skybox"){
    Environment env;

    auto cubemap = CubeMap(
            path_prefix_from_build + skybox_path.c_str()
        );

    env.skybox = Skybox( std::move(cubemap));

    return env;
}


Scene createScene(){
    Scene scene;

    scene.environment = std::move(
        createEnvironment()
    );

    // world
    GameObject world;
    world->addComponent<C_Transform>();

    auto* v = world->addComponent<C_MapManager>();

    GameObject player = createPlayer();

    world->addChild(std::move(player));

    v->player = player;
    v->initChunks();

    scene.setRoot(std::move(world));

    return scene;
}

void game( void )
{

    Game game;
    game.settings.windowWidth = 1280;
    game.settings.windowHeight = 720;

    game.init();
    addKeybind();

    game.setScene(
        createScene()
    );

    game.start();

    game.waitGameStopped();
}