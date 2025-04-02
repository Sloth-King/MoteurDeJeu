#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertices_position_modelspace;
layout(location = 1) in vec2 raw_uv;

out vec2 uv;
out float y;

uniform mat4 MVP;

// Values that stay constant for the whole mesh.


void main(){

        vec3 new_position = vec3(vertices_position_modelspace.x , vertices_position_modelspace.y , vertices_position_modelspace.z);

        gl_Position = MVP * vec4(new_position, 1.0);

        uv = raw_uv;
}

