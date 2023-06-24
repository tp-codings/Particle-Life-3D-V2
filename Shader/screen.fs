#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

//Offset um Fragment
const float offset = 1.0 / 300.0;

vec3 col = vec3(0.0);
vec3 color = vec3(0.0);

uniform int choice;

void main()
{
	vec2 offsets[9] = vec2[](
		vec2(-offset, offset),
		vec2(0.0f, offset),
		vec2(offset, offset),
		vec2(-offset, 0.0f),
		vec2(0.0f, 0.0f),
		vec2(offset, 0.0f),
		vec2(-offset, -offset),
		vec2(0.0f, -offset),
		vec2(offset, -offset)
	);

	float sharpnessKernel[9] = float[](
		-1, -1, -1,
		-1,  9, -1, 
		-1, -1, -1
	);

	float blurKernel[9] = float[](
		1.0 / 16, 2.0 / 16, 1.0 / 16,
		2.0 / 16, 4.0 / 16, 2.0 / 16,
		1.0 / 16, 2.0 / 16, 1.0 / 16
	);

	float edgeDetectionKernel[9] = float[](
		1,  1, 1,
		1, -8, 1, 
		1,  1, 1
	);

	vec3 sampleTex[9];

	for(int i= 0; i<9; i++)
	{
		sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
	}


	switch(choice)
	{
		case 0:
		//Sharpness
			for(int i = 0; i<9; i++)
				col += sampleTex[i] * sharpnessKernel[i];
			break;
		case 1:
		//Blur
			for(int i = 0; i<9; i++)
				col += sampleTex[i] * blurKernel[i];
			break;
		case 2:
		//Edge detection
			for(int i = 0; i<9; i++)
				col += sampleTex[i] * edgeDetectionKernel[i];
			break;
		case 3:
			//Inversion
			col = vec3(vec3(1.0 - texture(screenTexture, TexCoords)));
			break;
		case 4:
			//Grayscale
			col = vec3(texture(screenTexture, TexCoords));
			FragColor = vec4(col, 1.0);
			float average = 0.2126 * FragColor.r + 0.7152 * FragColor.g + 0.0722 * FragColor.b;
			col = vec3(average, average, average);
			break;
		default:
			col = vec3(texture(screenTexture, TexCoords));
			break;
	}


	FragColor = vec4(col, 1.0);
}
