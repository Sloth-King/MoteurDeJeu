#pragma once

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/random.hpp>

#include "engine/includes/core.h"
#include "engine/includes/components.h"

#include "src/mapManager/mapManager.h"


using Utils::print;


void createSpheres(GameObject & parent, int n, float radius){

    for (int i =0; i < n; ++i){
        static Mesh sphere = ResourceLoader::load_mesh_off(path_prefix_from_build + "resources/meshes/sphere2.off");
        GameObject ball;
        auto* ballTransofrm = ball->addComponent<C_Transform>();

        ballTransofrm->setScale(glm::vec3(0.2,0.2,0.2));
        ballTransofrm->setPositionGlobal(glm::ballRand(radius)/1.0f);

        auto* ballMesh = ball->addComponent<C_Mesh>();

        ballMesh->mesh = sphere;
        // Add physics
        ball->addComponent<C_RigidBody>();
        ball->getComponent<C_RigidBody>()->setVelocity(glm::ballRand(radius / 200.0));
        ball->addComponent<C_Collider>()->collider.sphere = SphereCollider(glm::vec3(0.0f), 1.0f);

        parent->addChild(std::move(ball));
    }
}

void andrew( void )
{
    //Setup game
    Game game;
    game.settings.windowWidth = 1850;
    game.settings.windowHeight = 990;

    game.init();

    // world
    GameObject world;
    world->addComponent<C_Transform>();

    //scene objects

    Mesh sphere = ResourceLoader::load_mesh_off(path_prefix_from_build + "resources/meshes/sphere2.off");
    Mesh plane = ResourceLoader::load_mesh_obj(path_prefix_from_build + "resources/meshes/test.obj");

    GameObject floor;


    auto* floorTransofrm = floor->addComponent<C_Transform>();

    floorTransofrm->setPosition(glm::vec3(0.,-2.,-8.0));
    floorTransofrm->setScale(glm::vec3(1000, 1000, 1000));

    auto* floorMesh = floor->addComponent<C_Mesh>();
    floorMesh->mesh.addTexture(Texture(path_prefix_from_build + "resources/textures/submarine.jpg"), "albedo");


    floorMesh->mesh = plane;

    floor->addComponent<C_RigidBody>();
    floor->getComponent<C_RigidBody>()->mass = 1000000.0f;
    floor->addComponent<C_Collider>()->collider.plane = PlaneCollider(glm::vec3(0.f,1.f,0.f), -0.0f);

    createSpheres(world, 100, 1.0);


    world->addChild(std::move(floor));


    // Create scene
    Scene scene;
    scene.environment = std::move(createEnvironment("resources/textures/cloudy_skybox"));
    scene.setRoot(std::move(world));

    game.setScene(std::move(scene));

    game.start();

    game.waitGameStopped();
}