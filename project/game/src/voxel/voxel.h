#pragma once 
#include "engine/includes/core.h"
#include "engine/includes/components.h"
#include <vector>
#include <functional>
#include <string>

using VOXEL_IDX_TYPE = uint8_t;

struct VoxelType {

    VOXEL_IDX_TYPE textureIndex = 0;

    unsigned int solidity;
    std::string name;
};

// externalized to be able to swap it easily. Maybe do an octree in the end (or a mix octree above surface / grid below ?)
struct VoxelContainer {
    size_t sX, sY, sZ;

    std::vector<VOXEL_IDX_TYPE> data;

    VoxelContainer( size_t x, size_t y, size_t z ): sX(x), sY(y), sZ(z) {
        data.resize(x * y * z);
    }

    inline VOXEL_IDX_TYPE get( size_t x, size_t y, size_t z ) {
        return data.at((z * sX * sY) + (y * sX) + x);
    }

    inline void set(size_t x, size_t y, size_t z, VOXEL_IDX_TYPE val) {
        data.at((z * sX * sY) + (y * sX) + x) = val;
    }

    VoxelContainer() = default;
};

const unsigned int CHUNK_SIZE_XZ = 32;
const unsigned int CHUNK_SIZE_Y = 16;

void generateChunk(VoxelContainer & container, glm::ivec3 offset);

class C_voxelMesh: public C_Mesh{

    void voxelize();

public:

    static const unsigned int  voxelTextureSize = 8; // nb of blocks. CHANGE IN THE SHADER ASWELL !
    const float size = 0.05;

    std::vector< VoxelType > types;

    VoxelContainer container;

    //VoxelType v = {1, 500, "debug"};
    C_voxelMesh() {
        types = {
            {1, 500, "debug"},
            {2, 100, "basalt"},
            {3, 75, "sandstone"},
            {4, 5, "sand"},
            {5, 10, "glorksmorf"},
            {6, 100, "copper"},
            {7, 150, "stone"},
            {8, 120, "ivory"},
        };
    }

    void create_chunk(glm::ivec3 offset = glm::ivec3(0)){

        generateChunk(container, offset);

        voxelize();

        // not ideal to have the shader + texture part here since it's gonna create it each time. We could do better 
        std::string path_prefix_from_build = "../game/";
        std::string vertex_shader_filename = path_prefix_from_build + "resources/shaders/voxel_shader_vert.glsl";
        std::string fragment_shader_filename = path_prefix_from_build + "resources/shaders/voxel_shader_frag.glsl";

        mesh.setShader(vertex_shader_filename, fragment_shader_filename);

        // load textures
        Texture atlas(path_prefix_from_build + "resources/textures/voxelAtlas.png");

        mesh.addTexture(atlas, "atlas");

        //std::cout << "points size: " << mesh.triangles.size() << std::endl;
    }

};