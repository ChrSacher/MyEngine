#include "Entity.h"


Entity* Entity::create(Vector3 pos,Vector3 rot,Vector3 skal)
{
	return new Entity(pos,rot,skal);
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
	component->SetParent(this);
	return this;
}
Entity::~Entity()
{
	for(unsigned int i = 0;i < components.size();i++)
	{
		delete(components[i]);
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
Entity::Entity(Vector3 pos,Vector3 rot,Vector3 skal):transform(pos,rot,skal)
{
	ID = id++;
	
}

std::string Entity::saveScene()
{
	std::string returnS = "E "	+ std::to_string(transform.getPos().x)		+ "  " +std::to_string(transform.getPos().y)	+ "  " +std::to_string(transform.getPos().z) + " "
								+ std::to_string(transform.getRot().x)		+ "  " +std::to_string(transform.getRot().y)	+ "  " +std::to_string(transform.getRot().z) + " "
								+ std::to_string(transform.getScale().x)	+ "  " +std::to_string(transform.getScale().y)	+ "  " +std::to_string(transform.getScale().z) + "\n";

	for(unsigned int i = 0;i < components.size();i++)
	{
		returnS += components[i]->sceneSave();
	}
	return returnS;
}
