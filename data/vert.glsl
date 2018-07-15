attribute vec3 vpos;
attribute vec2 vtex;
attribute vec3 vnormal;
attribute vec3 vtangent;

uniform mat4 MVP;
uniform mat4 normalMat;
uniform mat4 MV;
uniform mat4 M;
uniform vec3 eyePos;

uniform bool hasReflectionTex;
uniform bool hasRefractionTex;
uniform float refractionCoef;

varying vec3 fnormal;
varying vec3 fpos;
varying vec2 ftex;
varying mat3 TBN;
varying vec3 f_vertexCoords;

void main() {
	gl_Position = MVP * vec4(vpos, 1);
	ftex = vtex;
	fnormal = normalize((normalMat * vec4(vnormal, 0.0)).xyz);
	fpos = vec3(MV * vec4(vpos,1));

	// tangent
	vec3 tangent = (normalMat * vec4(vtangent, 1.0)).xyz;
	vec3 bitan = cross(fnormal, tangent);
	TBN = transpose(mat3(tangent, bitan, fnormal));

	// Reflection, refraction
	vec3 eye = normalize(vec3(M * vec4(vpos, 1)) - eyePos);
	vec3 normal = vec3(M * vec4(vnormal, 0));
	if (hasReflectionTex) {
		f_vertexCoords = reflect(eye, normal);
	} else if (hasRefractionTex) {
		f_vertexCoords = refract(eye, normal, refractionCoef);
	} else {
		f_vertexCoords = vpos;
	}
}