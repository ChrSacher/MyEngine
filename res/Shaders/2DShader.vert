#version 330

attribute vec2 position;
attribute vec2 uv;

varying vec2 uv0;
uniform mat4 ortho;
void main()
{
	gl_Position = ortho * vec4(position,0.0f,1.0f);
	gl_Position.z = 0.0f;
	//gl_Position.w = 1.0f;
	uv0=uv;
}