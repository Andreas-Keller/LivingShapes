#version 300 es

#undef mediump	//very ugly hack due to qt shader manipulation

precision mediump float;
	
in vec4 vColor;
in vec2 vUv;

out vec4 color;

void main(void)
{
    color = vColor;
}
