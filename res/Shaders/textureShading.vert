#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec3 tangent;
out vec2 uv0;
out vec3 normal0;
out vec3 tangent0;
out vec4 worldPos0;

uniform mat4 modelMatrix;
uniform mat4 MVP;

void main()
{
	gl_Position = MVP * vec4(position,1);
	uv0=vec2(uv.x, 1.0f - uv.y); //uv;
	normal0 = (modelMatrix * vec4(normal,0)).xyz;
	tangent0 = (modelMatrix * vec4(tangent,0)).xyz;
	worldPos0 = (modelMatrix * vec4(position,1));
}