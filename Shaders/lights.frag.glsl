varying vec2 vUv;

uniform sampler2D image;

void main(void)
{
    gl_FragColor = texture2D(image, vUv);
}
