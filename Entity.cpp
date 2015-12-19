#include "Entity.h"
	int Entity::id = 0;
	

Transform* Entity::getTransform()             
{
	return &transform; 
}
const Transform& Entity::getTransform() const
{ 
	return transform; 
}
Entity* Entity::addComponent(ComponentPosition* component)
{
	components.push_back(component);
	component->get()->SetParent(this); //-1 because array begins with 0
	transform.addListener(component);
	return this;
}
Entity* Entity::removeComponent(ComponentPosition* component)
{
	for (unsigned int i = 0; i < components.size(); i++)
	{
		if (components[i]->ID == component->ID)
		{
			transform.removeListener(component);
			component->get()->SetParent(NULL);
			component->destroy();
			components.erase(components.begin() + i);
		}
	}
	
	return this;
		
}

Entity*  Entity::addScript(ChaiPosition* chai)
{
	scripts.push_back(chai);
	ChaiScript *ref = &chai->get()->getState();
	return this;
}
Entity* Entity::removeScript(ChaiPosition* chai)
{
	for (unsigned int i = 0; i < scripts.size(); i++)
	{
		if (scripts[i]->ID == chai->ID)
		{
			chai->destroy();
			scripts.erase(scripts.begin() + i);
		}
	}
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
	for (unsigned int i = 0; i < scripts.size(); i++)
	{
		scripts[i]->destroy();
	}
}
void Entity::render(Shader* shader,Camera3d* camera)
{
	for(unsigned int i = 0;i < components.size();i++)
	{
		components[i]->get()->render(shader,camera);
	}
}
void Entity::update()
{
	for(unsigned int i = 0;i < components.size();i++)
	{
		components[i]->get()->update();
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
}

std::string Entity::saveScene()
{
	std::string returnS = "E "	+ name + " "+ std::to_string(transform.getPos().x)		+ " " +std::to_string(transform.getPos().y)	+ " " +std::to_string(transform.getPos().z) + " "
								+ std::to_string(transform.getRot().x)		+ " " +std::to_string(transform.getRot().y)	+ " " +std::to_string(transform.getRot().z) + " "
								+ std::to_string(transform.getScale().x)	+ " " +std::to_string(transform.getScale().y)	+ " " +std::to_string(transform.getScale().z) + "\n";

	for(unsigned int i = 0;i < components.size();i++)
	{
		returnS += components[i]->get()->sceneSave();
	}
	return returnS;
}

