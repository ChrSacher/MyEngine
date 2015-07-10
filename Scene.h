#pragma once
#include <glew.h>

#include <GL/GLu.h>
#include <tchar.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include "Math/3DMath.h"
#include "Errors.h"
#include "Object.h"
#include "RenderUtil.h"
#include "Lighting.h"
#include "UIrenderer.h"
#include <ostream>
#include "Shader.h"
#include "Terrain.h"
#include <Windows.h>

enum Primitives
{
	Box,
	Pyramide,
	Cylinder,
	Cone
};

enum Mode
{
	Game,
	Editing
};
class Scene
{
public:
	Scene(int Height,int Width,std::string path = "none"); 
	~Scene(void);
	
	//Scene updates
	void renderScene();
	void update();
	//update
	
	//funktions for adding and Deleting
	Object* addObject(std::string Name,std::string Objectpath,Vector3 pos = Vector3(),Vector3 rot = Vector3(),Vector3 skal = Vector3(1.0f,1.0f,1.0f),std::string texturepath = "Texture/white.png",Vector3 color = Vector3(1.0f,1.0f,1.0f),bool autoCenter = false);
	void addObject(Object* object);
	void addPrimitive(Primitives newPrimitive);
	void deleteObject(int ID);
	//functions for getting
	const Object* getObject(int ID);
	const std::map<int,Object*>& getIDName(){return __objects;}
	Camera3d* getCamera(){return __camera;}
	LightingCache* getLightingCache(){return &__lightingCache;}

	//loading parsing and saving scene
		void saveFile(std::string name);
private:
	 std::map<int,Object*> __objects;
	 Skybox* __skybox;
	 Camera3d *__camera;
	 Shader* __shader;
	 Terrain *__terrain;
	 LightingCache __lightingCache;

	bool readFile(std::string Path);

	void parseObject(std::vector<std::string>& lines);
	void parsePointLight(std::vector<std::string>& lines);
	void parseTerrain(std::vector<std::string>& lines);
	void split(const std::string& s, char c, std::vector<std::string>& v);
	bool validateScene(std::vector<std::string> &temp);
	ShaderObjectPipeLine *pipeline;
};


