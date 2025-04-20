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


int main( void )
{

    Game game;
    game.settings.windowWidth = 720;
    game.settings.windowHeight = 720;

    game.init();


    // gameobjects

    GameObject object1;

    // scene setup
    object1.addComponent<C_Transform>();
    auto* v = object1.addComponent<C_MapManager>();
    v->initChunks();

    game.current_scene.setRoot(std::move(object1));
    game.start();

    game.waitGameStopped();
}

