#include "voxel.h"



Texture heightmap;

uint8_t generateVoxel(const glm::ivec3 & global_position){
    const int fac = 30;
    float h = heightmap(global_position.x*20, global_position.z*20).g / 255.0f * 20+10;
    //return 1; // debug
    
    if (global_position.y < -15){
        return 0;
    }

    if (global_position.y < h - 5){
        return 2;
    }

    if (global_position.y < h-1){
        return 3;
    }
    if (global_position.y < h){
        return 4;
    }
    return 0;
}

void generateStrate(VoxelContainer & container, int k, const glm::ivec3 & offset){

    glm::vec3 global_position;

    for (int i = 0; i < CHUNK_SIZE_XZ; ++i){
        for (int j = 0; j < CHUNK_SIZE_XZ; ++j){

            global_position = glm::ivec3(i, k, j) + offset;

            container.set(
                i, k, j,
                generateVoxel(global_position)
                );
        }
    }
}

void generateChunk(VoxelContainer & container, const glm::ivec3 offset){

    heightmap.loadTexture(std::string("../game/resources/textures/heightmap.jpg"), false);

    container = VoxelContainer(CHUNK_SIZE_XZ, CHUNK_SIZE_Y, CHUNK_SIZE_XZ);

    for (int k = 0; k < CHUNK_SIZE_Y; ++k){
        
        generateStrate(container, k, offset);
    }
}
