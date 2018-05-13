uniform sampler2D texSampler;
uniform bool isTexturized;

uniform int matShine;
uniform vec4 matColor;
uniform vec3 ambientLight;
uniform int numLights;
uniform vec4 direction[8];
uniform vec3 color[8];
uniform float linAtt[8];

varying vec2 ftex;
varying vec3 fnormal;
varying vec3 fpos;

void main() {
	// Contributions
	vec3 diffuseContrib = ambientLight;
	float specularContrib = 0;

	// Vector N
	vec3 N = fnormal;

	for (int i = 0; i < numLights; i++) {
		// Attenuation
		float att = 1;
		float cos_eta = 0;

		// Vector L
		vec3 L = normalize(direction[i].xyz);

		// Point light
		if (direction[i].w == 1.0) {
			L = L - fpos;
			att = 1 / (1 + linAtt[i] * length(L));
		}

		// Normalize L
		L = normalize(L);

		// Dot product N L (MUST BE POSITIVE use max)
		cos_eta = max(0, dot(N, L));

		// Diffuse = Dot(NL) * color * att
		diffuseContrib += cos_eta * color[i] * att;

		// Specular
		if (cos_eta > 0 && matShine > 0) {
			vec3 H = normalize(L - normalize(fpos));
			float NdotH = max(0, dot(N, H));
			if (NdotH > 0) {
				specularContrib += pow(NdotH, matShine) * att;
			}
		}
	}

	vec4 finalColor = vec4(1, 1, 1, 1);

	// Combine color with texture
	if (isTexturized) {
		finalColor *= texture2D(texSampler, ftex);
	}
	finalColor *= matColor * vec4(diffuseContrib, 1.0);

	vec4 specularVec = vec4(specularContrib, specularContrib, specularContrib, 0);
	finalColor += specularVec;

	gl_FragColor = finalColor;
}
