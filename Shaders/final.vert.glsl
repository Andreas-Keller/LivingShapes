#version 300 es

#undef mediump	//very ugly hack due to qt shader manipulation

precision mediump float;	

layout (location = 0) in vec3 position;
layout (location = 1) in  vec2 uv;

out vec2 vUv;

void main(void)
{
    gl_Position = vec4(position, 1.f);
    vUv = uv;
}
