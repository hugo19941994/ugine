#version 330 core
out vec4 FragColor;
in vec2 TexCoords;
uniform sampler2D finalTexture;

#define KERNEL_SIZE 9
const float kernel[KERNEL_SIZE] = float[KERNEL_SIZE](
				0.0, -1.0 , 0.0 ,
				-1.0 , 5.0 , -1.0 ,
				0.0 , -1.0 , 0.0 
);
const float step_w = 1.0 / 1920;
const float step_h = 1.0 / 1080;
const vec2 offset[KERNEL_SIZE] = vec2[KERNEL_SIZE](
			vec2(-step_w, -step_h), vec2(0.0, -step_h), vec2(step_w, -step_h),
			vec2(-step_w, 0.0), vec2(0.0, 0.0), vec2(step_w, 0.0),
			vec2(-step_w, step_h), vec2(0.0, step_h), vec2(step_w, step_h)
);

void main(void)
{
	/*
	int i = 0;
	vec4 sum = vec4(0.0);

	vec4 tmp = texture2D(finalTexture, TexCoords.xy + offset[0]);
	sum += tmp * kernel[0];
	tmp = texture2D(finalTexture, TexCoords.st + offset[1]);
	sum += tmp * kernel[1];
	tmp = texture2D(finalTexture, TexCoords.st + offset[2]);
	sum += tmp * kernel[2];
	tmp = texture2D(finalTexture, TexCoords.st + offset[3]);
	sum += tmp * kernel[3];
	tmp = texture2D(finalTexture, TexCoords.st + offset[4]);
	sum += tmp * kernel[4];
	tmp = texture2D(finalTexture, TexCoords.st + offset[5]);
	sum += tmp * kernel[5];
	tmp = texture2D(finalTexture, TexCoords.st + offset[6]);
	sum += tmp * kernel[6];
	tmp = texture2D(finalTexture, TexCoords.st + offset[7]);
	sum += tmp * kernel[7];
	tmp = texture2D(finalTexture, TexCoords.st + offset[8]);
	sum += tmp * kernel[8];

	sum = clamp(sum, 0.0, 1.0);

	FragColor = sum;

	*/
	FragColor = texture(finalTexture, TexCoords);
}


