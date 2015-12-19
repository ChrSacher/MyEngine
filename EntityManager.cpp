#include "EntityManager.h"

Entity* EntityManager::create(std::string Name, Vector3 pos, Vector3 rot, Vector3 skal)
{

	Entity* temp = new Entity(Name, pos, rot, skal);
	entities.insert(std::make_pair(Name, temp));
	//send this to chai?
	return temp;
}