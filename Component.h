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
	PHYSICS,
	NUMCOMPONENT
};
//register all listeners inside the ComPos and then update through it
struct ComponentPosition
{
	ComponentType type;
	unsigned int position;
	unsigned int ID;
	ComponentPosition(ComponentType Type, unsigned int Pos) :type(Type), position(Pos) { static unsigned int id = 0; ID = id++; };
	Component* get();
	template <typename ValueType>
	ValueType getT() 
	{
		return static_cast<ValueType>(get());
	}
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
	virtual std::string sceneSave(){return "";};
	int getID(){return ID;}
	inline Transform* GetTransform();
	inline const Transform& GetTransform() const;
	void setTransform(Transform& Transform) 
	{
		transform = Transform;
	};
	ComponentType getType() { return type; }
	void render(Shader* shader, Camera3d* camera);
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
	friend class SkyboxComponentSystem;
public:
	SkyBoxComponent(Vector3 color ,std::vector<std::string> paths);
	~SkyBoxComponent();
	Skybox skyBox;
	std::string sceneSave();
};

class AmbientLightComponent :public Component
{
	friend class LightComponentSystem;
public:
	AmbientLightComponent(){type = AMBIENT;};
	~AmbientLightComponent();
	AmbientLight ambient;
	std::string sceneSave();
};

class DirectionalLightComponent :public Component
{
	friend class LightComponentSystem;
public:
	DirectionalLightComponent(BaseLight &Base = BaseLight(),Vector3 &Dir = Vector3(1,1,1)):light(Base,Dir){type = DIRECTIONAL;};
	~DirectionalLightComponent();
	DirectionalLight light;
	std::string sceneSave();
};


class GraphicsComponent:public Component // this needs 2 things instanced and normal
{
	friend class GraphicComponentSystem;
public:
	~GraphicsComponent();
	std::string sceneSave();
	GraphicsComponent();
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

class PhysicsComponent :public Component
{
public:
	friend class PhysicsComponentSystem;
	
	std::string sceneSave()
	{
		return "";
	}
	~PhysicsComponent()
	{
	}
	PhysicsComponent()
	{

	}
	void set(Transform& transform);
	btDefaultMotionState* groundMotionState;
	btRigidBody* object;
	btCollisionShape* shape;
};

class TerrainComponent :public Component
{
	friend class TerrainComponentSystem;
	
public:
	~TerrainComponent();
	std::string sceneSave();
	TerrainComponent();
private:
	Terrain terrain;
};
class PhysicsComponentSystem
{
	friend class ComponentManager;
	friend class PhysicsComponent;
	~PhysicsComponentSystem() {};
	PhysicsComponentSystem() {};
	//TODO add what is needed for physics object for now its just a sphere
	void load(PhysicsComponent& comp);
	void unload(PhysicsComponent& comp);
	void update(PhysicsComponent& comp);
	void update(std::vector<PhysicsComponent>& comp);
};
class LightComponentSystem
{
	friend class ComponentManager;
	friend class AmbientLightComponent;
	friend class DirectionalLightComponent;
	friend struct AmbientLight;
	friend struct DirectionalLight;
	LightComponentSystem() {}
	void update(std::vector<DirectionalLightComponent> &r);
	void update(std::vector<AmbientLightComponent> &r);
	void render(std::vector<DirectionalLightComponent> &r,Shader *shader);
	void render(std::vector<AmbientLightComponent> &r, Shader *shader);
	void load(AmbientLightComponent &light, Vector3 Ambient = Vector3(0.5f, 0.5f, 0.5f));
};

class TerrainComponentSystem
{
	friend class Terrain;
	friend class ComponentManager;
	friend class TerrainComponent;
	TerrainComponentSystem(){}
	void load(TerrainComponent &r, std::string Path, std::string Texture, Vector3 Scale, bool Center = false, int NumPatches = 2);
	void unload(TerrainComponent &r);
	void update(std::vector<TerrainComponent> &r);
	void render(std::vector<TerrainComponent> &r, Shader* shader);
	void update(TerrainComponent* r);
	void render(TerrainComponent* r, Shader* shader);
};

class GraphicComponentSystem
{
	GraphicComponentSystem(){}
	friend class ComponentManager;
	friend class GraphicsComponent;
	void update(std::vector<GraphicsComponent> &r);
	void render(std::vector<GraphicsComponent> &r, Shader* shader, Camera3d* camera);
	void render(GraphicsComponent &r, Shader* shader, Camera3d* camera);
	void load(GraphicsComponent &r, std::string &texturePath, std::string &normalMap, std::string &ObjectPath, Vector3 color, bool autoCenter);
	void unload(GraphicsComponent &r);
	std::string sceneSave(GraphicsComponent &r);
};

class SkyBoxComponentSystem
{
	SkyBoxComponentSystem(){}
	friend class ComponentManager;
	friend class SkyBoxComponent;
	void update(std::vector<SkyBoxComponent> &r);
	void render(std::vector<SkyBoxComponent> &r, Camera3d* camera);
};













