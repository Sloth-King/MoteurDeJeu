#version 330 core

// Ouput data
out vec3 color;

in vec2 texCoord;
in float y;

uniform sampler2D grass;
uniform sampler2D rock;
uniform sampler2D snow;

uniform sampler2D h;


void main(){

        vec3 grass2 = texture(grass , texCoord).rgb;
        vec3 rock2 = texture(rock , texCoord).rgb;
        vec3 snow2 = texture(snow , texCoord).rgb;
        float grassThreshold = 0.1;
        float rockThreshold = 0.2;
        float snowThreshold = 0.3;

        vec3 grassRock = mix(grass2, rock2, smoothstep(grassThreshold, rockThreshold, y));
        vec3 rockSnow = mix(rock2, snow2, smoothstep(rockThreshold, snowThreshold, y));
        color = mix(grassRock, rockSnow, smoothstep(grassThreshold, snowThreshold, y));

        //color = texture(grass, texCoord).rgb;

        //for debug
        //color = texture(h , texCoord).rgb;
}