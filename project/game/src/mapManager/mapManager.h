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
    
    const int chunkRadiusXZ = 9;
    const int chunkRadiusY = 2;

    const float world_scale = 0.05f;

    std::vector < GameObject > chunks;

    GameObject player;

    glm::ivec3 current_player_pos_in_world;

    glm::ivec3 current_player_chunk_in_map;

    C_MapManager(){
        chunks.resize((2*chunkRadiusXZ+1) * (2*chunkRadiusY+1) * (2*chunkRadiusXZ+1));

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
            Utils::posmod(v.x + current_player_chunk_in_map.x, 2 * chunkRadiusXZ + 1),
            Utils::posmod(v.y + current_player_chunk_in_map.y, 2 * chunkRadiusY + 1),
            Utils::posmod(v.z + current_player_chunk_in_map.z, 2 * chunkRadiusXZ + 1)
        };

        return chunks.at((v.z * chunkRadiusXZ * chunkRadiusY) + (v.y * chunkRadiusY) + v.x);
    }

    void setChunkAt(glm::ivec3 v, GameObject chunk){ // ALWAYS relative to player. (0, 0) is the player, this function does the fun conversion part
        v = {
            Utils::posmod(v.x + current_player_chunk_in_map.x, 2 * chunkRadiusXZ + 1),
            Utils::posmod(v.y + current_player_chunk_in_map.y, 2 * chunkRadiusY + 1),
            Utils::posmod(v.z + current_player_chunk_in_map.z, 2 * chunkRadiusXZ + 1)
        };

        chunks.at((v.z * chunkRadiusXZ * chunkRadiusY) + (v.y * chunkRadiusY) + v.x) = chunk;
    }

    glm::ivec3 getPlayerChunkCoords() const {
        if (!player) return glm::ivec3(0);

        auto pos = player->getComponent<C_Transform>()->getGlobalPosition();

        pos /= world_scale;

        pos.x /= CHUNK_SIZE_XZ;
        pos.y /= CHUNK_SIZE_XZ;
        pos.z /= CHUNK_SIZE_Y;

        return glm::ivec3(pos);
    }

    inline GameObject createChunk(glm::ivec3 chunkCoord){

        GameObject g;

        glm::ivec3 global_pos = chunkCoord * glm::ivec3(CHUNK_SIZE_XZ, CHUNK_SIZE_Y, CHUNK_SIZE_XZ);

        auto* c_transform = g->addComponent<C_Transform>();
        c_transform->setPosition(glm::vec3(global_pos) * world_scale);

        auto* c_voxelmesh = g->addComponent<C_voxelMesh>();
        c_voxelmesh->size = world_scale;

        c_voxelmesh->mesh.setShaderPid(shaderPID);

        c_voxelmesh->mesh.addTexture(atlas.unsafeCopyTodoRemoveThat(), "atlas");
        c_voxelmesh->mesh.addTexture(roughness.unsafeCopyTodoRemoveThat(), "roughness_map");
        c_voxelmesh->mesh.addTexture(metallic.unsafeCopyTodoRemoveThat(), "metallic_map");
        c_voxelmesh->mesh.addTexture(normal.unsafeCopyTodoRemoveThat(), "normal_map"); 

        c_voxelmesh->create_chunk(global_pos);

        
        getOwner()->addChild(std::move(g));
        return g;
    }
    
    inline glm::ivec3 chunkIdxToChunkCoord(glm::ivec3 chunkIdx){
        return getPlayerChunkCoords() + chunkIdx;
    }
    
    void initChunks(){

        Utils::time<'c'>(); // init (print pas la première fois)

        current_player_pos_in_world = getPlayerChunkCoords();

        for (int i = -chunkRadiusXZ; i <= chunkRadiusXZ; ++i){
            for (int j = -chunkRadiusY; j <= chunkRadiusY; ++j){
                for (int k = -chunkRadiusXZ; k <= chunkRadiusXZ; ++k){
                    auto chunkIdx = glm::ivec3(i, j, k);
                    GameObject chunk = createChunk(chunkIdxToChunkCoord(chunkIdx));
                    setChunkAt(
                        chunkIdx,
                        chunk
                    );
                }
            }
        }
        Utils::time<'c'>(); // init (print et reset pas (donc les temps se cumulent si appelés plusieurs fois))
    }

    virtual void _onUpdate(float deltaTime) override {
        return; //TODO finir ça
        auto new_player_pos_in_world = getPlayerChunkCoords();

        if (new_player_pos_in_world == current_player_pos_in_world) return;

        auto delta = new_player_pos_in_world - current_player_pos_in_world;

        // remove and replace all chunks
        int sXZ =  2 * chunkRadiusXZ + 1;
        int sY = 2 * chunkRadiusXZ + 1;

        current_player_chunk_in_map += delta;

        current_player_chunk_in_map.x = Utils::posmod(current_player_chunk_in_map.x, sXZ);
        current_player_chunk_in_map.y = Utils::posmod(current_player_chunk_in_map.y, sXZ);
        current_player_chunk_in_map.z = Utils::posmod(current_player_chunk_in_map.z, sY);

        int number_on_x = std::min(abs(delta.x), sXZ);

        // a lot of loops. Feels big but they're all small on one axis at least
        
        // on x
        if (delta.x > 0)
            for (int i = 0; i < number_on_x; ++i){
                for (int j = 0; j < sY; ++j){
                    for (int k = 0; k < sXZ; ++k){
                        auto chunkIdx = glm::ivec3(i, j, k);

                        getChunkAt(chunkIdx)->queueDelete();
                        GameObject chunk = createChunk(chunkIdxToChunkCoord(chunkIdx));
                        setChunkAt(
                            chunkIdx,
                            chunk
                        );
                    }
                }
            }
        else 
            for (int i = 0; i < number_on_x; ++i){
                for (int j = 0; j < sY; ++j){
                    for (int k = 0; k < sXZ; ++k){
                        auto chunkIdx = glm::ivec3(sXZ - i, sY - j, sXZ - k);

                        getChunkAt(chunkIdx)->queueDelete();
                        GameObject chunk = createChunk(chunkIdxToChunkCoord(chunkIdx));
                        setChunkAt(
                            chunkIdx,
                            chunk
                        );
                    }
                }
            }

        
        // on y
        int number_on_y = std::min(abs(delta.y), sY);
        if (delta.y > 0)
            for (int i = number_on_x; i < sXZ; ++i){
                for (int j = 0; j < number_on_y; ++j){ // we don't have to do the corner we've already done
                    for (int k = 0; k < sXZ; ++k){
                        auto chunkIdx = glm::ivec3(i, j, k);

                        getChunkAt(chunkIdx)->queueDelete();
                        GameObject chunk = createChunk(chunkIdxToChunkCoord(chunkIdx));
                        setChunkAt(
                            chunkIdx,
                            chunk
                        );
                    }
                }
            }
        else 
            for (int i = number_on_x; i < sXZ; ++i){
                for (int j = 0; j < number_on_y; ++j){
                    for (int k = 0; k < sXZ; ++k){
                        auto chunkIdx = glm::ivec3(sXZ - i, sY - j, sXZ - k);

                        getChunkAt(chunkIdx)->queueDelete();
                        GameObject chunk = createChunk(chunkIdxToChunkCoord(chunkIdx));
                        setChunkAt(
                            chunkIdx,
                            chunk
                        );
                    }
                }
            }



        // on z
        int number_on_z = std::min(abs(delta.z), sXZ);
        if (delta.y > 0)
            for (int i = number_on_x; i < sXZ; ++i){
                for (int j = number_on_y; j < sY; ++j){ // we don't have to do the corner we've already done
                    for (int k = 0; k < number_on_z; ++k){
                        auto chunkIdx = glm::ivec3(i, j, k);

                        getChunkAt(chunkIdx)->queueDelete();
                        GameObject chunk = createChunk(chunkIdxToChunkCoord(chunkIdx));
                        setChunkAt(
                            chunkIdx,
                            chunk
                        );
                    }
                }
            }
        else 
            for (int i = number_on_x; i < sXZ; ++i){
                for (int j = number_on_y; j < sY; ++j){
                    for (int k = 0; k < number_on_z; ++k){
                        auto chunkIdx = glm::ivec3(sXZ - i, sY - j, sXZ - k);

                        getChunkAt(chunkIdx)->queueDelete();
                        GameObject chunk = createChunk(chunkIdxToChunkCoord(chunkIdx));
                        setChunkAt(
                            chunkIdx,
                            chunk
                        );
                    }
                }
            }
        current_player_pos_in_world = new_player_pos_in_world;
    }
};