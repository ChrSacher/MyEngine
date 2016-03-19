#include "Entity.h"
#include "ServiceLocator.h"
	int Entity::id = 0;
	

	void Entity::entityChanged()
	{

		for (auto &itr = _listeners.begin(); itr != _listeners.end(); ++itr)
		{

			itr->second->entityChanged(this);
		}
	}

	void Entity::addListener(Entity::Listener* listener)
	{
		_listeners.insert(std::make_pair(listener->ListenerID, listener));
	}

	void Entity::removeListener(Entity::Listener* listener)
	{
		_listeners.erase(listener->ListenerID);

	}


void Entity::transformChanged(Transform* transform) 
{ 
	if ((this->transform != *transform)) 
	this->transform.set(*transform);
	entityChanged();
};
void Entity::setTransform(Transform& transformV)
{
	if(transformV != transform)
	transform.set(transformV);
	if (physics) physics->get()->setTransform(transformV);
	entityChanged();
}
Transform* Entity::getTransform()             
{
	return &transform; 
}

Entity* Entity::addComponent(ComponentPosition* component)
{
	components.push_back(component);
	component->get()->SetParent(this); //-1 because array begins with 0
	entityChanged();
	return this;
}
Entity* Entity::removeComponent(ComponentPosition* component)
{
	for (unsigned int i = 0; i < components.size(); i++)
	{
		if (components[i]->getID() == component->getID())
		{
			component->get()->SetParent(NULL);
			component->destroy();
			components.erase(components.begin() + i);
		}
	}
	entityChanged();
	return this;
		
}

void Entity::setPhysicsEnabled(bool x)
{
	physics->get()->setEnabled(x);
	if (x) physics->get()->setTransform(transform);
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
	physics = ServiceLocator::getCM().createPhysicComponent(PhysicsData(PTSPHERE, 1));
	physics->get()->SetParent(this);
	physics->get()->setTransform(transform);
	physics->get()->setEnabled(true);
	entityChanged();
}



