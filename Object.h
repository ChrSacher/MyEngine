#pragma once
#include <glew.h>
 
 
#include <GL/GLu.h>

#include <stdio.h>
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include "Math/3DMath.h"
#include "Errors.h"
#include "Mesh.h"
#include "Texture.h"
#include "Transform.h"
#include "Camera3d.h"
#include <math.h>
#include "Material.h"




class Object
{
public:
	Object(std::string Name,std::string Objectpath,Vector3 pos = Vector3(0.0f,0.0f,0.0f),Vector3 rot = Vector3(0.0f,0.0f,0.0f),Vector3 skal = Vector3(1.0f,1.0f,1.0f),std::string texturepath = "res/texture/white.png",Vector3 color = Vector3(1.0f,1.0f,1.0f),std::string NormalMap = "res/texture/normal_up.jpg",bool autoCenter = false);
	Object(const Object& otherobject);
	~Object(void);

	void drawMesh();

	Matrix4& getMatrix();
	GLuint getID(){return ID;};
	std::string getName(){return objectName;};
	std::string getPath(){return filePath;};
	bool operator==(const Object& other);
	Object& operator=(const Object& other);
	std::string toStringNames();
	std::string toString();
	bool renderable; //render or don't render 
	Material *material; //texture + lighting variables
	Mesh *mesh; //vertices
	Transform *transform;//position, rotation, scaling in 3d space

private:
	static GLuint id;
	GLuint ID;
	std::string objectName;
	std::string filePath;
	
	
};
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
		GLuint render(Shader *shader,Camera3d *cam);
		void renderShadow(Shader *shader);
		GLuint renderColor(Shader *shader,Camera3d* cam, Vector3 color);

		void loadBuffer();
		void loadBufferLast();
		void loadBufferIndexToLast();
		void emptyBuffer();
	private:
		GLuint maxSize;
		GLuint countObjects;
		GLuint lastDeleteObjectIndex;
		GLuint objectsDrawn;
		ObjectInformation lastInformation;
		GLuint vao,vab;
		std::unordered_map<int,ObjectInformation> objects;
		GLuint lastOffset,remainingSize; //pos,uv,normal size
	};

	class ShaderObjectPipeLine
	{
	public:
		ShaderObjectPipeLine();
		~ShaderObjectPipeLine();
		
		void addObject(Object* newObject);
		void deleteObject(Object* removeObject);
		GLuint  renderBatches(Shader* shader,Camera3d *cam = NULL);
		void renderBatchesInstanced(Shader* shader);
		void renderShadowBatches(Shader* shader);
		GLuint renderColor(Shader *shader,Camera3d* cam, Vector3 color);
		void emptyBatch();

		std::vector<ObjectBatch*> batches;
		GLuint countBatches;
		GLuint objectsDrawn;
		bool hasChanged;
		std::map<std::string,std::vector<ObjectInformation*>> instanceMap;
	};

