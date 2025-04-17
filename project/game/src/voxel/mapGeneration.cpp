#include "voxel.h"


const unsigned int CHUNK_SIZE_XZ = 32;
const unsigned int CHUNK_SIZE_Y = 16;


uint8_t generateVoxel(const glm::ivec3 & global_position){
    return 1; // debug
    if (global_position.y < -45){
        return 0;
    }
    if (global_position.y < 4){
        return 2;
    }

    if (global_position.y < 6){
        return 3;
    }
    return 0;
}

void generateStrate(VoxelContainer & container, int k, const glm::ivec3 & offset){

    glm::vec3 global_position;

    for (int i = 0; i < CHUNK_SIZE_XZ; ++i){
        for (int j = 0; j < CHUNK_SIZE_XZ; ++j){

            global_position = glm::ivec3(i, j, k) + offset;

            container.set(
                i, j, k,
                generateVoxel(global_position)
                );
        }
    }
}

void generateMap(VoxelContainer & container, const glm::ivec3 offset){

    container = VoxelContainer(CHUNK_SIZE_XZ, CHUNK_SIZE_Y, CHUNK_SIZE_XZ);

    for (int k = 0; k < CHUNK_SIZE_Y; ++k){
        
        generateStrate(container, k, offset);
    }
}
