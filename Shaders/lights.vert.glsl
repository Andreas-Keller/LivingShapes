/* simple shader to draw light textures into a framebuffer */

#version 300 es

#undef mediump	//very ugly hack due to qt shader manipulation

precision mediump float;	

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;

out vec2 vUv;

uniform mat4 M; //Model Matrix
uniform mat4 O; //Orfthonormal matrix
uniform mat4 V; //View matrix

void main(void)
{
    gl_Position = O * V * M * vec4(position, 1.0);
    vUv = uv;
}
