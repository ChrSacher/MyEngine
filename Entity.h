#pragma once

#include "Component.h"
enum ComponentType;
class Component;
class GraphicComponent;
class PhysicsComponent;
class PhysicsWorld;
class CollisionComponent;
class CollisionWorld;

class Entity
{
public:
	static Entity* create(Vector3 pos = Vector3(0.0f,0.0f,0.0f),Vector3 rot = Vector3(0.0f,0.0f,0.0f),Vector3 skal = Vector3(1.0f,1.0f,1.0f));
	Transform* getTransform();
	const Transform& getTransform() const;
	Entity* addComponent(Component* component);
	~Entity();
	void render(Shader* shader = NULL,Camera3d* camera = NULL);
	void update(float delta);
	std::string saveScene();
	int getID();
private:
	static int id;
	int ID;
	Entity(Vector3 pos = Vector3(0.0f,0.0f,0.0f),Vector3 rot = Vector3(0.0f,0.0f,0.0f),Vector3 skal = Vector3(1.0f,1.0f,1.0f));
	std::vector<Component*> components;
	Transform transform; 
	Entity(const Entity& other);
	void operator=(const Entity& other);
};