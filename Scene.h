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
#include "FBO.h"
#include "Picker.h"

enum Primitives
{
	Box,
	Pyramide,
	Cylinder,
	Cone
};

struct SceneDetails
{
	 std::map<int,Object*> objects;
	 Skybox* skybox;
	 Camera3d *camera;
	 Shader* shader;
	 Terrain *terrain;
	 LightingCache lightingCache;
	 GLuint objectCount;
	 SceneDetails(){}
	 void clear()
	 {
		 objects.clear();
		 lightingCache.clear();
	 }
};

struct SceneLoader
{
	bool readFile(std::string Path);
	void parseObject(std::vector<std::string>& lines);
	void parsePointLight(std::vector<std::string>& lines);
	void parseTerrain(std::vector<std::string>& lines);
	void split(const std::string& s, char c, std::vector<std::string>& v);
	bool validateScene(std::vector<std::string> &temp);
	void saveFile(std::string name,SceneDetails& Data);
	Object* addObject(std::string Name,std::string Objectpath,Vector3 pos = Vector3(),Vector3 rot = Vector3(),Vector3 skal = Vector3(1.0f,1.0f,1.0f),std::string texturepath = "res/Texture/white.png",Vector3 color = Vector3(1.0f,1.0f,1.0f),std::string normalMap = "res/texture/normal_up.jpg",bool autoCenter = false);
	SceneDetails loadScene(int Height,int Width,std::string path);
	SceneDetails data;
	SceneLoader()
	{

	}
};

enum Mode
{
	Game,
	Editing
};

class Scene
{
public:
	Scene(){}; 
	~Scene(void);
	static Scene* createScene(int Height,int Width,std::string path = "none");
	//Scene updates
	void renderScene();
	void update(float delta);
	//update
	
	//funktions for adding and Deleting
	Object* addObject(std::string Name,std::string Objectpath,Vector3 pos = Vector3(),Vector3 rot = Vector3(),Vector3 skal = Vector3(1.0f,1.0f,1.0f),std::string texturepath = "res/Texture/white.png",Vector3 color = Vector3(1.0f,1.0f,1.0f),std::string normalMap = "res/texture/normal_up.jpg",bool autoCenter = false);
	void addObject(Object* object);
	void addPrimitive(Primitives newPrimitive);
	void deleteObject(int ID);
	//functions for getting
	const Object* getObject(int ID);
	const std::map<int,Object*>& getIDName(){return objects;}
	LightingCache* getLightingCache(){return &lightingCache;}
	GLuint getCount(){return objectCount;}
	GLuint getObjectDrawCount(){return drawnObjects;}
	Camera3d* getCamera(){return camera;}
	Ray getClick(int x,int y); //TODO
	std::vector<Object*> getObjectVector();
	std::vector<Object*> getObjectsOnRay(Ray ray);
	Object* getFirstObjectOnRay(Ray ray);
	void pick(int x,int y );
	//loading parsing and saving scene
	void parseData(SceneDetails &Data);
	void saveFile(std::string name);
private:
	SceneLoader loader;
	std::map<int,Object*> objects;
	Skybox* skybox;
	Shader* shader;
	Camera3d* camera;
	Terrain *terrain;
	LightingCache lightingCache;
	GLuint objectCount;
	FBO *fbo;
	Picker picker;
	GLuint drawnObjects;
};




