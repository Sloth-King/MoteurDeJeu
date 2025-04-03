#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertices_position_modelspace;
layout(location = 1) in vec2 aTex;

out vec2 texCoord;
out float y;

uniform mat4 MVP;
uniform sampler2D h;
uniform float heightScale;

// Values that stay constant for the whole mesh.


void main(){

        float height =  texture(h, aTex).r; //heightscale replacement
        //height = 1.0;

        //Generating the plane with heightmap in the shader
        //vec3 new_position = vec3(vertices_position_modelspace.x, height , vertices_position_modelspace.y); 

        //Generating the plane with heightmap in the cpu
        vec3 new_position = vertices_position_modelspace;

        gl_Position = MVP * vec4(new_position, 1.0);

        texCoord = aTex;
        y = height;
}
