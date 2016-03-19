#pragma once
#include <glew.h>

#include <GL/GLu.h>
#include <tchar.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include "Math/3DMath.h"
#include "Errors.h"
#include "RenderUtil.h"
#include <ostream>
#include "Shader.h"
#include <Windows.h>
#include "FBO.h"
#include "Component.h"
#include "Camera3d.h"
struct RayTracer
{

	static Entity* getFirstEntityOnRay(std::vector<Entity*> objs,Camera3d* cam,Ray ray);
	static std::vector<Entity*> getEntitiesOnRay(std::vector<Entity*> objs,Camera3d* cam,Ray ray);
	static Entity* getFirstEntityOnRay(Ray ray);
	static std::vector<Entity*> getEntitiesOnRay(Ray ray);
};

class Picker
{
public:
	Picker(void);
	~Picker(void);
	class Listener
	{
	public:
		Listener()
		{
			static unsigned int id = 0;
			ListenerID = id++;
		}
		virtual ~Listener() { }

		/**
		* Handles when an camera settings change.
		*
		* @param camera The camera that was changed.
		*/
		virtual void pickerChanged(Picker* pick) = 0;
		unsigned int ListenerID;
	};
	void addListener(Picker::Listener* listener);
	void removeListener(Picker::Listener* listener);
	void pickerChanged();

	void reset();
	void setEntity(Entity* ent);
	void pick(std::vector<Entity*> objs,Ray ray,Camera3d* cam);
	void pick(int x, int y , std::vector<Entity*> objs,Camera3d* cam);
	void pick(Ray ray);
	Entity* getPick(){return currentObject;};
private:
	Vector3 lastColor;
	Entity* currentObject;
	Entity* lastObject;
	Entity* pickerEntity;

	std::map<unsigned int,Picker::Listener*> _listeners;
	ComponentPosition* comp;
	void select();
};

