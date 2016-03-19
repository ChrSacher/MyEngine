#include "Picker.h"
#include "ServiceLocator.h"
#include "Component.h"
Picker::Picker(void)
{
	lastObject = NULL;
	currentObject = NULL;
	pickerEntity = ServiceLocator::getEM().create("pickerEntity");
	comp = ServiceLocator::getCM().createGraphics(std::string(), std::string(), std::string("res/models/Picker.obj"),Vector3(1.0f,1.0f,1.0f));
	pickerEntity->addComponent(comp);
	comp->getT<GraphicsComponent*>()->setRenderable(false);
	pickerEntity->getTransform()->setLockRot(true);
}

void Picker::pickerChanged()
{

	for (auto &itr = _listeners.begin(); itr != _listeners.end(); ++itr)
	{

		itr->second->pickerChanged(this);
	}
}

void Picker::addListener(Picker::Listener* listener)
{
	_listeners.insert(std::make_pair(listener->ListenerID, listener));
}

void Picker::removeListener(Picker::Listener* listener)
{
	_listeners.erase(listener->ListenerID);

}


void Picker::reset()
{
	lastObject = currentObject ;
	currentObject = NULL; 
	select();
}
Picker::~Picker(void)
{
	ServiceLocator::getEM().deleteEntity(pickerEntity);

}

Entity* RayTracer::getFirstEntityOnRay(Ray ray)
{
	
	auto &x = ServiceLocator::getPE().rayCast(ray.pos,ray.pos +  ray.dir * 1000);
	std::vector<Entity*> c;
	for (unsigned int i = 0; i < x.m_collisionObjects.size(); i++)
	{
		c.push_back(static_cast<Entity*>(x.m_collisionObjects[0]->getUserPointer()));
		if(x.m_collisionObjects[0]->getUserPointer() != NULL) return static_cast<Entity*>(x.m_collisionObjects[0]->getUserPointer());
	}
	return NULL;
}
std::vector<Entity*> RayTracer::getEntitiesOnRay(Ray ray)
{
	std::vector<Entity*> c;
	auto &x = ServiceLocator::getPE().rayCast(ray.pos, ray.dir);
	for (unsigned int i = 0; i < x.m_collisionObjects.size(); i++)
	{
		c.push_back(static_cast<Entity*>(x.m_collisionObjects[0]->getUserPointer()));
	}
	return c;
}


Entity* RayTracer::getFirstEntityOnRay(std::vector<Entity*> objs,Camera3d* cam,Ray ray)
{
	
	Vector3 currentPos;
	 //remove stuff behind camera
	for(unsigned int i = 0; i < objs.size();i++)
	{
		if(cam->isBehind(objs[i]->getTransform()->getPos()))
		{
			objs.erase(objs.begin() + i );
			i--;
		}
	}
	if(objs.size() == 0) return NULL;
	std::vector<float> dis;
	for(unsigned int i = 0; i < objs.size();i++)
	{
		dis.push_back(10000);
	}
	for(unsigned int j = 0; j < 2000; j++)
	{
		float shortestDis = 10000;
		for(unsigned int i = 0; i < objs.size();i++)
		{
			float distance = objs[i]->getTransform()->getPos().distance(ray.pos + ray.dir * j );
			if(distance < dis[i])
			{
				dis[i] = distance;
				if(distance < shortestDis) shortestDis = distance;
				if(distance < 1)
				{
					return objs[i];
				}//should check for boundingsphere for now arbitrary number
			}
			else
			{
				objs.erase(objs.begin() + i);
				dis.erase(dis.begin() + i);
				i--;
			}//distance is smaller
		}//remove all where distance is higher than before
		if(objs.size() == 0) break;
		j += shortestDis;
	};
	return NULL;
}
std::vector<Entity*> RayTracer::getEntitiesOnRay(std::vector<Entity*> objs,Camera3d* cam,Ray ray)
{
	std::vector<Entity*> returnObj;
	
	Vector3 currentPos;
	 //remove stuff behind camera
	for(unsigned int i = 0; i < objs.size();i++)
	{
		if(cam->isBehind(objs[i]->getTransform()->getPos()))
		{
			objs.erase(objs.begin() + i );
			i--;
		}
	}
	if(objs.size() == 0) return returnObj;
	std::vector<float> dis;
	for(unsigned int i = 0; i < objs.size();i++)
	{
		dis.push_back(10000);
	}
	for(unsigned int j = 0; j < 2000; j++)
	{
		float shortestDis = 10000;
		for(unsigned int i = 0; i < objs.size();i++)
		{
			float distance = objs[i]->getTransform()->getPos().distance(ray.pos + ray.dir * j );
			if(distance < dis[i])
			{
				dis[i] = distance;
				if(distance < shortestDis) shortestDis = distance;
				if(distance < 1)
				{
					returnObj.push_back(objs[i]);
					objs.erase(objs.begin() + i);
					dis.erase(dis.begin() + i);
					i--;
				}//should check for boundingsphere for now arbitrary number
			}
			else
			{
				objs.erase(objs.begin() + i);
				dis.erase(dis.begin() + i);
				i--;
			}//distance is smaller
		}//remove all where distance is higher than before
		if(objs.size() == 0) break;
		j += shortestDis;
	};
	return returnObj;
}

void Picker::pick(std::vector<Entity*> objs,Ray ray,Camera3d* cam)
{
	lastObject = currentObject;
	currentObject = RayTracer::getFirstEntityOnRay(objs,cam,ray);
	select();
}
void Picker::setEntity(Entity* ent)
{
	lastObject = currentObject;
	currentObject = ent;
	select();
}
void Picker::pick(Ray ray)
{
	lastObject = currentObject;
	currentObject = RayTracer::getFirstEntityOnRay(ray);
	select();
}

void Picker::select()
{
	
	if (lastObject == currentObject) return;
	pickerChanged();
	if (currentObject != NULL)
	{
		if (lastObject != NULL) lastObject->getTransform()->removeListener(pickerEntity);
		comp->getT<GraphicsComponent*>()->setRenderable(true);
	    pickerEntity->setTransform(*currentObject->getTransform());
		currentObject->getTransform()->addListener(pickerEntity);
	}
	else
	{
		if (lastObject != NULL) lastObject->getTransform()->removeListener(pickerEntity);
		comp->getT<GraphicsComponent*>()->setRenderable(false);
	}
}
void Picker::pick(int x, int y , std::vector<Entity*> objs,Camera3d* cam)
{
	Ray ray = cam->getDirClick(x,y);
	pick(objs,ray,cam);
}

