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
Entity* Entity::addComponent(Component* component)
{
	components.push_back(component);
	component->SetParent(this,components.size() - 1); //-1 because array begins with 0
	return this;
}
Entity* Entity::removeComponent(Component* component)
{
	for(int i = 0;i < components.size();i++)
	{
		if(components[i]->getID() == component->getID())
		{
			ComponentManager::get().deleteGraphics(components[i]);
			//delete(component);
			components.erase(components.begin() + i);

		}

	}
	for(int i = 0;i < components.size();i++)
	{
		components[i]->setPosition(i);
	}
	return this;
}
bool Entity::updateComponentPointer(int Position,Component* component)
{
	if(components.size() <= Position ) return false;
	components[Position] = component;
	return true;
}
Entity::~Entity()
{
	for(unsigned int i = 0;i < components.size();i++)
	{
		//delete(components[i]);//needs to be moved to ComponentManager
	}
}
void Entity::render(Shader* shader,Camera3d* camera)
{
	for(unsigned int i = 0;i < components.size();i++)
	{
		components[i]->render(shader,camera);
	}
}
void Entity::update(float delta)
{
	for(unsigned int i = 0;i < components.size();i++)
	{
		components[i]->update(delta);
	}
}
int Entity::getID()
{
	return ID;
}
Entity::Entity(std::string Name,Vector3 pos,Vector3 rot,Vector3 skal):transform(pos,rot,skal),name(Name)
{
	ID = id++;
	
}

std::string Entity::saveScene()
{
	std::string returnS = "E "	+ name + " "+ std::to_string(transform.getPos().x)		+ " " +std::to_string(transform.getPos().y)	+ " " +std::to_string(transform.getPos().z) + " "
								+ std::to_string(transform.getRot().x)		+ " " +std::to_string(transform.getRot().y)	+ " " +std::to_string(transform.getRot().z) + " "
								+ std::to_string(transform.getScale().x)	+ " " +std::to_string(transform.getScale().y)	+ " " +std::to_string(transform.getScale().z) + "\n";

	for(unsigned int i = 0;i < components.size();i++)
	{
		returnS += components[i]->sceneSave();
	}
	return returnS;
}
