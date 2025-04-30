#version 460 core

layout(location = 0) in vec3 vertices_position_modelspace;
layout(location = 1) in vec2 raw_uv;
layout(location = 2) in vec3 raw_normals;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;

uniform mat4 Model;
uniform mat4 MVP;

out vec2 uv;
out vec3 normal;
out mat3 TBN;

void main(){

        uv = raw_uv;
        mat4 norm_transform = transpose(inverse(Model));
        normal = normalize((norm_transform * vec4(raw_normals, 0)).xyz);
        
        vec4 pos = vec4(vertices_position_modelspace,1);

        gl_Position = MVP * pos;

        //TBN matrix
        vec3 T = normalize(vec3(Model * vec4(aTangent,   0.0)));
        vec3 B = normalize(vec3(Model * vec4(aBitangent, 0.0)));
        vec3 N = normalize(vec3(Model * vec4(normal,    0.0)));
        mat3 TBN = mat3(T, B, N);
}