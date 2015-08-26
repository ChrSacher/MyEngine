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
class Entity
{
public:
	static Entity* create(std::string Name,Vector3 pos = Vector3(0.0f,0.0f,0.0f),Vector3 rot = Vector3(0.0f,0.0f,0.0f),Vector3 skal = Vector3(1.0f,1.0f,1.0f));
	Transform* getTransform();
	const Transform& getTransform() const;
	Entity* addComponent(Component* component);
	Entity* removeComponent(Component* component);
	bool updateComponentPointer(int Position,Component* component); //returns false if failure
	~Entity();
	void render(Shader* shader = NULL,Camera3d* camera = NULL);
	void update(float delta);
	std::string saveScene();
	int getID();
private:
	static int id;
	int ID;
	std::string name;
	Entity(std::string Name,Vector3 pos = Vector3(0.0f,0.0f,0.0f),Vector3 rot = Vector3(0.0f,0.0f,0.0f),Vector3 skal = Vector3(1.0f,1.0f,1.0f));
	std::vector<Component*> components;
	Transform transform; 
	Entity(const Entity& other);
	void operator=(const Entity& other);
};
