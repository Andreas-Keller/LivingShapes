attribute vec3 position;
attribute vec3 color;
attribute vec2 uv;

varying vec2 vUv;
varying vec4 vColor;

uniform mat4 M; //Model Matrix
uniform mat4 O; //Orfthonormal matrix
uniform mat4 V; //View matrix

void main(void)
{
    gl_Position = O * V * M * vec4(position, 1.0);
    vColor = vec4(color, 0.5f);
    vUv = uv;
}
