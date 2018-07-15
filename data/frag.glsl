// Regular texture
uniform sampler2D texSampler;
uniform bool isTexturized;

// Cubemap texture
uniform samplerCube cubeTexSampler;
uniform bool isCube;

// Normal texture
uniform sampler2D normalTex;
uniform bool hasNormalTex;

// Reflection texture
uniform samplerCube reflectionTex;
uniform bool hasReflectionTex;

// Refraction texture
uniform samplerCube refractionTex;
uniform bool hasRefractionTex;

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
varying mat3 TBN;
varying vec3 f_vertexCoords;

void main() {
	vec3 uvw = normalize(f_vertexCoords);

	// Vector N
	vec3 N = fnormal;
	if (hasNormalTex) {
        vec3 texNormal = texture(normalTex, ftex).rgb;
		N = normalize(texNormal * 2.0 - 1.0); //change range
		N = normalize(TBN * N);
	}

	// Contributions
	vec3 diffuseContrib = ambientLight;
	float specularContrib = 0;

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
		if (!isCube) {
			finalColor *= texture2D(texSampler, ftex);
		}
		else {
			finalColor *= textureCube(cubeTexSampler, uvw);
		}
	}

	finalColor *= matColor * vec4(diffuseContrib, 1.0);

	vec4 specularVec = vec4(specularContrib, specularContrib, specularContrib, 0);
	finalColor += specularVec;

	if (hasReflectionTex) {
		vec4 reflectionColor = textureCube(reflectionTex, uvw);
		finalColor = vec4(mix(finalColor.rgb, reflectionColor.rgb, reflectionColor.a), 1.0f);
	}

	if (hasRefractionTex) {
		vec4 refractionColor = textureCube(refractionTex, uvw);
		finalColor = vec4(mix(finalColor.rgb, refractionColor.rgb, refractionColor.a), 1.0f);
	}

	gl_FragColor = finalColor;
}