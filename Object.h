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
	Object(std::string Name,std::string Objectpath,Vector3 pos = Vector3(0.0f,0.0f,0.0f),Vector3 rot = Vector3(0.0f,0.0f,0.0f),Vector3 skal = Vector3(1.0f,1.0f,1.0f),std::string texturepath = "texture/white.png",Vector3 color = Vector3(1.0f,1.0f,1.0f),bool autoCenter = false);
	Object(const Object& otherobject);
	Object();
	~Object(void);
	void draw();

	Matrix4& getMatrix();
	GLuint getID(){return __ID;};
	std::string getName(){return __objectName;};
	bool operator==(const Object& other);
	Object& operator=(const Object& other);

	bool renderable; //render or don't render 
	Material *material; //texture + lighting variables
	Mesh *mesh; //vertices
	Transform *transform;//position, rotation, scaling in 3d space

private:
	static GLuint __id;
	GLuint __ID;
	std::string __objectName;
	std::string __filePath;
	
	
};

