#pragma once

#include <ft2build.h>
#include "Shader.h"
#include <map>
#include <vector>
#include FT_FREETYPE_H

struct Character 
{
    GLuint TextureID;   // ID handle of the glyph texture
    Vector2 Size;    // Size of glyph
	Vector2 Bearing;  // Offset from baseline to left/top of glyph
    GLuint Advance;    // Horizontal offset to advance to next glyph
};
struct TextData
{
	std::string text;
	GLfloat x,y,sx,sy;
	Vector3 color;
	GLfloat timetolive;
	TextData(std::string &Text, GLfloat X, GLfloat Y, GLfloat SX, GLfloat SY, Vector3 Color,float timeinsecs = 99999)//this is also used for normal text which is not stored over multiple frames
	{
		text = Text;
		x = X;
		y = Y;
		sx = SX;
		sy = SY;
		color = Color;
		timetolive = timeinsecs;
	}

};
struct Text
{
	std::map<GLchar, Character> Characters;
	GLuint vao,vbo;
	FT_Library library;
	FT_Face face;
	Shader* shader;
	bool valid;
	Matrix4 projection;
	~Text();
	Text();
	std::vector<TextData> data;
	void update(GLfloat time);
	void addTimedText(std::string text, GLfloat x, GLfloat y,  GLfloat SX, GLfloat SY, Vector3 color,GLfloat time);
	void RenderText(std::string text, GLfloat x, GLfloat y,  GLfloat SX, GLfloat SY, Vector3 color);
	void RenderText(std::string text, GLfloat x, GLfloat y,  GLfloat scale, Vector3 color);
	void RenderText(std::vector<TextData> &data);
	void draw(std::vector<Vector4> &vertices,std::string text);
	void setProjection(float width,float height){projection = Matrix4().InitOrthographic(0,width,0,height,-1,1);};
};

struct text
{
	static Text& get()
	{
		static Text text;
		return text;
	}
};