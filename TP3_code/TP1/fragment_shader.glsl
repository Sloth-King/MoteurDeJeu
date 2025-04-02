#version 330 core

// Ouput data
out vec3 color;
in vec2 uv;

uniform sampler2D mesh_texture;


void main(){   
        color = texture(mesh_texture, uv).rgb;
}