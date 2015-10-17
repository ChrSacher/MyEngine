#include "Entity.h"

	int Entity::id = 0;
	
Entity* Entity::create(std::string Name,Vector3 pos,Vector3 rot,Vector3 skal)
{

	return new Entity(Name,pos,rot,skal);

}
Transform* Entity::getTransform()             
{
	return &transform; 
}
const Transform& Entity::getTransform() const
{ 
	return transform; 
}
Entity* Entity::addComponent(ComponentPosition& component)
{
	components.push_back(&component);
	ComponentManager::get().findComponent(component)->SetParent(this); //-1 because array begins with 0
	return this;
}
Entity* Entity::removeComponent(ComponentPosition& component)
{
	for (unsigned int i = 0; i < components.size(); i++)
	{
		if (components[i]->ID == component.ID)
		{
			ComponentManager::get().deleteComponent(component);
			components.erase(components.begin() + i);
		}
	}
	return this;
		
}

	void Entity::notify(int eventType,Component* sender ,Entity* entityInteractedWith) //this can be enchanched with Commands
	{
		for(unsigned int i = 0;i < components.size();i++)
		{
				ComponentManager::get().findComponent(*components[i])->receive(eventType,sender,entityInteractedWith);
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
		ComponentManager::get().deleteComponent(*components[i]);
	}
}
void Entity::render(Shader* shader,Camera3d* camera)
{
	for(unsigned int i = 0;i < components.size();i++)
	{
		ComponentManager::get().findComponent(*components[i])->render(shader,camera);
	}
}
void Entity::update()
{
	for(unsigned int i = 0;i < components.size();i++)
	{
		ComponentManager::get().findComponent(*components[i])->update();
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
		returnS += ComponentManager::get().findComponent(*components[i])->sceneSave();
	}
	return returnS;
}
