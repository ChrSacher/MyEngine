#include "Picker.h"


Picker::Picker(void)
{
	lastObject = NULL;
	currentObject = NULL;
}


Picker::~Picker(void)
{
}

Object* RayTracer::getFirstObjectOnRay(std::vector<Object*> objs,Camera3d* cam,Ray ray)
{
	
	Vector3 currentPos;
	 //remove stuff behind camera
	for(int i = 0; i < objs.size();i++)
	{
		if(cam->isBehind(objs[i]->transform->getPos()))
		{
			objs.erase(objs.begin() + i );
			i--;
		}
	}
	if(objs.size() == 0) return NULL;
	std::vector<float> dis;
	for(int i = 0; i < objs.size();i++)
	{
		dis.push_back(10000);
	}
	for(int j = 0; j < 2000; j++)
	{
		float shortestDis = 10000;
		for(int i = 0; i < objs.size();i++)
		{
			float distance = objs[i]->transform->getPos().distance(ray.pos + ray.dir * j );
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
std::vector<Object*> RayTracer::getObjectsOnRay(std::vector<Object*> objs,Camera3d* cam,Ray ray)
{
	std::vector<Object*> returnObj;
	
	Vector3 currentPos;
	 //remove stuff behind camera
	for(int i = 0; i < objs.size();i++)
	{
		if(cam->isBehind(objs[i]->transform->getPos()))
		{
			objs.erase(objs.begin() + i );
			i--;
		}
	}
	if(objs.size() == 0) return returnObj;
	std::vector<float> dis;
	for(int i = 0; i < objs.size();i++)
	{
		dis.push_back(10000);
	}
	for(int j = 0; j < 2000; j++)
	{
		float shortestDis = 10000;
		for(int i = 0; i < objs.size();i++)
		{
			float distance = objs[i]->transform->getPos().distance(ray.pos + ray.dir * j );
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

void Picker::pick(std::vector<Object*> objs,Ray ray,Camera3d* cam)
{
	lastObject = currentObject;
	currentObject = RayTracer::getFirstObjectOnRay(objs,cam,ray);
	if(lastObject == NULL && currentObject == NULL) return;

	if(lastObject == NULL && currentObject != NULL)
	{
		lastColor = currentObject->material->getColor();
		currentObject->material->setColor(Vector3(1,0,0));
	}
	if( lastObject != NULL && currentObject == NULL)
	{
		lastObject->material->setColor(lastColor);
	}
	if(lastObject != NULL && currentObject  != NULL)
	{
		lastObject->material->setColor(lastColor);
		lastColor = currentObject->material->getColor();
		currentObject->material->setColor(Vector3(1,0,0));
	}
}

void Picker::pick(int x, int y , std::vector<Object*> objs,Camera3d* cam)
{
	Ray ray = cam->getDirClick(x,y);
	pick(objs,ray,cam);
}

