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
#include "RenderUtil.h"
#include "Lighting.h"
#include "UIrenderer.h"
#include <ostream>
#include "Shader.h"
#include "Terrain.h"
#include <Windows.h>
#include "FBO.h"
#include "Picker.h"
//#include "Engine.h"
#include "Component.h"

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
	friend class Entity;
	friend class DirectionalLightComponent;
	friend class AmbientLightComponent;
	friend class PhysicsComponent;
	friend class PhysicsWorld;
	friend class GraphicsComponent;
	friend class TerrainComponent;
	friend class Component;
	friend class SeScript;
	friend class CollisionComponent;
	static Scene* createScene(int Height,int Width,std::string path = "none");
	static void deleteScene(Scene* deleteScene);
	
	//Scene updates
	void renderScene();
	void renderGeometry();//used for deferred
	void renderLight();//used for deferred
	bool loadScene(std::string path);


	void update();
	//update
	



	//funktions for adding and Deleting
	void addEntity(Entity* Entity);
	void addPrimitive(Primitives newPrimitive);
	void deleteEntity(int ID);
	//functions for getting
	const Entity* getEntity(int ID);
	const std::map<int,Entity*>& getIDNameMap(){return entities;}
	GLuint getCount(){return entityCount;}
	GLuint getEntityDrawCount(){return drawnEntities;}
	Camera3d* getCamera(){return camera;}
	Ray getClick(int x,int y); //TODO
	std::vector<Entity*> getEntityVector();
	std::vector<Entity*> getEntityOnRay(Ray ray);
	Entity* getFirstEntityOnRay(Ray ray);
	void pick(int x,int y);
	//loading parsing and saving scene
	void saveFile(std::string name);
	
private:
	Scene(){}; 
	~Scene(void);
	std::map<int,Entity*> entities;
	Shader* shader;//0 is normal shader
	GBuffer *deferedShader;
	Shader* instancedShader;
	Camera3d* camera;//TODO MAKE THIS A COMPONENT
	GLuint entityCount;
	FBO *fbo;
	Picker picker;//TODO MAKE THIS A COMPONENT
	GLuint drawnEntities;
	static char seperatingCharacter;
};




