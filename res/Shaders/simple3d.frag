#version 330 core
//The fragment shader operates on each pixel in a given polygon

uniform vec3 color;

void main()
{

	gl_FragColor = color;
	
}