//we break mobile compatibility here:
#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

out vec4 vcolor;

uniform mat4 M; //Model Matrix
uniform mat4 O; //Orfthonormal matrix
uniform mat4 V; //View matrix

void main(void)
{
    gl_Position = O * V * M * vec4(position, 1.0);
    vcolor = vec4(color, 1.0);
}
