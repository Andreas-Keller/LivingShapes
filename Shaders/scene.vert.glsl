#version 300 es

#undef mediump	//very ugly hack due to qt shader manipulation
#undef highp

precision mediump float;	

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;

out vec2 vUv;
out vec4 vColor;

uniform mat4 M; //Model Matrix
uniform mat4 O; //Orfthonormal matrix
uniform mat4 V; //View matrix

uniform vec3 color;

void main(void)
{
    gl_Position = O * V * M * vec4(position, 1.0);
    vColor = vec4(color, 0.5f);
    vUv = uv;
}
