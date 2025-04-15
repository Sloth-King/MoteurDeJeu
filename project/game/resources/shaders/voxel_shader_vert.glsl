#version 330 core

layout(location = 0) in vec3 vertices_position_modelspace;
layout(location = 1) in vec2 raw_uv;
layout(location = 2) in vec3 raw_normals;

uniform mat4 Model;
uniform mat4 MVP;

out vec2 uv;
out vec3 normal;

void main(){

        uv = raw_uv;
        mat4 norm_transform = transpose(inverse(Model));
        normal = (norm_transform * vec4(raw_normals, 0)).xyz;


        vec4 pos = vec4(vertices_position_modelspace,1);

        gl_Position = MVP * pos;
}