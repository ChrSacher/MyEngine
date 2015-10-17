#pragma once
#include "Component.h"

class DirectionalLightComponent;
class AmbientLightComponent;
class PhysicsComponent;
class PhysicsWorld;
class GraphicsComponent;
class TerrainComponent;
class Component;
class CollisionComponent;
class CollisionWorld;
enum ComponentType;
class ComponentManager;
struct ComponentPosition;
//Main Component for the Engine
//make Entity Manager
class Entity
{
public:
	static Entity* create(std::string Name,Vector3 pos = Vector3(0.0f,0.0f,0.0f),Vector3 rot = Vector3(0.0f,0.0f,0.0f),Vector3 skal = Vector3(1.0f,1.0f,1.0f));
	Transform* getTransform();
	const Transform& getTransform() const;
	//add a Component to the Entity
	Entity* addComponent(ComponentPosition& component);
	//remove a Component from the Entity
	Entity* removeComponent(ComponentPosition& component);
	//tells all components that something happend
	void notify(int eventType, Component* sender ,Entity* entityInteractedWith = NULL); //this can be enchanched with Commands
	//receive message froma component
	void receive(int eventType, Component* sender,Entity* entityInteractedWith = NULL);
	~Entity();
	void render(Shader* shader = NULL,Camera3d* camera = NULL);
	void update();
	std::string saveScene();
	int getID();
private:
	static int id;
	int ID;
	std::string name;
	Entity(std::string Name,Vector3 pos = Vector3(0.0f,0.0f,0.0f),Vector3 rot = Vector3(0.0f,0.0f,0.0f),Vector3 skal = Vector3(1.0f,1.0f,1.0f));
	std::vector<ComponentPosition*> components;
	Transform transform; 
	Entity(const Entity& other);
	void operator=(const Entity& other);
};
