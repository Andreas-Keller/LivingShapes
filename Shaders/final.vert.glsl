attribute vec3 position;
attribute vec3 color;
attribute vec2 uv;

varying vec2 vUv;
varying vec3 vColor;

void main(void)
{
    gl_Position = vec4(position, 1.f);
    vUv = uv;
    vColor = color;
}
