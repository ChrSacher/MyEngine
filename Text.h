

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
	
	virtual ~Text();
	Text();
	void initialize();
	void destroy();
	std::vector<TextData> data;
	/* update timed text*/
	void update(GLfloat time);
	/*
		param /text/ text that will be displaced
		param /x/ x pos on screen
		param /y/ y pos on screen
		param /SX/ size in x dir
		param /SY/ size in y dir
		param /time/ time in seconds for time to display
		Will render text on screen text will be filling the entire box
	*/
	void addTimedText(std::string text, GLfloat x, GLfloat y,  GLfloat SX, GLfloat SY, Vector3 color = Vector3(1,1,1),GLfloat time = 60.0f);
	/*
		param /text/ text that will be displaced
		param /x/ x pos on screen
		param /y/ y pos on screen
		param /SX/ size in x dir
		param /SY/ size in y dir
		Will render text on screen text will be filling the entire box
	*/
	void renderText(std::string text, GLfloat x, GLfloat y,  GLfloat SX, GLfloat SY, Vector3 color= Vector3(1,1,1));
	/*
		param /text/ text that will be displaced
		param /x/ x pos on screen
		param /y/ y pos on screen
		param /SX/ size in x dir
		param /SY/ size in y dir
		Will render text on screen text will be scaled down to good x-y
	*/
	void renderText(std::string text, GLfloat x, GLfloat y,  GLfloat scale, Vector3 color= Vector3(1,1,1));

	void renderText(std::vector<TextData> &data);
	/*
		param /text/ text that will be displaced
		param Vector4(x,y,SX,SY)
		param /x/ x pos on screen
		param /y/ y pos on screen
		param /SX/ size in x dir
		param /SY/ size in y dir
		Will render text on screen text will be filling the entire box
	*/
	void renderText(std::string text,std::vector<Vector4> &vertices);
	void setProjection(float width,float height){projection = Matrix4().InitOrthographic(0,width,0,height,-1,1);};
private:
	std::map<GLchar, Character> Characters;
	GLuint vao,vbo;
	FT_Library library;
	FT_Face face;
	Shader* shader;
	bool valid;
	Matrix4 projection;
};

struct NullText:public Text
{
	
	NullText(){};
	/* update timed text*/
	void update(GLfloat time){};
	/*
		param /text/ text that will be displaced
		param /x/ x pos on screen
		param /y/ y pos on screen
		param /SX/ size in x dir
		param /SY/ size in y dir
		param /time/ time in seconds for time to display
		Will render text on screen text will be filling the entire box
	*/
	void addTimedText(std::string text, GLfloat x, GLfloat y,  GLfloat SX, GLfloat SY, Vector3 color= Vector3(1,1,1),GLfloat time = 60.0f){};
	/*
		param /text/ text that will be displaced
		param /x/ x pos on screen
		param /y/ y pos on screen
		param /SX/ size in x dir
		param /SY/ size in y dir
		Will render text on screen text will be filling the entire box
	*/
	void renderText(std::string text, GLfloat x, GLfloat y,  GLfloat SX, GLfloat SY, Vector3 color= Vector3(1,1,1)){};
	/*
		param /text/ text that will be displaced
		param /x/ x pos on screen
		param /y/ y pos on screen
		param /SX/ size in x dir
		param /SY/ size in y dir
		Will render text on screen text will be scaled down to good x-y
	*/
	void renderText(std::string text, GLfloat x, GLfloat y,  GLfloat scale, Vector3 color= Vector3(1,1,1)){};

	void renderText(std::vector<TextData> &data){};
	/*
		param /text/ text that will be displaced
		param Vector4(x,y,SX,SY)
		param /x/ x pos on screen
		param /y/ y pos on screen
		param /SX/ size in x dir
		param /SY/ size in y dir
		Will render text on screen text will be filling the entire box
	*/
	void renderText(std::string text,std::vector<Vector4> &vertices){};
	void setProjection(float width,float height){};
};
inline float convertSTT(GLuint wanted,GLuint maxSize)
{
	return (float)wanted/ (float)maxSize * 1000;
}