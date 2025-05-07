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


using Utils::print;


int andrew( void )
{

    std::string path_prefix_from_build = "../game/";

    Game game;
    game.settings.windowWidth = 720;
    game.settings.windowHeight = 720;

    game.init();


    // world
    GameObject world;

    //auto* v = world.addComponent<C_MapManager>();
    //v->initChunks();

    game.current_scene.setRoot(std::move(world));

    Mesh submarine = ResourceLoader::load_mesh_obj(path_prefix_from_build + "resources/meshes/sousmarin_v1.obj");

    GameObject player;

    player.addComponent<C_Transform>();
    auto* playerMesh = player.addComponent<C_Mesh>();
    playerMesh->mesh = submarine;

    world.addChild(std::move(player));

    game.start();

    game.waitGameStopped();
}