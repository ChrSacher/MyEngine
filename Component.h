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
public:
	Component() :
		parent(NULL),
		transform()
	{
		static int id = 0;
		ID = id++;
	}
	virtual ~Component() {}

	//whatever input 
	virtual void processInput() {} //not inputmanager but input subclass
								   //update the Component
	int getID() { return ID; }
	inline Transform* GetTransform();
	inline const Transform& GetTransform() const;
	void setTransform(Transform& Transform)
	{
		transform = Transform;
	};
	ComponentType getType() { return type; }
	void notify(int eventType, Entity* entityInteractedWith = NULL);
	void receive(int eventType, Component* sender, Entity* entityInteractedWith = NULL);
	void SetParent(Entity* Parent);
protected:
	Entity* parent;
	Component(const Component& other) {}
	void operator=(const Component& other) {}
	int ID;
	ComponentType type;
	Transform transform;
};

//register all listeners inside the ComPos and then update through it
struct ComponentPosition
{
	ComponentType type;
	unsigned int position;
	unsigned int ID;
	ComponentPosition(ComponentType Type, unsigned int Pos) :type(Type), position(Pos) { static unsigned int id = 0; ID = id++; };
	Component* get();
	template <typename ValueType>
	ValueType getT()
	{
		return static_cast<ValueType>(get());
	}
	void destroy();
};
