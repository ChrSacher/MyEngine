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
#include "Object.h"
#include "RenderUtil.h"
#include "Lighting.h"
#include "UIrenderer.h"
#include <ostream>
#include "Shader.h"

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
class Scene:public LightingCache
{
public:
	Scene(int Height,int Width,std::string path = "none"); 
	~Scene(void);
	bool readFile(std::string Path);
	void saveFile(std::string name);
	
	void deleteObject(int ID);
	
	void renderScene();
	
	
	//update
	void update();
	//funktions for adding
	void addObject(std::string Name,std::string Objectpath,Vector3 pos = Vector3(),Vector3 rot = Vector3(),Vector3 skal = Vector3(1.0f,1.0f,1.0f),std::string texturepath = "Textures/white.png",Vector3 color = Vector3(),bool autoCenter = false);
	void addPrimitive(Primitives newPrimitive);
	//functions for getting
	const Object* getObject(int ID);
	const std::map<int,Object*>& getIDName(){return __objects;}
	Camera3d* getCamera(){return __camera;}
	void split(const std::string& s, char c, std::vector<std::string>& v);
	bool validateScene(std::vector<std::string> &temp);
private:
	 std::map<int,Object*> __objects;
	 Skybox* __skybox;
	 Camera3d *__camera;
	 Shader* __shader;
};


