#version 300 es

#undef mediump	//very ugly hack due to qt shader manipulation
#undef highp

precision mediump float;

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

out vec4 vColor;

uniform mat4 O; //Orfthonormal matrix
uniform mat4 V; //View matrix

void main(void)
{
    gl_Position = O * V * vec4(position, 1.0);
    vColor = vec4(color, 0.8f);
}
