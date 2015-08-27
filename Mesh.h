#pragma once
#include "Math/3DMath.h"
#include <glew.h>
 
 
#include <GL/GLu.h>
#include <vector>
#include <map>
#include <string>
#include "Errors.h"
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing fla

struct Vertex
{
public:
	Vertex();
	Vertex(const Vector3& position,const Vector2& tcoord,const Vector3 &tnormal,const Vector3& ttangent);
	Vertex(float x,float y,float z);
	Vector3& getPos(){return pos;};
	Vector2& getUV(){return uv;};
	Vector3& getNormal(){return normal;};
	Vector3& getTangent(){return tangent;};
	Vector3 pos;
	Vector2 uv;
	Vector3 normal;
	Vector3 tangent;
};

struct Model
{
	Model(std::string &path);
	Model();
	std::vector<GLushort> Indices;
	std::vector<Vertex> Vertices;
	GLuint countVertices,countIndices,highestIndice;
	bool valid;
	void center();
};


class Mesh
{
public:
	virtual ~Mesh(void);
	Mesh(std::vector<Vertex> vertices);
	Mesh(std::string path,bool autoCenter = false);
	void init();
	void draw();
	void drawInstanced(int count);
	void loadOBJ(std::string path,bool autoCenter = false);
	void loadOBJuncached(std::string path,bool autoCenter);
	void loadBufferVertex();
	void clearData();
	std::string getPath(){return filePath;}
	Model model;
	GLuint getVao(){return vao;}
private:
	bool indiced;
	GLuint vao ,vab[2];
	std::string filePath;
	
};



