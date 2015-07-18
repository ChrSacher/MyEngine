#pragma once
#include <string>
#include <glew.h>
 
 
#include <GL/GLu.h>
#include <vector>


class BoundTexture
{
public:
	BoundTexture(){};
	~BoundTexture(){};

	BoundTexture(BoundTexture const&);              // Don't Implement
	void operator=(BoundTexture const&);	
	void unbind(GLuint ID,GLenum TextureType = GL_TEXTURE_2D); //Unbind texture ID from all Units
	void bind(GLuint ID,GLuint unit,GLenum TextureType = GL_TEXTURE_2D); //bind ID to unit 
private:
	bool _isBound(GLuint ID,GLuint unit); //check if ID is bound to unit in the current shader
	GLuint _units[31];
};

class RenderUtil
{
public:
	RenderUtil(void);
	~RenderUtil(void);
	//Utility Functions
	void clearScreen();
	void initGraphics();
	void renderInWireframe();
	void renderInPolygone();
	std::string getOpenGLVersion();

	//Generate Operations
	void genBuffer(GLuint Num,GLuint &Buffer);
	void genVertexArray(GLuint Num,GLuint &Buffer);
	void genTexture(GLuint Num,GLuint &Buffer);
	//add Operations
	void addProgram(GLuint Program); //shader has to run this function when a program is completed
	//Binding Operations
	void   bindTexture(GLuint ID,GLuint unit,GLenum TextureType = GL_TEXTURE_2D);
	void   unbindTexture(GLuint ID);
	void   activeTexture(GLuint Unit);
	void   bindVertexArray(GLuint ID);
	void   useProgram(GLuint ID);
	//Delete Operations
	void deleteVertexArrays(GLuint Num,GLuint &Buffer);
	void deleteBuffers(GLuint Num,GLuint &Buffer);
	void deleteTextures(GLuint Num,GLuint &Texture);
	void deleteProgram(GLuint ID);
	//get Operations
	GLuint getCurrentProgram(){return CurrentProgram;};
private:
	std::vector<GLuint> _VertexArrays,_Buffers,_Programs,_Textures;
	BoundTexture _BT;
	GLuint CurrentActiveTexture,CurrentProgram,CurrentVertexArray;
};

static RenderUtil util;
