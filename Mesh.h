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
	static void loadSet()
	{
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
	}
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
	//function to load Buffers
	void init();
	//draw the mesh
	void draw();
	//draw mesh as Instances
	void drawInstanced(int count);
	//load a new OBJ
	void loadOBJ(std::string path,bool autoCenter = false);
	//load a new OBJ uncached
	//autocenter of the OBJ should be centered
	void loadOBJuncached(std::string path,bool autoCenter);
	//load Mesh into Biffer
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



