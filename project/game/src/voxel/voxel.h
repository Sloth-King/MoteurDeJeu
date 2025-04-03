#pragma once 
#include "engine/includes/core.h"
#include "engine/includes/components.h"
#include <vector>
#include <functional>

using VOXEL_IDX_TYPE = uint8_t;

/*
public int[] to3D( int idx ) {
    final int z = idx / (xMax * yMax);
    idx -= (z * xMax * yMax);
    final int y = idx / xMax;
    final int x = idx % xMax;
    return new int[]{ x, y, z };
}
*/

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
};


class C_voxelMesh: public C_Mesh{

    size_t sX = 20;
    size_t sY = 10;
    size_t sZ = 20;
    float size = 1.0;

    VoxelContainer container = VoxelContainer(sX, sY, sZ);


    void voxelize(std::function<int(glm::vec3)> f);

public:
    void create ( size_t x, size_t y, size_t z, float size, std::function<int(glm::vec3)> f ){
        sX = x;
        sY = y;
        sZ = z;
        size = size;
        container = VoxelContainer(sX, sY, sZ);
        voxelize(f);

        //std::cout << "points size: " << mesh.triangles.size() << std::endl;
        //std::cout << "tri size: " << mesh.triangles.size() << std::endl;
    }

};