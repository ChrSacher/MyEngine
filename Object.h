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
	friend class InstancedObject;
	
	~Object(void);
	static Object* createObject(std::string Name,std::string Objectpath,Vector3 pos = Vector3(0.0f,0.0f,0.0f),Vector3 rot = Vector3(0.0f,0.0f,0.0f),Vector3 skal = Vector3(1.0f,1.0f,1.0f),std::string texturepath = "res/texture/white.png",Vector3 color = Vector3(1.0f,1.0f,1.0f),std::string NormalMap = "res/texture/normal_up.jpg",bool autoCenter = false);
	void drawMesh();
	bool draw(Shader* shader = NULL,Camera3d* cam = NULL );
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
	Object(std::string Name,std::string Objectpath,Vector3 pos = Vector3(0.0f,0.0f,0.0f),Vector3 rot = Vector3(0.0f,0.0f,0.0f),Vector3 skal = Vector3(1.0f,1.0f,1.0f),std::string texturepath = "res/texture/white.png",Vector3 color = Vector3(1.0f,1.0f,1.0f),std::string NormalMap = "res/texture/normal_up.jpg",bool autoCenter = false);
	Object(const Object& otherobject);
	static GLuint id;
	GLuint ID;
	std::string objectName;
	std::string filePath;
	
	
};

class InstancedObject //TODO Actually finish this class, add remove Transform
{
public:
	friend class Object;
	InstancedObject(std::string Name,std::string Objectpath,std::vector<Vector3> pos,std::vector<Vector3> rot,std::vector<Vector3> skal,std::string texturepath = "res/texture/white.png",Vector3 color = Vector3(1.0f,1.0f,1.0f),std::string NormalMap = "res/texture/normal_up.jpg",bool autoCenter = false);
	InstancedObject(InstancedObject& otherobject);
	~InstancedObject(void);

	void drawMesh();
	void draw(Shader* shader = NULL);
	Matrix4& getMatrix();
	GLuint getID(){return ID;};
	std::string getName(){return objectName;};
	std::string getPath(){return filePath;};
	std::string toStringNames();
	std::string toString();
	InstancedObject& operator=(const InstancedObject& other);


	bool renderable; //render or don't render 
	Material *material; //texture + lighting variables
	Mesh *mesh; //vertices
	std::vector<Transform*> transforms;//position, rotation, scaling in 3d space
	std::vector<Matrix4> modelMatrices;//stores the matrices from transform for OpenGl access

private:
	static GLuint id;
	GLuint buffer;//Buffer for instanced Rendering matrix storing
	GLuint ID;
	std::string objectName;
	std::string filePath;
};


