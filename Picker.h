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
#include "Object.h"
#include "RenderUtil.h"
#include <ostream>
#include "Shader.h"
#include <Windows.h>
#include "FBO.h"


struct RayTracer
{

	static Object* getFirstObjectOnRay(std::vector<Object*> objs,Camera3d* cam,Ray ray);
	static std::vector<Object*> getObjectsOnRay(std::vector<Object*> objs,Camera3d* cam,Ray ray);
};

class Picker
{
public:
	Picker(void);
	~Picker(void);
	
	void pick(std::vector<Object*> objs,Ray ray,Camera3d* cam);
	void pick(int x, int y , std::vector<Object*> objs,Camera3d* cam);
	Object* getPick(){return currentObject;};
private:
	Vector3 lastColor;
	Object* currentObject;
	Object* lastObject;
};

