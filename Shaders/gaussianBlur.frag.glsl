#version 300 es

#undef mediump
#undef highp

precision mediump float;
precision mediump int;

in vec2		fUV;
out vec4	color;

uniform sampler2D image;
uniform bool horizontal;

float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

void main() {
	ivec2 size = textureSize(image, 0);
	vec2 offset = vec2(1.0f / float(size.x), 1.0f / float(size.y)); //size of a texel
	vec3 result = texture2D(image, fUV).rgb * weight[0];

	if (horizontal) {
		for (int i = 1; i < 5; ++i) {
			result += texture(image, fUV + vec2(offset.x * float(i), 0.0)).rgb * weight[i];
			result += texture(image, fUV - vec2(offset.x * float(i), 0.0)).rgb * weight[i];
		}
	}
	else {
		for (int i = 1; i < 5; ++i) {
			result += texture(image, fUV + vec2(0.0, offset.y * float(i))).rgb * weight[i];
			result += texture(image, fUV - vec2(0.0, offset.y * float(i))).rgb * weight[i];
		}
	}

	color = vec4(result, 1.0f);
}
