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

void createSpheres(GameObject &parent, int n, float radius)
{

    for (int i = 0; i < n; ++i)
    {
        static Mesh sphere = ResourceLoader::load_mesh_obj(path_prefix_from_build + "resources/meshes/unit_sphere.obj");
        GameObject ball;
        auto *ballTransofrm = ball->addComponent<C_Transform>();

        ballTransofrm->setScale(glm::vec3(0.2, 0.2, 0.2));
        ballTransofrm->setPositionGlobal(glm::ballRand(radius) / 1.0f);

        auto *ballMesh = ball->addComponent<C_Mesh>();

        ballMesh->mesh = sphere;
        // Add physics
        ball->addComponent<C_RigidBody>();
        // ball->getComponent<C_RigidBody>()->setVelocity(glm::ballRand(radius / 200.0));
        ball->getComponent<C_RigidBody>()->restitution = 0.1f;
        ball->getComponent<C_RigidBody>()->mass = 10.0f;
        ball->addComponent<C_Collider>()->collider.sphere = SphereCollider(glm::vec3(0.0f), 1.0f);

        parent->addChild(std::move(ball));
    }
}

void createCubes(GameObject &parent, int n, float radius)
{

    for (int i = 0; i < n; ++i)
    {
        static Mesh sphere = ResourceLoader::load_mesh_obj(path_prefix_from_build + "resources/meshes/cube.obj");
        GameObject cube;
        auto *cubeTransofrm = cube->addComponent<C_Transform>();

        cubeTransofrm->setScale(glm::vec3(0.2, 0.2, 0.2));
        cubeTransofrm->setPositionGlobal(glm::vec3(glm::linearRand(-radius, radius), glm::linearRand(0.0f, radius), glm::linearRand(-radius, radius)));

        auto *cubeMesh = cube->addComponent<C_Mesh>();
        // cubeMesh->mesh.addTexture(Texture(path_prefix_from_build + "resources/textures/rubiks_texture.jpg"), "albedo");

        cubeMesh->mesh = sphere;
        // Add physics
        cube->addComponent<C_RigidBody>();
        cube->getComponent<C_RigidBody>()->setVelocity(glm::vec3(0.0f));
        cube->addComponent<C_Collider>()->collider.cube = CubeCollider(glm::vec3(0.0f), 0.5f);
        cube->getComponent<C_RigidBody>()->isStatic = true;

        parent->addChild(std::move(cube));
    }
}

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

    // scene objects

    Mesh sphere = ResourceLoader::load_mesh_off(path_prefix_from_build + "resources/meshes/sphere2.off");
    Mesh plane = ResourceLoader::load_mesh_obj(path_prefix_from_build + "resources/meshes/test.obj");

    GameObject floor;

    auto *floorTransofrm = floor->addComponent<C_Transform>();

    floorTransofrm->setPosition(glm::vec3(0., -2., -8.0));
    floorTransofrm->setScale(glm::vec3(10, 10, 10));
    floorTransofrm->setRotation(glm::vec3(10.0f,0.0f,0.0f));

    auto *floorMesh = floor->addComponent<C_Mesh>();
    floorMesh->mesh.addTexture(Texture(path_prefix_from_build + "resources/textures/submarine.jpg"), "albedo");

    floorMesh->mesh = plane;

    floor->addComponent<C_RigidBody>()->isStatic = true;
    floor->getComponent<C_RigidBody>()->mass = 1000000.0f;
    floor->addComponent<C_Collider>()->collider.plane = PlaneCollider(glm::vec3(0.f, 1.f, 0.f), -0.0f);

    // createSpheres(world, 1, 1.0);
    createCubes(world, 4000, 1.0);

    world->addChild(std::move(floor));

    // Create scene
    Scene scene;
    scene.environment = std::move(createEnvironment("resources/textures/cloudy_skybox"));
    scene.setRoot(std::move(world));

    game.setScene(std::move(scene));

    game.start();

    game.waitGameStopped();
}