//precision mediump float;

varying vec2 vUv;
varying vec3 vColor;

uniform sampler2D sceneTex;
uniform sampler2D lightTex;

void main(void)
{
    vec4 lightColor = texture2D(lightTex, vUv);
    vec4 objectColor = texture2D(sceneTex, vUv);

	//gl_FragColor = lightColor;
    gl_FragColor = objectColor * lightColor + objectColor * 0.1f;
}
