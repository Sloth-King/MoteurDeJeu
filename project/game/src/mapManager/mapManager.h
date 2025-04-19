#pragma once 
#include "engine/includes/core.h"
#include "engine/includes/components.h"
#include <vector>
#include <functional>
#include <string>
#include "game/src/voxel/voxel.h"


/*
three referentials:
- ChunkIdx is the indices of the chunks in the pseudo 3D chunks array
- ChunkCoords is the coordinate in the global chunk grid. Can differ from the first one if the player is not at the origin
- block_position is the position in the global world but as an integer. Helps for the offset in the mapgeneration file.
*/


class C_MapManager: Component{


public:
    
    const int chunkRadius = 4;

    std::vector < GameObject* > chunks;

    GameObject* player;

    glm::ivec3 current_player_chunk_in_map;

    C_MapManager(){
        chunks.resize((chunkRadius+1) * (chunkRadius+1) * (chunkRadius+1));
    }

    GameObject* getChunkAt(glm::ivec3 v){
        return chunks.at((v.z * chunkRadius * chunkRadius) + (v.y * chunkRadius) + v.x);
    }

    glm::ivec3 getPlayerChunkCoords() const {
        if (!player) return glm::ivec3(0);

        return glm::ivec3(0); // worrying about this later lol
    }

    glm::ivec3 getChunkCoords(glm::ivec3 chunkIdx) const {
        GameObject* chunk_ptr = chunks.at((chunkIdx.z * chunkRadius * chunkRadius) + (chunkIdx.y * chunkRadius) + chunkIdx.x);

        glm::vec3 translation = glm::vec3(chunk_ptr->getComponent<C_Transform>()->getGlobalTransformationMatrix()[2]);

        return glm::ivec3(
            translation.x / CHUNK_SIZE_XZ,
            translation.y / CHUNK_SIZE_Y,
            translation.z / CHUNK_SIZE_XZ
        );
    }

    inline GameObject* createChunk(glm::ivec3 block_offset){

        GameObject g;
        g.addComponent<C_Transform>();
        auto comp = g.addComponent<C_voxelMesh>();
        comp->create_chunk(block_offset);
        return getOwner().addChild(std::move(g));
    }
    /*
    void initChunks(){ // TODO
        for (int i = )
    }
    */
};