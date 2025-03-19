#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <string>
#include <array>
#include <utility>
#include <vector>
#include <GL/glew.h>

using TRI_IDX_TYPE = unsigned int; // change both if needed!
extern GLuint TRI_GL_TYPE; // defined in mesh.cpp

struct Triangle {
    inline Triangle () {
        v[0] = v[1] = v[2] = 0;
    }
    inline Triangle (const Triangle & t) {
        v[0] = t.v[0];   v[1] = t.v[1];   v[2] = t.v[2];
    }
    inline Triangle (TRI_IDX_TYPE v0, TRI_IDX_TYPE v1, TRI_IDX_TYPE v2) {
        v[0] = v0;   v[1] = v1;   v[2] = v2;
    }
    TRI_IDX_TYPE & operator [] (unsigned int iv) { return v[iv]; }
    TRI_IDX_TYPE operator [] (unsigned int iv) const { return v[iv]; }
    inline Triangle & operator = (const Triangle & t) {
        v[0] = t.v[0];   v[1] = t.v[1];   v[2] = t.v[2];
        return (*this);
    }
    // membres :
    std::array<TRI_IDX_TYPE, 3> v;
};


class Mesh {
protected:

    // GPU, VBO etc
    void unsynchronize();
    void synchronize();

    void debug_draw();

    bool _synchronized = false;
    GLuint _VBO;
    GLuint _VAO; // https://stackoverflow.com/questions/21652546/what-is-the-role-of-glbindvertexarrays-vs-glbindbuffer-and-what-is-their-relatio
    GLuint _EBO;
    GLuint _UV;

public:


    GLuint shaderPID; // public for now.
    glm::mat4 transform = glm::mat4(1.0f);
    std::vector< glm::vec3 > vertices;
    std::vector< glm::vec2 > uvs;
    std::vector< glm::vec3 > normals;
    std::vector< Triangle > triangles;
    std::vector< glm::vec3 > colors;

    //TODO Add textures eventually
    //std::vector< std::pair<Texture, std::string> > textures;

    static Mesh gen_tesselatedSquare(int nX, int nY, float sX = 1, float sY = 1);

    Mesh() = default;
    ~Mesh(){
        if (_synchronized){
            unsynchronize();
        }
    }

    void recomputeNormals();

    void setShader(std::string vertex_shader, std::string fragment_shader);
    void render(glm::mat4 vpMatrix);

    void rotate(float v, glm::vec3 axis){
        transform = glm::rotate(transform, v, axis);
    }

    //TODO Add textures eventually.
    // void addTexture(Texture tex, std::string name_in_shader){
    //     textures.push_back( std::pair(tex, name_in_shader));
    // }


    friend std::ostream& operator<< (std::ostream& stream, const Mesh& mesh) {
            stream << "Mesh(" << mesh.vertices.size() <<" verts, " << mesh.triangles.size() <<" tris)" <<
            " at position (" << mesh.transform[3][0] << ", " << mesh.transform[3][1] << ", " << mesh.transform[3][2] << ")";
        
        return stream;
    }
};