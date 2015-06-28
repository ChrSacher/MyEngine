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
#include "Transform.h"
#include "Camera3d.h"
#include "Material.h"
#include "Object.h"
#include "Lighting.h"
const int static MAXPOINTLIGHTS = 4;
const int static MAXSPOTLIGHTS = 4;




class Shader
{
private: //nested class
	struct ObjectInformation
	{
	public:
		ObjectInformation(Object* newObject, GLuint Offset,GLuint Count);
		~ObjectInformation();
		ObjectInformation();
		Object* object; //pointer to object for matrices and textures
		GLuint offset; //offset from all previous objects
		GLuint count; //number of vertices
	};

	class ObjectBatch
	{
	public:
		ObjectBatch();
		~ObjectBatch();
		enum
		{
			VERTEXBUFFER,
			TEXTUREBUFFER,
			NORMALBUFFER,
			INDICESBUFFER,
			NUMBUFFERS
		};
		void addObject(Object* newObject);
		void deleteObject(Object* removeObject);
		bool checkSize(Object* newObject);
		void render(Shader *shader);
		void renderShadow(Shader *shader);
		GLuint vao,vab;
		std::unordered_map<int,ObjectInformation> objects;
		GLuint lastOffset,remainingSize; //pos,uv,normal size
		void loadBuffer();
		void loadBufferLast();
		void loadBufferIndexToLast();
		void emptyBuffer();
		GLuint maxSize;
		GLuint countObjects;
		GLuint lastDeleteObjectIndex;
		ObjectInformation lastInformation;
	};

	class ShaderObjectPipeLine
	{
	public:
		ShaderObjectPipeLine();
		~ShaderObjectPipeLine();
		std::vector<ObjectBatch*> batches;
		void addObject(Object* newObject);
		void deleteObject(Object* removeObject);
		void renderBatches(Shader* shader);
		void renderShadowBatches(Shader* shader);
		void emptyBatch();
		GLuint countBatches;
		
	};

	struct Matrices
	{
		Matrix4 model,view,projection;
	};

public:
	Shader();
	~Shader(void);
	static GLuint currentActiveShader;
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
	std::map<std::string,GLint> uniforms;
	GLint getUniformLocation(const std::string& uniformName);
	//setters
	void setmodelMatrices(std::vector<Matrix4*> modelMatrices,std::vector<Matrix4*> MVPMatrices);
	void setUniform(std::string uniformName, int value);
	void setUniform(std::string uniformName, bool value);
	void setUniform(std::string uniformName, float value);
	void setUniform(std::string uniformName, Vector3 value);
	void setUniform(std::string uniformName, Vector4 value);
	void setUniform(std::string uniformName, Matrix4 value);
	void setUniform(std::string uniformName,BaseLight value);
	void setmodelMatrix(Transform *transform);
	void setviewMatrix(Camera3d *view);
	void setbaseColor(Vector3 Color);
	void setbaseColor(Vector4 Color);
	void setSpecular(Material *material);
	void setCameraPos(Camera3d *view);


	//update operations
	void updateMaterial(Material *material); //kann man auch überladen übersichtshalber mach ich das erst später
	void updateCamera(Camera3d *camera);
	void updateObject(Object *object);
	void updateDirectionLight(DirectionalLight *light);
	void updateAmbientLight(AmbientLight *ambient);
	void updatePointLight(std::string uniformname ,PointLight *point);
	void updatePointLights(std::vector<PointLight> &point);
	void updateSpotLight(std::string uniformname ,SpotLight *spot);
	void updateSpotLights(std::vector<SpotLight> &spot);
	void updateFog(Fog *fog);

	//Batch operations
	void renderBatch();
	void renderShadowBatch();
	void addObject(Object* object);
	void deleteObject(Object* object);
	void emptyBatch();
	ObjectInformation* getObject(Object* object);
	//
    int _numAttributes;
	std::vector<GLuint> attachedShaders;
    void compileShader(const std::string& filePath, GLuint id);	
	GLuint _programID;

	ShaderObjectPipeLine *pipeline;
	Matrices matrices;
	//classes are only known to shader
	
	
};

class BasicShader
{
	BasicShader();
	~BasicShader();
	void addProgram(std::string path,int type);
	void use();
	void unuse();
	GLuint _programID;
	int _numAttributes;
};

#endif
