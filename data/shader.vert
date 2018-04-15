attribute vec3 vpos;
attribute vec2 vtex;
varying vec2 ftex;
uniform mat4 MVP;

void main() {
	gl_Position = MVP * vec4(vpos, 1);
	ftex = vtex;
}