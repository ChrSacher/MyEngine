#version 330

layout(location = 0) in vec3 pos;
layout(location = 1) in vec4 color;

out vec4 outColor;
varying vec3 worldPos0;
uniform mat4 viewMatrix;
uniform vec3 eyePos;

void main()
{
	gl_Position = viewMatrix * vec4(pos,1);
	outColor = color;

	float dist = length(worldPos0 - eyePos);
    float att = inversesqrt(0.1f*dist);
    gl_PointSize = 2.0f * att;
}