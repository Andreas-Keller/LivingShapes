//we break mobile compatibility here:
#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

out vec4 vcolor;

void main(void)
{
    gl_Position = vec4(position, 1.0);
    vcolor = vec4(color, 1.0);
}
