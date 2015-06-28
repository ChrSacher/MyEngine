#pragma once
#include <glew.h>
 
 
#include <GL/GLu.h>

#include <stdio.h>
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include "Math/Matrices.h" 
#include "Math/Vectors.h"
#include "Shader.h"
#include "Transform.h"

class Text
{
	Text()
	{
			glActiveTexture(GL_TEXTURE0);
			glGenTextures(1, &textureID);
			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
 
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glGenBuffers(1, &vbo);
			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	}
	~Text();
	Shader textShader;
	Vector4 Color;
	std::string text;
	float scaleX,scaleY,xStart,yStart;
	Transform position;
	GLuint textureID;
	GLuint vbo
	void renderText()
	{

	}
};