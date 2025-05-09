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

void andrew( void )
{
    //Setup game
    Game game;
    game.settings.windowWidth = 720;
    game.settings.windowHeight = 720;

    game.init();

    // world
    GameObject world;
    world.addComponent<C_Transform>();

    //scene objects

    Mesh sphere = ResourceLoader::load_mesh_off(path_prefix_from_build + "resources/meshes/sphere2.off");
    Mesh plane = ResourceLoader::load_mesh_obj(path_prefix_from_build + "resources/meshes/test.obj");

    GameObject ball;
    GameObject floor;

    auto* ballTransofrm = ball.addComponent<C_Transform>();
    auto* floorTransofrm = floor.addComponent<C_Transform>();

    ballTransofrm->setScale(glm::vec3(0.2,0.2,0.2));
    ballTransofrm->setPosition(glm::vec3(0.0,2.0,0.0));

    floorTransofrm->setPosition(glm::vec3(0.0,-0.5,0.0));

    auto* ballMesh = ball.addComponent<C_Mesh>();
    //ballMesh->mesh.addTexture(Texture(path_prefix_from_build + "resources/textures/submarine.jpg"), "albedo");
    auto* floorMesh = floor.addComponent<C_Mesh>();
    floorMesh->mesh.addTexture(Texture(path_prefix_from_build + "resources/textures/submarine.jpg"), "albedo");


    ballMesh->mesh = sphere;
    floorMesh->mesh = plane;

    // Add physics
    ball.addComponent<C_RigidBody>();
    ball.addComponent<C_Collider>();
    ball.getComponent<C_Collider>()->collider = SphereCollider(ballTransofrm->getPosition(), 2.0f);

    floor.addComponent<C_RigidBody>();
    floor.addComponent<C_Collider>();
    floor.getComponent<C_Collider>()->collider = PlaneCollider(glm::vec3(0.0,1.0,0.0), -0.5f);

    world.addChild(std::move(ball));
    world.addChild(std::move(floor));


    // Create scene
    Scene scene;
    scene.environment = std::move(createEnvironment("resources/textures/cloudy_skybox"));
    scene.setRoot(std::move(world));

    game.setScene(std::move(scene));

    game.start();

    game.waitGameStopped();
}