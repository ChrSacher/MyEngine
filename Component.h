#pragma once
#include "Transform.h"
#include "Entity.h"

class Entity;
enum ComponentType
{
	GRAPHICS,
	COLLISIONS,
	ENTITY,
	TERRAIN,
	AMBIENT,
	DIRECTIONAL,
	SKYBOX,
	PHYSICS,
	SCRIPT,
	AUDIO,
	NUMCOMPONENT
};

class Component

{
	friend class Entity;
public:
	Component() :
		parent(NULL)
		
	{
		static int id = 0;
		ID = id++;
		enabled = true;
	}
	virtual ~Component() {}

	//whatever input 
	virtual void processInput() {} //not inputmanager but input subclass
								   //update the Component
	int getID() { return ID; }
	ComponentType getType() { return type; }
	virtual void setTransform(Transform& Trans) {}; //used for components which need to set something somewhere else like physics
	void notify(int eventType, Entity* entityInteractedWith = NULL);
	void receive(int eventType, Component* sender, Entity* entityInteractedWith = NULL);
	void SetParent(Entity* Parent);
	void setEnabled(bool ena) { enabled = ena; }
	bool getEnabled() { return enabled; }
	virtual void load(Entity* Parent) {};
protected:
	Transform* transform;
	Entity* parent;
	Component(const Component& other) {}
	void operator=(const Component& other) {}
	int ID;
	bool enabled;
	ComponentType type;
};

//register all listeners inside the ComPos and then update through it
struct ComponentPosition
{
	ComponentType type;
	unsigned int position;
	
	ComponentPosition(ComponentType Type, unsigned int Pos) :type(Type), position(Pos) { static unsigned int id = 0; ID = id++; };
	Component* get();
	template <typename ValueType>
	ValueType getT()
	{
		return static_cast<ValueType>(get());
	}
	void destroy();
	unsigned int getID() { return ID; }
private:
	unsigned int ID;
};
