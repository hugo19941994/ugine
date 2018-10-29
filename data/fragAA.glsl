#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D currentTexture;
uniform sampler2D historyAATexture;
uniform sampler2D motionTexture;


#define KERNEL_SIZE 9
const float step_w = 1.0 / 1920;
const float step_h = 1.0 / 1080;
const vec2 offset[KERNEL_SIZE] = vec2[KERNEL_SIZE](
			vec2(-step_w, -step_h), vec2(0.0, -step_h), vec2(step_w, -step_h),
			vec2(-step_w, 0.0), vec2(0.0, 0.0), vec2(step_w, 0.0),
			vec2(-step_w, step_h), vec2(0.0, step_h), vec2(step_w, step_h)
);

void main() { 
	vec2 historyVel = texture(motionTexture, TexCoords).xy;
	vec2 uvLast = TexCoords + historyVel;

	vec4 maxVal = vec4(0.0f, 0.0f, 0.0f, 0.0f);
	vec4 minVal = vec4(1.0f, 1.0f, 1.0f, 0.0f);

	for (int i = 0; i < KERNEL_SIZE; ++i) {
		vec4 tmp = texture2D(historyAATexture, uvLast.xy + offset[i]);
		maxVal = max(tmp, maxVal);
		minVal = min(tmp, minVal);
	}

	vec4 color = mix(texture(currentTexture, TexCoords), clamp(texture(historyAATexture, uvLast),minVal, maxVal), 0.60f);

	FragColor = color;
}