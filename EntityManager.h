#pragma once
#include "Entity.h"

class EntityManager
{
	friend Entity;
public:
	std::map<std::string, Entity*> entities;
	Entity* create(std::string Name, Vector3 pos = Vector3(0.0f, 0.0f, 0.0f), Vector3 rot = Vector3(0.0f, 0.0f, 0.0f), Vector3 skal = Vector3(1.0f, 1.0f, 1.0f));
	void initialize() {};
	void shutdown()
	{
		for (auto &r = entities.begin(); r != entities.end(); r++)
		{
			delete(r->second);
		}
	}
};

class NullEntityManager :public EntityManager
{
	friend Entity;
public:
	Entity* create(std::string Name, Vector3 pos = Vector3(0.0f, 0.0f, 0.0f), Vector3 rot = Vector3(0.0f, 0.0f, 0.0f), Vector3 skal = Vector3(1.0f, 1.0f, 1.0f)) { return NULL; };
};