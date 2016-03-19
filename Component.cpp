#include "Component.h"

void Component::SetParent(Entity* Parent)
{
	parent = Parent;
	if (parent != NULL)
	{
		transform = &Parent->transform;
		setTransform(*parent->getTransform());

	}
	load(parent);
}
void Component::notify(int eventType, Entity* entityInteractedWith)
{
	parent->receive(eventType, this, entityInteractedWith);
};
void Component::receive(int eventType, Component* sender, Entity* entityInteractedWith)
{
};