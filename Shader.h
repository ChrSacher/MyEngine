#ifndef SHADER_INCLUDED_H
#define SHADER_INCLUDED_H


#include <glew.h>
 
 
#include <GL/GLu.h>
#include <string>
#include "Errors.h"
#include <vector>
#include <fstream>
#include <unordered_map>
#include "Math/3DMath.h"




class Shader
{
private: //nested class
	
	struct Matrices
	{
		Matrix4 model,view,projection;
	};

public:
	Shader();
	~Shader(void);
	static GLuint currentActiveShader;
	static GLuint lastActiveShader;
	//shader operations
    void compileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilepath);
    void linkShaders();
	void addVertexShader(std::string path);
	void addGeometryShader(std::string path);
	void addFragmentShader(std::string path);
	void addProgram(std::string path,int type);
	void bind();
	void use();
	void unuse();
    void addAttribute(const std::string& attributeName);
	std::unordered_map<std::string,GLint> uniforms;
	GLint getUniformLocation(const std::string& uniformName);
	//setters
	void setmodelMatrices(std::vector<Matrix4*> &modelMatrices) ;
	void setUniform(std::string uniformName, int value) ;
	void setUniform(std::string uniformName, bool value) ;
	void setUniform(std::string uniformName, float value) ;
	void setUniform(std::string uniformName, Vector3 value);
	void setUniform(std::string uniformName, Vector4 value) ;
	void setUniform(std::string uniformName, Matrix4 value) ;
	void setbaseColor(Vector3 Color) ;
	void setbaseColor(Vector4 Color) ;
	void setMVP(Matrix4 &view,Matrix4 &model);
	void setMVP(Matrix4 &model);
	//
    int _numAttributes;
	std::vector<GLuint> attachedShaders;
    void compileShader(const std::string& filePath, GLuint id);	
	GLuint _programID;

	
	Matrices matrices;
	//classes are only known to shader
private:
	
};

class BasicShader:public Shader
{
public:
	BasicShader();
	~BasicShader();
	void addProgram(std::string path,int type);
	void use();
	void unuse();
	GLuint _programID;
	int _numAttributes;
};

#endif
