varying vec2 vUv;
varying vec3 vColor;

uniform sampler2D image;

void main(void)
{
    gl_FragColor = texture2D(image, vUv);
}
