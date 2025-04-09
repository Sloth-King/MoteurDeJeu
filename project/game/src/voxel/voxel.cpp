
#include "voxel.h"

#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))

#define SET_BIT(var,pos) (var = (var) | (1<<(pos)))


void add_voxel(Mesh & mesh, glm::vec3 pos, glm::vec3 size, int face_mask){
    if (face_mask <= 0) return;

    int v = mesh.vertices.size();
    mesh.vertices.push_back( pos - glm::vec3(size[0.0f]/2.0f, 0.0f, 0.0f) - glm::vec3(0.0f, size[1]/2.0f, 0.0f) - glm::vec3(0.0f, 0.0f, size[2]/2.0f) ); //front bottom left
    mesh.vertices.push_back( pos + glm::vec3(size[0.0f]/2.0f, 0.0f, 0.0f) - glm::vec3(0.0f, size[1]/2.0f, 0.0f) - glm::vec3(0.0f, 0.0f, size[2]/2.0f) ); //front bottom right

    mesh.vertices.push_back( pos - glm::vec3(size[0.0f]/2.0f, 0.0f, 0.0f) + glm::vec3(0.0f, size[1]/2.0f, 0.0f) - glm::vec3(0.0f, 0.0f, size[2]/2.0f) ); //front top left
    mesh.vertices.push_back( pos + glm::vec3(size[0.0f]/2.0f, 0.0f, 0.0f) + glm::vec3(0.0f, size[1]/2.0f, 0.0f) - glm::vec3(0.0f, 0.0f, size[2]/2.0f) ); //front top right
    
    mesh.vertices.push_back( pos - glm::vec3(size[0.0f]/2.0f, 0.0f, 0.0f) - glm::vec3(0.0f, size[1]/2.0f, 0.0f) + glm::vec3(0.0f, 0.0f, size[2]/2.0f) ); //back bottom left
    mesh.vertices.push_back( pos + glm::vec3(size[0.0f]/2.0f, 0.0f, 0.0f) - glm::vec3(0.0f, size[1]/2.0f, 0.0f) + glm::vec3(0.0f, 0.0f, size[2]/2.0f) ); //back bottom right

    mesh.vertices.push_back( pos - glm::vec3(size[0.0f]/2.0f, 0.0f, 0.0f) + glm::vec3(0.0f, size[1]/2.0f, 0.0f) + glm::vec3(0.0f, 0.0f, size[2]/2.0f) ); //back bottom left
    mesh.vertices.push_back( pos + glm::vec3(size[0.0f]/2.0f, 0.0f, 0.0f) + glm::vec3(0.0f, size[1]/2.0f, 0.0f) + glm::vec3(0.0f, 0.0f, size[2]/2.0f) ); //back bottom right

    //face front
    if (CHECK_BIT(face_mask, 0)){
        mesh.triangles.push_back( Triangle(v+1, v+0, v+2) ); mesh.triangles.push_back( Triangle(v+1, v+2, v+3) );
    }
    if (CHECK_BIT(face_mask, 1)){
        // back
        mesh.triangles.push_back( Triangle(v+4, v+5, v+7) ); mesh.triangles.push_back( Triangle(v+4, v+7, v+6) );
    }
    if (CHECK_BIT(face_mask, 2)){
        // right
        mesh.triangles.push_back( Triangle(v+5, v+1, v+3) ); mesh.triangles.push_back( Triangle(v+5, v+3, v+7) );
    }
    if (CHECK_BIT(face_mask, 3)){
        // left
        mesh.triangles.push_back( Triangle(v+0, v+4, v+6) ); mesh.triangles.push_back( Triangle(v+0, v+6, v+2) );

    }
    if (CHECK_BIT(face_mask, 4)){
        // top
        mesh.triangles.push_back( Triangle(v+3, v+2, v+6) ); mesh.triangles.push_back( Triangle(v+3, v+6, v+7) );
    }
    if (CHECK_BIT(face_mask, 5)){
        // bottom
        mesh.triangles.push_back( Triangle(v+5, v+4, v+1) ); mesh.triangles.push_back( Triangle(v+4, v+0, v+1) );
    }


    //normals (un peu dirty, mais bon!)
    /*
    mesh.normals.push_back( glm::normalize(-1.0f* glm::vec3(size[0.0f]/2.0f, 0.0f, 0.0f) - glm::vec3(0.0f, size[1]/2.0f, 0.0f) - (glm::vec3(0.0f, 0.0f, size[2]/2.0f))) ); //front bottom left
    mesh.normals.push_back( glm::normalize(glm::vec3(size[0.0f]/2.0f, 0.0f, 0.0f) - glm::vec3(0.0f, size[1]/2.0f, 0.0f) - (glm::vec3(0.0f, 0.0f, size[2]/2.0f))) ); //front bottom right

    mesh.normals.push_back( glm::normalize(-1.0f*  glm::vec3(size[0.0f]/2.0f, 0.0f, 0.0f) + glm::vec3(0.0f, size[1]/2.0f, 0.0f) - (glm::vec3(0.0f, 0.0f, size[2]/2.0f))) ); //front top left
    mesh.normals.push_back( glm::normalize(pos + glm::vec3(size[0.0f]/2.0f, 0.0f, 0.0f) + glm::vec3(0.0f, size[1]/2.0f, 0.0f) - (glm::vec3(0.0f, 0.0f, size[2]/2.0f))) ); //front top right
    
    mesh.normals.push_back( glm::normalize(-1.0f*  glm::vec3(size[0.0f]/2.0f, 0.0f, 0.0f) - glm::vec3(0.0f, size[1]/2.0f, 0.0f) + (glm::vec3(0.0f, 0.0f, size[2]/2.0f))) ); //back bottom left
    mesh.normals.push_back( glm::normalize(glm::vec3(size[0.0f]/2.0f, 0.0f, 0.0f) - glm::vec3(0.0f, size[1]/2.0f, 0.0f) +(glm::vec3(0.0f, 0.0f, size[2]/2.0f))) ); //back bottom right

    mesh.normals.push_back( glm::normalize(-1.0f*  glm::vec3(size[0.0f]/2.0f, 0.0f, 0.0f) + glm::vec3(0.0f, size[1]/2.0f, 0.0f) + (glm::vec3(0.0f, 0.0f, size[2]/2.0f))) ); //back bottom left
    mesh.normals.push_back( glm::normalize(glm::vec3(size[0.0f]/2.0f, 0.0f, 0.0f) + glm::vec3(0.0f, size[1]/2.0f, 0.0f) +(glm::vec3(0.0f, 0.0f, size[2]/2.0f))) ); //back bottom right
    */
    mesh.uvs.push_back( glm::vec2(0.0f, 0.0f)); //front bottom left
    mesh.uvs.push_back( glm::vec2(1.0f, 0.0f) ); //front bottom right

    mesh.uvs.push_back( glm::vec2(0.0f, 1.0f) ); //front top left
    mesh.uvs.push_back( glm::vec2(1.0f, 1.0f) ); //front top right
    
    mesh.uvs.push_back( glm::vec2(1.0f, 0.0) ); //back bottom left
    mesh.uvs.push_back( glm::vec2(0.0f, 0.0f) ); //back bottom right

    mesh.uvs.push_back( glm::vec2(1.0f, 1.0f) ); //back top left
    mesh.uvs.push_back( glm::vec2(0.0f, 1.0f) ); //back top right
}



