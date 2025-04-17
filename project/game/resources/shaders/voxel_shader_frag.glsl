#version 460 core

uniform vec3 view;


const int atlas_nb_textures = 8;
const int atlas_size_textures = 8;

uniform sampler2D atlas;

in vec2 uv;
in vec3 normal;

out vec3 color;
void main(){

    color = texture(
        atlas,
        uv
        ).xyz * clamp(dot(view, normal)+0.2, 0.0, 1.0);
}