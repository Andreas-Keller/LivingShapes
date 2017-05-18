#version 300 es

#undef mediump	//very ugly hack due to qt shader manipulation

precision mediump float;	

in vec2 vUv;

out vec4 color;

uniform sampler2D image;

void main(void)
{
    color = texture2D(image, vUv);
}
