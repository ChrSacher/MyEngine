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
#include "Transform.h"
#include "Entity.h"
#include "Terrain.h"
#include "UIrenderer.h"
#include "Lighting.h"
#include "PhysicsEngine.h"
#include "Camera3d.h"
class Entity;
class EntityManager;
class NullEntityManager;
class Component;
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
//register all listeners inside the ComPos and then update through it
struct ComponentPosition :public Transform::Listener
{
	ComponentType type;
	unsigned int position;
	unsigned int ID;
	ComponentPosition(ComponentType Type, unsigned int Pos) :type(Type), position(Pos) { static unsigned int id = 0; ID = id++; };
	void transformChanged(Transform& transform);
	Component* get();
	void destroy();
};

class Component
{
public:
	Component() :
		parent(NULL),
		transform()
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
	void setTransform(Transform& Transform) 
	{
		transform = Transform;
	};
	void notify(int eventType,Entity* entityInteractedWith = NULL);
	void receive(int eventType, Component* sender,Entity* entityInteractedWith = NULL);
	void SetParent(Entity* Parent);
protected:
	Entity* parent;
	Component(const Component& other) {}
	void operator=(const Component& other) {}
	int ID;
	ComponentType type;
	Transform transform;
};

class SkyBoxComponent : public Component
{
public:
	SkyBoxComponent(Vector3 color ,std::vector<std::string> paths);
	~SkyBoxComponent();
	Skybox skyBox;
	void render(Shader* shader,Camera3d* camera) override;
	std::string sceneSave();
};

class AmbientLightComponent :public Component
{
public:
	AmbientLightComponent(Vector3 Ambient = Vector3(0.5f,0.5f,0.5f)):ambient(Ambient){type = AMBIENT;};
	~AmbientLightComponent();
	AmbientLight ambient;
	void render(Shader* shader,Camera3d* camera) override;
	static void setColor(std::vector<AmbientLightComponent> &ambients,Shader *shader);
	std::string sceneSave();
};

class DirectionalLightComponent :public Component
{
public:
	DirectionalLightComponent(BaseLight &Base = BaseLight(),Vector3 &Dir = Vector3(1,1,1)):light(Base,Dir){type = DIRECTIONAL;};
	~DirectionalLightComponent();
	DirectionalLight light;
	void render(Shader* shader,Camera3d* camera) override;
	static void setDirects(std::vector<DirectionalLightComponent> &ambients,Shader *shader);
	std::string sceneSave();
};


class GraphicsComponent:public Component // this needs 2 things instanced and normal
{
public:
	~GraphicsComponent();
	void render(Shader* shader,Camera3d* camera) override;
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



















