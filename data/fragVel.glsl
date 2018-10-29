#version 330 core
uniform sampler2D depthTexture;

in vec2 TexCoords;
uniform mat4 VP;
uniform mat4 PrevVP;

out vec4 velocity;

void main() {
	/*
	float zOverW = texture2D(depthTexture, TexCoords).r * 2.0f - 1.0f;

	vec4 H = vec4(TexCoords.x * 2.0f - 1.0f, (1.0f - TexCoords.y) * 2.0f - 1.0f, zOverW, 1.0f);

	vec4 D = H * inverse(VP);

	// Original position
	vec4 worldPos = D / D.w;

	// Prev. position
	vec4 currentPos = H;
	vec4 previousPos = worldPos * PrevVP;
	previousPos /= previousPos.w;

		
	velocity = vec4((currentPos.xy - previousPos.xy) / 2.0f, 0.0f, 1.0f);
	*/
	velocity = vec4(0.0f, 0.0f, 0.0f, 1.0f);
}

