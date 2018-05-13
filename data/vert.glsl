attribute vec3 vpos;
attribute vec2 vtex;
attribute vec3 vnormal;

uniform mat4 MVP;
uniform mat4 normalMat;
uniform mat4 MV;

varying vec3 fnormal;
varying vec3 fpos;
varying vec2 ftex;

void main() {
	gl_Position = MVP * vec4(vpos, 1);
	ftex = vtex;
	fnormal = normalize((normalMat * vec4(vnormal, 0.0)).xyz);
	fpos = vec3(MV * vec4(vpos,1));
}