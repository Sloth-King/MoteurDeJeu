#version 330 core

in vec2 uv;

uniform sampler2D rock;
uniform sampler2D grass;
uniform sampler2D snow;

in float slopeness;
in float height;

// Ouput data
out vec3 color;

void main(){

        color =vec3(uv, 0);

        if (height < 0.2){


                color = mix(texture(grass, uv).rgb, texture(rock, uv).rgb, slopeness);
        }
        else {
                color = mix(texture(grass, uv).rgb, texture(rock, uv).rgb, slopeness);
        }

}
