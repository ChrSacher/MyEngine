#include "Entity.h"
	int Entity::id = 0;
	


void Entity::transformChanged(Transform& transform) 
{ 
	for (unsigned int i = 0; i < components.size(); i++)
	{
		components[i]->get()->setTransform(transform);
	}
};
void Entity::setTransform(Transform& transformV)
{
	transform.set(transformV);
}
Transform* Entity::getTransform()             
{
	return &transform; 
}

Entity* Entity::addComponent(ComponentPosition* component)
{
	components.push_back(component);
	component->get()->SetParent(this); //-1 because array begins with 0
	return this;
}
Entity* Entity::removeComponent(ComponentPosition* component)
{
	for (unsigned int i = 0; i < components.size(); i++)
	{
		if (components[i]->ID == component->ID)
		{
			component->get()->SetParent(NULL);
			component->destroy();
			components.erase(components.begin() + i);
		}
	}
	
	return this;
		
}


//add a Component to the Entity
Entity* Entity::addPhysics(ComponentPosition* component)
{
	removePhysics();
	component->get()->SetParent(this);
	physics = component;
	component->get()->setTransform(transform);
	return this;
}
//remove a Component from the Entity
Entity* Entity::removePhysics()
{
	if (physics != NULL) physics->destroy();
	return this;
}

	void Entity::notify(int eventType,Component* sender ,Entity* entityInteractedWith) //this can be enchanched with Commands
	{
		for(unsigned int i = 0;i < components.size();i++)
		{
				components[i]->get()->receive(eventType,sender,entityInteractedWith);
		}
	}
	void Entity::receive(int eventType, Component* sender,Entity* entityInteractedWith)
	{
		//do stuff if necessary
		notify(eventType,sender,entityInteractedWith);
	}
Entity::~Entity()
{
	for(unsigned int i = 0;i < components.size();i++)
	{
		components[i]->destroy();
	}
}
int Entity::getID()
{
	return ID;
}
Entity::Entity(std::string Name,Vector3 pos,Vector3 rot,Vector3 skal):transform(pos,rot,skal),name(Name)
{
	ID = id++;
	components.reserve(50);
	transform.addListener(this);
	physics = NULL;
}



