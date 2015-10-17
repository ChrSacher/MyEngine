#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include "Math/3DMath.h"
#include "Errors.h"
#include "Mesh.h"
#include "Transform.h"
#include <math.h>
#include "Material.h"
#include "Camera3d.h"
#include "Transform.h"
#include "Entity.h"
#include "Terrain.h"
#include "UIrenderer.h"
#include "Lighting.h"
#include "PhysicsEngine.h"
#include "ServiceLocator.h"

class Entity;


enum ComponentType
{
	GRAPHICS,
	COLLISIONS,
	ENTITY,
	TERRAIN,
	AMBIENT,
	DIRECTIONAL,
	SKYBOX,
	NUMCOMPONENT
};

struct ComponentPosition
{
	ComponentType type;
	unsigned int position;
	unsigned int ID;
	ComponentPosition(ComponentType Type, unsigned int Pos) :type(Type), position(Pos) { static unsigned int id = 0; ID = id++; };
};

class Component
{
public:
	Component() :
		parent(0)
	{
		static int id = 0;
		ID = id++;
	}
	virtual ~Component() {}

	//whatever input 
	virtual void processInput() {} //not inputmanager but input subclass
	//update the Component
	virtual void update() {}
	virtual void render(Shader* shader,Camera3d* camera){}
	virtual std::string sceneSave(){return "";};
	int getID(){return ID;}
	inline Transform* GetTransform();
	inline const Transform& GetTransform() const;
	
	void notify(int eventType,Entity* entityInteractedWith = NULL);
	void receive(int eventType, Component* sender,Entity* entityInteractedWith = NULL);
	void SetParent(Entity* Parent);
protected:
	Entity* parent;
	Component(const Component& other) {}
	void operator=(const Component& other) {}
	int ID;
	ComponentType type;
};

class SkyBoxComponent : public Component
{
public:
	SkyBoxComponent(Vector3 color ,std::vector<std::string> paths);
	~SkyBoxComponent();
	Skybox skyBox;
	void render(Shader* shader,Camera3d* camera);
	std::string sceneSave();
};

class AmbientLightComponent :public Component
{
public:
	AmbientLightComponent(Vector3 Ambient = Vector3(0.5f,0.5f,0.5f)):ambient(Ambient){type = AMBIENT;};
	~AmbientLightComponent(void){};
	AmbientLight ambient;
	void render(Shader* shader,Camera3d* camera);
	static void setColor(std::vector<AmbientLightComponent> &ambients,Shader *shader);
	std::string sceneSave();
};

class DirectionalLightComponent :public Component
{
public:
	DirectionalLightComponent(BaseLight Base = BaseLight(),Vector3 Dir = Vector3(1,1,1)):light(Base,Dir){type = DIRECTIONAL;};
	~DirectionalLightComponent(void){};
	DirectionalLight light;
	void render(Shader* shader,Camera3d* camera);
	static void setDirects(std::vector<DirectionalLightComponent> &ambients,Shader *shader);
	std::string sceneSave();
};


class GraphicsComponent:public Component // this needs 2 things instanced and normal
{
public:
	~GraphicsComponent();
	void render(Shader* shader,Camera3d* camera);
	std::string sceneSave();
	GraphicsComponent(std::string &texturePath,std::string &normalMap,std::string &ObjectPath, Vector3 color = Vector3(1,1,1),bool autoCenter = false);
private:
	Material material;
	Mesh mesh;
	bool renderable;
	

};

class CollisionWorld
{

};
class CollisionComponent:public Component
{
public:
	static CollisionComponent* create();
	~CollisionComponent();
	CollisionComponent(CollisionWorld *World = NULL);
	void update();
	CollisionWorld* world;
};

class TerrainComponent:public Component
{
	public:
	~TerrainComponent();
	void render(Shader* shader,Camera3d* camera);
	std::string sceneSave();
	TerrainComponent(std::string Path,std::string Texture,Vector3 Scale = Vector3(1,1,1),bool Center = false);
private:
	Terrain terrain;
	

};






//Component Manager needs to be a static thing inside the Engine ...done
//TODO IMPLEMENT A SYSTEM WHERE I CAN STORE RAW DATA IN THOSE VECTORS
//IF THE VECTOR GETS RESIZED ALL COMPONENTS SHOULD UPDATE THEIR PARRENT
//THIS IS DONE BY STORING THE COMPONENT ARRAY POSITION INSIDE THE THE ACTUAL COMPONENT ... done

//create Functions should be moved to Content Manager
//A component Pointer should be returned done
//delete Functions should be moved to Content Manager done
//creatXXX(resources for emplace back) to avoid copying 
//this will be  a singelton ..... done
//add this to ServiceLocator
//this gives me the freedom of setting actuall order of rendering
class ComponentManager
{
public:
	//start the Manager
	void startup();
	//destroy Manager and all components
	void shutdown();
	static ComponentManager& get();
	//update all Components
	void update();
	//render all Components
	void render(Shader* shader,Camera3d* camera);


	ComponentPosition& createGraphics(std::string texturePath,std::string normalMap,std::string ObjectPath, Vector3 color = Vector3(1.0f,1.0f,1.0f),bool autoCenter = false);
	ComponentPosition& createTerrain(std::string Path,std::string Texture,Vector3 Scale = Vector3(1.0f,1.0f,1.0f),bool Center = false);
	ComponentPosition& createAmbient(Vector3 Color);
	ComponentPosition& createDirectional(Vector3 Color = Vector3(1.0f,1.0f,1.0f),float Intensity = 0.2f,Vector3 Dir = Vector3(1.0f,1.0f,1.0f));
	ComponentPosition& createSkyBox(Vector3 color,std::string Directory, std::string posx, std::string negx, std::string posy, std::string negy, std::string posz, std::string negz);
	void deleteComponent(ComponentPosition& Pos);
	Component* findComponent(ComponentPosition Pos);
private:
	std::vector<GraphicsComponent> graphics;
	std::vector<CollisionComponent> collisions;
	std::vector<TerrainComponent> terrains;
	std::vector<AmbientLightComponent> ambients;
	std::vector<DirectionalLightComponent> directionals;
	std::vector<SkyBoxComponent> skies;
	std::map<ComponentType,std::map<unsigned int,ComponentPosition*>> positions; //positions begin at 0
	ComponentManager()
	{
	}
	~ComponentManager()
	{
	}
};













