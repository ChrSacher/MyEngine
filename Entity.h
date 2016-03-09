#ifndef __ENT_H
#define __ENT_H
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
class Camera3d;
struct Ray;
class ScriptComponentSystemListener;

//Main Component for the Engine

class Entity :public Transform::Listener
{
public:
	friend class EntityManager;
	friend class Scene;
	void transformChanged(Transform& transform);
	Transform* getTransform();
	const Transform& getTransform() const;
	//add a Component to the Entity
	Entity* addComponent(ComponentPosition* component);
	//remove a Component from the Entity
	Entity* removeComponent(ComponentPosition* component);

	//add a Component to the Entity
	Entity* addPhysics(ComponentPosition* component);
	//remove a Component from the Entity
	Entity* removePhysics();
	void setTransform(Transform& transformV);
	//tells all components that something happend
	void notify(int eventType, Component* sender ,Entity* entityInteractedWith = NULL); //this can be enchanched with Commands
	//receive message froma component
	void receive(int eventType, Component* sender,Entity* entityInteractedWith = NULL);
	~Entity();
	ComponentPosition* getPhysics() { return physics; }
	int getID();
private:
	static int id;
	int ID;
	std::string name;
	Entity(std::string Name,Vector3 pos = Vector3(0.0f,0.0f,0.0f),Vector3 rot = Vector3(0.0f,0.0f,0.0f),Vector3 skal = Vector3(1.0f,1.0f,1.0f));
	std::vector<ComponentPosition*> components;
	ComponentPosition* physics;
	Transform transform;


	Entity(const Entity& other);
	void operator=(const Entity& other);
};

#endif