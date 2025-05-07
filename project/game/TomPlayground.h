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

static std::string path_prefix_from_build = "../game/";

GameObject createPlayer(){
    GameObject player;

    Mesh submarine = ResourceLoader::load_mesh_obj(path_prefix_from_build + "resources/meshes/sousmarin_v1.obj");

    auto* playerTransform = player.addComponent<C_Transform>();
    playerTransform->setScale(glm::vec3(0.07, 0.07, 0.07));
    playerTransform->move(glm::vec3(0, 1, 0));


    auto* playerMesh = player.addComponent<C_Mesh>();
    playerMesh->mesh = submarine;
    playerMesh->mesh.addTexture( Texture(path_prefix_from_build + "resources/textures/submarine.jpg"), "albedo" );
    
    return player;
}

int tom( void )
{

    Game game;
    game.settings.windowWidth = 720;
    game.settings.windowHeight = 720;

    game.init();


    // world
    GameObject world;
    world.addComponent<C_Transform>();

    auto* v = world.addComponent<C_MapManager>();
    v->initChunks();

    GameObject player = createPlayer();

    world.addChild(std::move(player));

    game.current_scene.setRoot(std::move(world));

    game.start();

    game.waitGameStopped();
}