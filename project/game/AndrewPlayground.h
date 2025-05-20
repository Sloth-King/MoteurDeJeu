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

void andrew(void)
{
    // Setup game
    Game game;
    game.settings.windowWidth = 720;
    game.settings.windowHeight = 720;

    game.init();

    // world
    GameObject world;
    world->addComponent<C_Transform>();

    // light

    createLightObject(world , glm::vec3(1.0f,1.0f,1.0f) , glm::vec3(100.0f));

    // scene objects

    static Mesh cubemesh_obj = ResourceLoader::load_mesh_obj(path_prefix_from_build + "resources/meshes/cube.obj");

    cubemesh_obj.material = Handle<MaterialPBR>(
        Texture(path_prefix_from_build + "resources/textures/submarine.jpg")
    );

    GameObject cube;
    auto *cubeTransofrm = cube->addComponent<C_Transform>();

    cubeTransofrm->setPositionGlobal(glm::vec3(0.0f,0.0f,1.0f));
    cubeTransofrm->setScale(glm::vec3(1.0, 1.0, 1.0));

    auto *cubeMesh = cube->addComponent<C_Mesh>();
    cubeMesh->mesh = cubemesh_obj;

    // Add physics
    cube->addComponent<C_RigidBody>();
    cube->getComponent<C_RigidBody>()->setVelocity(glm::vec3(0.0f));
    cube->addComponent<C_Collider>()->collider.cube = CubeCollider(glm::vec3(0.0f), 0.5f);
    cube->getComponent<C_RigidBody>()->isStatic = true;

    world->addChild(std::move(cube));

    // Create scene
    Scene scene;
    scene.environment = std::move(createEnvironment("resources/textures/cloudy_skybox"));
    scene.setRoot(std::move(world));

    game.setScene(std::move(scene));

    game.start();

    game.waitGameStopped();
}