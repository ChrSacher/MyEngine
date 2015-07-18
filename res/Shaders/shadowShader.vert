#version 330
uniform mat4 depthVP;
uniform mat4 modelMatrix[10];
layout(location = 0) in vec3 position;

void main(void)
{
    gl_Position =  depthVP * modelMatrix * vec4(position,1);
}