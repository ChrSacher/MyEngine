#version 330
//The fragment shader operates on each pixel in a given polygon

varying vec3 worldPos0;
in vec4 outColor;

void main()
{
	gl_FragColor = outColor ;
}