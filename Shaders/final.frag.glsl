#version 300 es

#undef mediump	//very ugly hack due to qt shader manipulation

precision mediump float;	

in vec2 vUv;

out vec4 color;

uniform sampler2D sceneTex;
uniform sampler2D lightTex;

void main(void)
{
    vec4 lightColor = texture2D(lightTex, vUv);
    vec4 objectColor = texture2D(sceneTex, vUv);

    color = objectColor * lightColor + objectColor * 0.1f + lightColor * 0.1f;
}
