

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <map>
#include <iostream>
#include <memory>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "TP1/engine/utils/utils.h"
#include "TP1/engine/core/scene/scene.h"


class Game {
protected:
    bool running = false;

public:
    Scene current_scene;

    void launch();

    void initialize_gl();

    void update_game();

    void :stop();

}