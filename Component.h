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
#include "Audio.h"
#include "Terrain.h"
#include "UIrenderer.h"
#include "Lighting.h"

class Entity;


enum ComponentType
{
	GRAPHICS,
	COLLISIONS,
	PHYSICS,
	ENTITY,
	TERRAIN,
	AMBIENT,
	DIRECTIONAL,
	NUMCOMPONENT
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

	virtual void processInput(float delta) {} //not inputmanager but input subclass
	virtual void update(float delta) {}
	virtual void render(Shader* shader,Camera3d* camera){}
	virtual std::string sceneSave(){return "";};
	int getID(){return ID;}
	inline Transform* GetTransform();
	inline const Transform& GetTransform() const;
	
	void SetParent(Entity* Parent,int Position);
	void updatePointer();
	void setPosition(int Position){position = Position;};
protected:
	Entity* parent;
	int position;
	Component(const Component& other) {}
	void operator=(const Component& other) {}
	int ID;
	ComponentType type;
};


class AmbientLightComponent :public Component
{
public:
	AmbientLightComponent(Vector3 ambient = Vector3(0.5f,0.5f,0.5f)){type = AMBIENT;};
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




class PhysicsWorld //should be in physics.cpp and should be gatchered in an Engine Class also more 
{
public:
};
class PhysicsComponent:public Component
{
public:
	static PhysicsComponent* create();
	~PhysicsComponent();
	PhysicsComponent(PhysicsWorld *World = NULL); //if Null no physics on object
	void update(float delta);
	PhysicsWorld* world;
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
	void update(float delta);
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
//A component Pointer should be returned
//delete Functions should be moved to Content Manager
//if deletion all pointers must be updated
//Also try to ask someone who knows shit
//also this shit is fucking huge thing
//creatXXX(resources for emplace back) to avoid copying
//this will be  a singelton ..... done

//this gives me the freedom of setting actuall order of rendering
class ComponentManager
{
public:
	
	void startup();
	void shutdown();
	static ComponentManager& get();
	void update(float delta);
	void render(Shader* shader,Camera3d* camera);


	GraphicsComponent& createGraphics(std::string texturePath,std::string normalMap,std::string ObjectPath, Vector3 color = Vector3(1.0f,1.0f,1.0f),bool autoCenter = false);
	void deleteGraphics(Component *graphicsC);
	TerrainComponent& createTerrain(std::string Path,std::string Texture,Vector3 Scale = Vector3(1.0f,1.0f,1.0f),bool Center = false);
	void deleteTerrain(Component *terrain);
	AmbientLightComponent& createAmbient(Vector3 Color);
	void deleteAmbient(Component * ambient);
	DirectionalLightComponent& createDirectional(Vector3 Color = Vector3(1.0f,1.0f,1.0f),float Intensity = 0.2f,Vector3 Dir = Vector3(1.0f,1.0f,1.0f));
	void deleteDirectional(Component * directional);

private:
	std::vector<GraphicsComponent> graphics;
	std::vector<PhysicsComponent> physics;
	std::vector<CollisionComponent> collisions;
	std::vector<TerrainComponent> terrains;
	std::vector<AmbientLightComponent> ambients;
	std::vector<DirectionalLightComponent> directionals;
	ComponentManager()
	{
	}
	~ComponentManager()
	{
	}
};













