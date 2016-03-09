#pragma once

#include <glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <GL/GLu.h>

#include <stdio.h>
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <memory>
#include <math.h>

#include "PhysicsEngine.h"
#include "stb_image.h"
#include "Errors.h"
#include "Mesh.h"
#include "Math\3DMath.h"
#include "Camera3d.h"
#include "Material.h"
#include "Transform.h"
#include "Shader.h"

struct TerrainIndex //to be removed
	{
		GLuint vao,vab,count;

		TerrainIndex(std::vector<Vertex> &vertices)
		{
			glGenVertexArrays(1,&vao);
			glGenBuffers(1,&vab);
			count = vertices.size();
			glBindVertexArray(vao);
			glBindBuffer(GL_ARRAY_BUFFER,vab);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)offsetof(Vertex,pos));
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)offsetof(Vertex,uv));
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)offsetof(Vertex,normal));
			glBufferData(GL_ARRAY_BUFFER,vertices.size() * sizeof(Vertex),&vertices[0],GL_STATIC_DRAW);
		}
		~TerrainIndex()
		{
			glDeleteBuffers(1,&vab);
			glDeleteVertexArrays(1,&vao);
		}
	};
struct TerrainPatch
{
	GLuint vao, vab, count;
	Material material;
	std::vector<Vertex> vertices;//this should be moved into a physicscomponent
	btDefaultMotionState* groundMotionState;
	btRigidBody* object;
	btBvhTriangleMeshShape* terrainPhysics;
	Vector3 scale;
	//Vector3 trans;


	btVector3 V3BF(Vector3 &r)
	{
		return btVector3(r.x, r.y, r.z);
	}
	void load();
	TerrainPatch(std::vector<Vertex> &Vertices, Material &material2, Vector3& v);
	void operator=(const TerrainPatch &other);
	TerrainPatch(const TerrainPatch &other);
//	void setTrans(Vector3 vec) { trans = vec; }
	~TerrainPatch();
};



class Terrain
{
	friend class TerrainComponentSystem;
public:
	Terrain(std::string Path,std::string Texture,Vector3 Scale,bool Center = false,int NumPatches = 2);
	Terrain() {};
	~Terrain(void);
	void render(Shader *Shader = NULL);
	
	Material* getMaterial() {return &material;}
	Transform* getTransform(){return &transform;};
	void loadTerrain();
	void calculateNormalMap();
	void calculateHeightMap(std::string Path);
	void calculateVertices(std::vector<std::vector<int>> &heightmap, std::vector<std::vector<Vector3>> &normalMap, std::vector<Vertex> &vertices);
	std::string getPath(){return path;}
	float getHeight(float X,float Z);
	void resizeTerrain(Vector3 Scale);
	bool isCentered(){return centered;}
	void setTransform(Transform* Transform){transform = *Transform;};
	void operator=(const Terrain& other);
	void start(std::string Path, std::string Texture, Vector3 Scale, bool Center = false, int NumPatches = 2);


	//prolly needs rework each terrainpatch has 
	
	//load patch physics into this
private: 
	int width,height,numComponents,count;
	TerrainIndex* Index;
	std::vector<TerrainPatch> patches;
	Material material;
	Transform transform;
	Shader *shader;
	std::string path;
	bool success;
	std::vector<std::vector<int>> heightMap;
	std::vector<std::vector<Vector3>> normalMap;
	int extension;
	bool centered ;
	int numPatches;
	unsigned char* data;
};

class TerrainLoader
{
	static void loadTerrain(Terrain* terrain);
};