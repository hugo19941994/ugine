uniform sampler2D texSampler;
varying vec2 ftex;
uniform bool isTexturized;

void main() {
    if (isTexturized) {
	    gl_FragColor = texture2D(texSampler, ftex);
	} else {
		gl_FragColor = vec4(1, 1, 1, 1);
    }
}
