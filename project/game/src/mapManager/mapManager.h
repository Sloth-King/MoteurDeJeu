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


class C_MapManager: public Component{
    GLuint shaderPID;
    Texture atlas;
    Texture roughness;
    Texture metallic;
    Texture normal;

public:
    
    const int chunkRadius = 4;
    const float world_scale = 0.05f;

    std::vector < GameObject > chunks; // for now a pointer. this is NOT safe because things can reparent.
    // acceptable refactor : include in the scene a hashmap of the objects id against a pointer to them, which is updated when needed.
    // then any component in the scene can access the gameobject with its id in O(1)


    GameObject player;

    glm::ivec3 current_player_chunk_in_map;

    C_MapManager(){
        chunks.resize((2*chunkRadius+1) * (2*chunkRadius+1) * (2*chunkRadius+1));

        std::string path_prefix_from_build = "../game/";
        std::string vertex_shader_filename = path_prefix_from_build + "resources/shaders/voxel_shader_vert.glsl";
        std::string fragment_shader_filename = path_prefix_from_build + "resources/shaders/voxel_shader_frag.glsl";

        shaderPID = loadShadersFromFileGLSL(vertex_shader_filename.c_str(), fragment_shader_filename.c_str());

        atlas = Texture(path_prefix_from_build + "resources/textures/voxelAtlas.png");
        roughness = Texture(path_prefix_from_build + "resources/textures/voxelAtlas_Roughness.png");
        metallic = Texture(path_prefix_from_build + "resources/textures/voxelAtlas_Metallic.png");
        normal = Texture(path_prefix_from_build + "resources/textures/voxelAtlas_Normal.png");
        
    }

    GameObject getChunkAt(glm::ivec3 v){ // ALWAYS relative to player. (0, 0) is the player, this function does the fun conversion part
        v = {
            Utils::posmod(v.x + current_player_chunk_in_map.x, 2 * chunkRadius + 1),
            Utils::posmod(v.y + current_player_chunk_in_map.y, 2 * chunkRadius + 1),
            Utils::posmod(v.z + current_player_chunk_in_map.z, 2 * chunkRadius + 1)
        };

        return chunks.at((v.z * chunkRadius * chunkRadius) + (v.y * chunkRadius) + v.x);
    }

    void setChunkAt(glm::ivec3 v, GameObject chunk){ // ALWAYS relative to player. (0, 0) is the player, this function does the fun conversion part
        v = {
            Utils::posmod(v.x + current_player_chunk_in_map.x, 2 * chunkRadius + 1),
            Utils::posmod(v.y + current_player_chunk_in_map.y, 2 * chunkRadius + 1),
            Utils::posmod(v.z + current_player_chunk_in_map.z, 2 * chunkRadius + 1)
        };

        chunks.at((v.z * chunkRadius * chunkRadius) + (v.y * chunkRadius) + v.x) = chunk;
    }

    glm::ivec3 getPlayerChunkCoords() const {
        if (!player) return glm::ivec3(0);

        return glm::ivec3(0); // worrying about this later lol
    }

    inline GameObject createChunk(glm::ivec3 chunkCoord){

        GameObject g;

        glm::ivec3 global_pos = chunkCoord * glm::ivec3(CHUNK_SIZE_XZ, CHUNK_SIZE_Y, CHUNK_SIZE_XZ);

        auto* c_transform = g->addComponent<C_Transform>();
        c_transform->setPosition(glm::vec3(global_pos) * world_scale);

        auto* c_voxelmesh = g->addComponent<C_voxelMesh>();
        c_voxelmesh->size = world_scale;

        c_voxelmesh->mesh.setShaderPid(shaderPID);

        c_voxelmesh->mesh.addTexture(atlas, "atlas");
        c_voxelmesh->mesh.addTexture(roughness, "roughness_map");
        c_voxelmesh->mesh.addTexture(metallic, "metallic_map");
        c_voxelmesh->mesh.addTexture(normal, "normal_map"); 


        c_voxelmesh->create_chunk(global_pos);

        
        getOwner()->addChild(std::move(g));
        return g;
    }
    
    inline glm::ivec3 chunkIdxToChunkCoord(glm::ivec3 chunkIdx){
        return getPlayerChunkCoords() + chunkIdx;
    }
    
    void initChunks(){

        for (int i = -chunkRadius; i <= chunkRadius; ++i){
            for (int j = -1; j <= 1; ++j){
                for (int k = -chunkRadius; k <= chunkRadius; ++k){
                    auto chunkIdx = glm::ivec3(i, j, k);
                    GameObject chunk = createChunk(chunkIdxToChunkCoord(chunkIdx));
                    //std::cout << chunk->getComponent<C_voxelMesh>()->mesh << std::endl;
                    setChunkAt(
                        chunkIdx,
                        chunk
                    );
                }
            }
        }

    }
};