void C_voxelMesh::voxelize(std::function<int(glm::vec3, float)> f){

    glm::vec3 AABB_v1 = -glm::vec3(sX, sY, sZ) * size/2.0f;
    glm::vec3 AABB_v2 = glm::vec3(sX, sY, sZ) * size/2.0f;

    mesh = Mesh();

    //create grid 
    for (int i = 0; i < sX; ++i){
        for (int j = 0; j < sY; ++j){
            for (int k = 0; k < sZ; ++k){

                container.set(i, j, k,
                    f(glm::vec3(i, j, k) / size, size)
                );
            }
        }
    }

    //simplify
    for (int i = 0; i < sX; ++i){
        for (int j = 0; j < sY; ++j){
            for (int k = 0; k < sZ; ++k){
                
                int mask = 0;

                if (container.get(i, j, k) > 0){
                    
                    // si la case d'avant en x est vide
                    if (i == 0 || ( i > 0 && container.get(i-1, j, k) == 0)) SET_BIT(mask, 3);

                    // si la case d'avant en y est vide
                    if(j == 0 || ( j > 0 && container.get(i, j-1, k) == 0)) SET_BIT(mask, 5);

                    // si la case d'avant en z est vide
                    if (k== 0 || ( k > 0 && container.get(i, j, k-1)) == 0) SET_BIT(mask, 0);

                    // si la case d'après en x est vide
                    if (i == sX-1 || ( i < sX && container.get(i+1, j, k) == 0)) SET_BIT(mask, 2);

                    // si la case d'après en y est vide
                    if (j == sY-1 || ( j < sY && container.get(i, j+1, k) == 0)) SET_BIT(mask, 4);

                    // si la case d'après en z est vide
                    if (k == sZ-1 || ( k < sZ && container.get(i, j, k+1) == 0)) SET_BIT(mask, 1);

                    // TODO check if we could remove points depending on those values and others around them.

                    // create the voxel

                    glm::vec3 pos = AABB_v1 + glm::vec3(
                        size * i,
                        size * j,
                        size * k
                    );
                    add_voxel(mesh, pos, glm::vec3(size, size, size), mask);
                }
            }
        }
    }

    mesh.recomputeNormals();
}
