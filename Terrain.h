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


#include "stb_image.h"
#include "Errors.h"
#include "Mesh.h"
#include "Math\3DMath.h"
#include "Camera3d.h"
#include "Material.h"
#include "Transform.h"
#include "Shader.h"

struct TerrainIndex
	{
		GLuint vao,vab,count;
		std::vector<Vertex> vertices;
		TerrainIndex(std::vector<Vertex> &Vertices)
		{
			glGenVertexArrays(1,&vao);
			glGenBuffers(1,&vab);
			count = Vertices.size();
			vertices = Vertices;
			loadBuffer();
		}
		void resize(float widthChange,float heightChange)
		{
			for(unsigned int i = 0;i < vertices.size();i++)
			{
				Vertex &temp = vertices[i];
				temp.pos=Vector3(temp.pos.x * widthChange,temp.pos.y * heightChange,temp.pos.z * widthChange);
			}
			loadBuffer();
		}
		void loadBuffer()
		{		
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

class Terrain
{
public:
	Terrain(std::string Path,std::string Texture,float PW = 2.0f, float PH = 0.5f,bool Center = false);
	~Terrain(void);
	void render(Shader *shader = NULL);
	
	Material* getMaterial() {return material;}
	Transform* getTransform(){return transform;};
	void loadTerrain();
	void calculateNormalMap();
	void calculateHeightMap(std::string Path);
	std::string& getPath(){return path;}
	Vector2 getPixelSize(){return Vector2(pixelWidth,pixelHeight);}
	float getHeight(float X,float Z);
	void resizeTerrain(float pixelW,float pixelH);
	bool isCentered(){return centered;}
private: 
	int width,height,numComponents,count;
	float pixelWidth,pixelHeight;
	TerrainIndex* Index;
	Material *material;
	Transform *transform;
	Shader *shader;
	std::string path;
	bool success;
	std::vector<std::vector<int>> heightMap;
	std::vector<std::vector<Vector3>> normalMap;
	int extension;
	bool centered ;
};

