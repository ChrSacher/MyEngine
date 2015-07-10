#version 330 core

const int Max_Instances  = 1000;
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 normal;

varying vec2 uv0;
varying vec3 normal0;
varying vec4 worldPos0;
varying vec4 viewworldPos0;

uniform mat4 modelMatrix[Max_Instances];
uniform mat4 MVP[Max_Instances];
void main()
{
	gl_Position = MVP[gl_InstanceID] * vec4(position,1);
	viewworldPos0 = gl_Position;
	uv0=vec2(uv.x, 1.0f - uv.y); //uv;
	normal0 = (modelMatrix[gl_InstanceID] * vec4(normal,0)).xyz;
	worldPos0 = (modelMatrix[gl_InstanceID] * vec4(position,1));
}