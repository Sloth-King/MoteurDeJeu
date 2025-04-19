// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>


// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include <common/shader.hpp>
#include <common/objloader.hpp>
#include <common/vboindexer.hpp>

#include "engine/includes/core.h"
#include "engine/includes/components.h"


#include "src/voxel/voxel.h"

using Utils::print;

// juste une petite structure par praticité. La génération de voxel fonctionne sans, donc il faut donner les paramètres à la main
struct Img3DData{
    std::string path;
    int sx, sy, sz;
    float  scalex, scaley, scalez;
};


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
    object1.addComponent<C_voxelMesh>();

    // generation

    Img3DData engine = {"../game/resources/engine.256x256x128.1.0x1.0x1.0.img", 256, 256, 128, 1.0, 1.0, 1.0};
    Img3DData manix = {"../game/resources/manixSansIV.512x512x48.0.4570x0.4570x3.0.img", 512, 512, 48, 0.470, 0.470, 3.0};
    Img3DData cube = {"../game/resources/cube.128x128x64.1.0x1.0x2.0.img", 128, 128, 64, 1.0, 1.0, 2.0};
    Img3DData statueLeg = {"../game/resources/statueLeg.341x341x93.1.0x1.0x4.0.img", 341, 341, 93, 1.0, 1.0, 4.0};

    int threshold_engine = 200;
    int threshold_manix = 800;
    int threshold_cube = 3000;
    int threshold_satueLeg = 15;

    // en mettre qu'un à la fois, sinon ça va override et prendre du temps pour rien

    // engine
    //object1.getComponent<C_voxelMesh>()->createFrom3DImg(engine.path, engine.sx, engine.sy, engine.sz, engine.scalex, engine.scaley, engine.scalez, threshold_engine, 7);
    // manix
    object1.getComponent<C_voxelMesh>()->createFrom3DImg(manix.path, manix.sx, manix.sy, manix.sz, manix.scalex, manix.scaley, manix.scalez, threshold_manix, 4);
    // cube
    //object1.getComponent<C_voxelMesh>()->createFrom3DImg(cube.path, cube.sx, cube.sy, cube.sz, cube.scalex, cube.scaley, cube.scalez, threshold_cube);
    // statueLeg
    //object1.getComponent<C_voxelMesh>()->createFrom3DImg(statueLeg.path, statueLeg.sx, statueLeg.sy, statueLeg.sz, statueLeg.scalex, statueLeg.scaley, statueLeg.scalez, threshold_satueLeg);

    game.current_scene.setRoot(std::move(object1));
    game.start();

    game.waitGameStopped();
}

