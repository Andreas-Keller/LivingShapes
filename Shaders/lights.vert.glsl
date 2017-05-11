/* simple shader to draw light textures into a framebuffer */

attribute vec3 position;
attribute vec2 uv;

varying vec2 vUv;

uniform mat4 M; //Model Matrix
uniform mat4 O; //Orfthonormal matrix
uniform mat4 V; //View matrix

void main(void)
{
    gl_Position = O * V * M * vec4(position, 1.0);
    vUv = uv;
}
