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


enum UniformType
{
	UTBOOL,
	UTFLOAT,
	UTINT,
	UTVECTOR2,
	UTVECTOR3,
	UTVECTOR4,
	UTMATRIX

};
struct UniformPosition
{
	UniformPosition(GLint Pos,Vector4 Value)
	{
		position = Pos;
		value = Value;
	}
	void get(bool &boolReturn);
	void get(float &returnFloat);
	void get(int &returnInt);
	void get(Vector2 &returnV2);
	void get(Vector3 &returnV3);
	void get(Vector4 &returnV4);
	GLint position;
	Vector4 value;
	UniformType type;
};

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
	
	//shader operations
    void compileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilepath);
    void linkShaders();
	void addVertexShader(std::string path);
	void addGeometryShader(std::string path);
	void addFragmentShader(std::string path);
	void addProgram(std::string path,int type);
	void compileShader(const std::string& filePath, GLuint id);	
	void bind();
	void use();
	void unuse();
    void addAttribute(const std::string& attributeName);
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
  

	Matrices matrices;
	//classes are only known to shader
private:
	std::unordered_map<std::string,UniformPosition> uniforms;
	std::vector<GLuint> attachedShaders;
	GLuint _programID;
	static GLuint currentActiveShader;
	static GLuint lastActiveShader;
	 int _numAttributes;
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
