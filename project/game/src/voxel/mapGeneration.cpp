#include "voxel.h"
#include "im3d.h"
#include "engine/external/FastNoiseLite.h"


const int seed = 15987; // here for now
FastNoiseLite baseTerrainNoise(seed);
FastNoiseLite sharpTerrainNoise(seed);
FastNoiseLite rugosityNoise(seed);

FastNoiseLite rockNoise(seed);


void setupNoise(){
    baseTerrainNoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2S);
    baseTerrainNoise.SetFrequency(0.02);

    sharpTerrainNoise.SetNoiseType(FastNoiseLite::NoiseType_Cellular);
    sharpTerrainNoise.SetCellularDistanceFunction(FastNoiseLite::CellularDistanceFunction_EuclideanSq);
    sharpTerrainNoise.SetFrequency(0.1);

    rockNoise.SetNoiseType(FastNoiseLite::NoiseType_Cellular);
    rockNoise.SetCellularDistanceFunction(FastNoiseLite::CellularDistanceFunction_EuclideanSq);
    rockNoise.SetFrequency(0.03);

    rugosityNoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2S);
    rugosityNoise.SetFrequency(0.01);
}

uint8_t generateVoxel(const glm::ivec3 & global_position){
    if (global_position.x % 16 == 0 || global_position.z % 16 == 0 ) return 0 ;
    const int fac = 5;

    float rugosity = rugosityNoise.GetNoise((float)global_position.x, (float)global_position.z);

    float h = baseTerrainNoise.GetNoise((float)global_position.x, (float)global_position.z) * fac;
    
    float rock = baseTerrainNoise.GetNoise((float)global_position.x, (float)global_position.z);

    h += sharpTerrainNoise.GetNoise((float)global_position.x, (float)global_position.z) * fac * 10 * pow(rugosity, 3);
    
    VOXEL_IDX_TYPE result = 0;

    // base terrain

    if (global_position.y < -50){
        result = 0;
    }

    if (rugosity < -0.65){

        if (global_position.y < h - rugosity * (-h / 3) - 1){
            result = 2;
        }

        if (global_position.y <  h - rugosity * 3 - 1){
            result = 3;
        }

        if (global_position.y < h - rock * 5 - 4){
            result = 2;
        }

        if (global_position.y < h - rock * rugosity * 7 - 20){
            result = 7;
        }


    } else if (rugosity < 0.65){

        if (global_position.y < h - rock * 10 - 13){
            result = 7;
        }
        else if (global_position.y < h - rugosity * 3 - 6){
            result = 2;
        }

        else if (global_position.y <  h - rock * 3 - 3){
            result = 3;
        }
        else if (global_position.y < h){
            result = 4;
        }
    } else {

        if (global_position.y < h - rugosity * (-h / 3) - 1){
            result = 2;
        }

        if (global_position.y <  h - rugosity * (-h / 2) - 1){
            result = 3;
        }

        if (global_position.y < h - rugosity * 3 - 7){
            result = 2;
        }

        if (global_position.y < h - rock * 10 - 13){
            result = 7;
        }

    }

    // rocks

    if (pow(rock, 7) > 0.15 && global_position.y > h- abs(rugosity) * 3 && global_position.y  < h + abs(rock) *2 ){
        result = 7;
    }

    //result = 6;

    return result;
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
    setupNoise();

    container = VoxelContainer(CHUNK_SIZE_XZ, CHUNK_SIZE_Y, CHUNK_SIZE_XZ);

    for (int k = 0; k < CHUNK_SIZE_Y; ++k){
        
        generateStrate(container, k, offset);
    }
}



void generateFrom3DTexture(VoxelContainer & container, std::string path, int dimX, int dimY, int dimZ, int threshold, int trueValue){
    

    Image im = read_img_file(path.c_str() , dimX , dimY , dimZ);

    saveHistogramData(im, path + "HISTO.dat", dimX, dimY, dimZ);

    container = VoxelContainer(dimX, dimY, dimZ);



    for(int i = 0 ; i < dimX * dimY * dimZ ; i++){
        auto v = (unsigned int)im.data[i];
        
        //on met à trueValue les valeurs au dessus du seuil et à 0 les autres
        container.data[i] = (v >= threshold)? trueValue : 0;
    }
}