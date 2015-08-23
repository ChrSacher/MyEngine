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

class Entity;

enum ComponentType
{
	GRAPHICS,
	COLLISIONS,
	PHYSICS,
	ENTITY,
	NUMCOMPONENT
};

class Component
{
public:
	Component() :
		parent(0) {}
	virtual ~Component() {}

	virtual void processInput(float delta) {} //not inputmanager but input subclass
	virtual void update(float delta) {}
	virtual void render(Shader* shader,Camera3d* camera){}
	virtual std::string sceneSave(){return "";};
	
	inline Transform* GetTransform();
	inline const Transform& GetTransform() const;
	
	virtual void SetParent(Entity* Parent);
protected:
	Entity* parent;
	
	Component(const Component& other) {}
	void operator=(const Component& other) {}
};


class GraphicsComponent:public Component // this needs 2 things instanced and normal
{
public:
	static GraphicsComponent* create(std::string texturePath,std::string normalMap,std::string ObjectPath, Vector3 color = Vector3(1,1,1),bool autoCenter = false);
	~GraphicsComponent();
	void render(Shader* shader,Camera3d* camera);
	std::string sceneSave();
private:
	Material material;
	Mesh mesh;
	bool renderable;
	GraphicsComponent(std::string &texturePath,std::string &normalMap,std::string &ObjectPath, Vector3 color = Vector3(1,1,1),bool autoCenter = false);

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
	void update(Entity* entity);
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
	void update(Entity* entity);
	CollisionWorld* world;
};






