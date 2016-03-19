#ifndef __COMP_H
#define __COMP_H
#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <vector>
#include <list>
#include <fstream>
#include "Math/3DMath.h"
#include "Errors.h"
#include "Mesh.h"
#include "Transform.h"
#include <math.h>
#include "Material.h"
#include "Transform.h"

#include "Terrain.h"
#include "Lighting.h"
#include "PhysicsEngine.h"
#include "Camera3d.h"
#include "LuaEngine.h"
#include "GameState.h"
#include "Audio.h"
#include "UIrenderer.h"
#include "Component.h"

struct Ray;
class Camera3d;
class Entity;
class EntityManager;
class NullEntityManager;
class Component;







class SkyBoxComponent : public Component,public Skybox
{
	friend class SkyBoxComponentSystem;
	friend class Scene;
	friend class Skybox;
	friend class EntityComponentGui;
	friend class ComponentManager;

public:
	SkyBoxComponent();
	~SkyBoxComponent();
};

class AmbientLightComponent :public Component,public AmbientLight
{
	friend class LightComponentSystem;
	friend class Scene;
	friend class EntityComponentGui;
	friend class ComponentManager;

public:
	AmbientLightComponent(){type = AMBIENT;};
	~AmbientLightComponent();
	void reload(Vector3 Ambient = Vector3(0.5f, 0.5f, 0.5f));
};

class DirectionalLightComponent :public Component,public DirectionalLight
{
	friend class LightComponentSystem;
	friend class EntityComponentGui;
	friend class Scene;
	friend class ComponentManager;

public:
	DirectionalLightComponent(){type = DIRECTIONAL;};
	~DirectionalLightComponent();
	void reload(BaseLight &Base = BaseLight(), Vector3 &Dir = Vector3(1, 1, 1));
};

class AudioComponent :public Component
{
	friend class AudioComponentSystem;
	friend class EntityComponentGui;
	friend class Scene;
	friend class ComponentManager;

public:
	AudioComponent() { type = AUDIO; };
	~AudioComponent() {};
	YSE::sound sound;
	bool is2D;
	std::string path;
	AudioComponent& operator=(const AudioComponent& comp)
	{
		return *this;
	}
	void reload(std::string audiopath = "", float Volume = 1.0f);
	void reload(std::string audiopath = "", Vector3 position = Vector3(), float Volume = 1.0f);
	AudioComponent(const AudioComponent& comp)
	{
		return;
	}
};
class GraphicsComponent:public Component // this needs 2 things instanced and normal
{
	friend class GraphicComponentSystem;
	friend class EntityComponentGui;
	friend class Scene;
	friend class ComponentManager;

public:
	~GraphicsComponent();
	GraphicsComponent();
	void setRenderable(bool x) {
		renderable = x;
	};
	void reload(std::string &texturePath, std::string &normalMap, std::string &ObjectPath, Vector3 color, bool autoCenter);
private:
	Material material;
	Mesh mesh;
	bool renderable;
	

};




class PhysicsComponent :public Component
{
public:
	friend class PhysicsComponentSystem;
	friend class Scene;
	friend class EntityComponentGui;
	friend class ComponentManager;

	~PhysicsComponent()
	{
	}
	PhysicsComponent()
	{
		type = PHYSICS;
	}
	void setTransform(Transform& Trans) override;
	void load(Entity* Parent) override;
	void reload(PhysicsData &Data);
	PhysicsData& getData() { return data; }
	btDefaultMotionState* groundMotionState;
	btRigidBody* object;
	btCollisionShape* shape;
	PhysicsData data; //contains all the information 
};

class TerrainComponent :public Component
{
	friend class TerrainComponentSystem;
	friend class Scene;
	friend class Terrain;
	friend class EntityComponentGui;
	friend class ComponentManager;

public:
	~TerrainComponent();
	TerrainComponent();
	void load(Entity* Parent) override;
private:
	Terrain terrain;
	bool hasPhysics;
	std::vector<ComponentPosition*> terrainphysics;
	void setScale(Vector3 scale) {};
};

class ScriptComponent : public Component
{
	friend class ScriptComponentSystem;
	friend class Scene;
	friend class SeScript;
	friend class Camera3d;
	friend class EntityComponentGui;
	friend class ComponentManager;
public:
	ScriptComponent() { type = SCRIPT; };
	~ScriptComponent() {}
	void load(Entity* Entity) override;
	void reload(std::string &path);

private:
	SeScript script;

};



class PhysicsComponentSystem
{
	friend class ComponentManager;
	friend class PhysicsComponent;

	~PhysicsComponentSystem() {};
	PhysicsComponentSystem() {};
	//TODO add what is needed for physics object for now its just a sphere
	void load(PhysicsComponent& comp,PhysicsData &Data);
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
	void load(DirectionalLightComponent &light, BaseLight &Base = BaseLight(), Vector3 &Dir = Vector3(1, 1, 1));
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
	void loadPhysics(TerrainPatch &patch);
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
	PhysicsComponent *component; //used for camera culling
};

class SkyBoxComponentSystem
{
	friend class SkyBoxComponent;
	friend class Skybox;
	SkyBoxComponentSystem(){}
	friend class ComponentManager;
	friend class SkyBoxComponent;
	void update(std::vector<SkyBoxComponent> &r);
	void render(std::vector<SkyBoxComponent> &r, Camera3d* camera);
	void load(SkyBoxComponent& r,Vector3 Color, std::vector<std::string> &paths);
	void unload(SkyBoxComponent& r);
};



class AudioComponentSystem 
{
	friend class ComponentManager;
	friend class AudioComponent;
	AudioComponentSystem();
	void update(std::vector<AudioComponent> &r);
	void load(AudioComponent &r,std::string audiopath = "", float Volume = 1.0f);
	void load(AudioComponent &r, std::string audiopath = "", Vector3 position = Vector3(), float Volume = 1.0f);
	void unload(AudioComponent &r);
	
};



class ScriptComponentSystem
{
	friend class SeScript;
	friend class ScriptComponent;
	friend class ComponentManager;
	friend class Transform;
public:
	class Listener
	{
	public:
		Listener() { static unsigned int i = 0; ListenerID = i++; }
		unsigned int ListenerID;
		virtual ~Listener() { }
		virtual void ScriptCreated(SeScript* Script) = 0;
	};
	ScriptComponentSystem() {  }
	void update(std::vector<ScriptComponent> &r);
	void load(ScriptComponent &r, std::string path = "");
	void unload(ScriptComponent &r);
	
	void addListener(ScriptComponentSystem::Listener* listener);
	void removeListener(ScriptComponentSystem::Listener* listener);
private:
	void ScriptComponentSystem::ScriptCreated(SeScript* SeScript);
	std::map<std::string, chaiscript::Boxed_Value> publicVariables;
	std::list<ScriptComponentSystem::Listener*> _listeners;

};


#endif



