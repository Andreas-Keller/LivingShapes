attribute vec3 position;
attribute vec3 color;

varying vec4 vcolor;

uniform mat4 M; //Model Matrix
uniform mat4 O; //Orfthonormal matrix
uniform mat4 V; //View matrix

void main(void)
{
    gl_Position = O * V * M * vec4(position, 1.0);
    vcolor = vec4(color, 0.5f);
}
