#pragma once

// Std. Includes
#include <vector>

// GL Includes
#include <GL\glew.h>
 
 
#include "Math/3DMath.h"
#include "Transform.h"
#include <math.h>

#define PI 3.14159265358979323846f
 
class Camera3d
{
public:
    Camera3d(Vector3 Pos,float fov,int width,int height,float zNear,float zFar);
	Camera3d(int WindowWidth, int WindowHeight);
	~Camera3d(void);
	Camera3d(){}
	Matrix4 perspectiveMatrix;
	Matrix4 viewMatrix;
    void OnMouse(int x, int y);

	float cameraspeed;
	bool updateneeded;

	Matrix4& GetViewProjection();
	void updatePerspectiveMatrix(float fov,int width,int height,float zNear,float zFar);
	Vector3 getDir();
	Vector3 getPos();
	Vector3 getUp();
	void init();
	//movement
	void moveforward(float distance = 0);
	void movebackward(float distance = 0);
	void raise(float distance = 0);
	void sink(float distance = 0);
	void turnright();
	void turnleft();
	void strafeleft();
	void straferight();
private:
	Vector3 pos;
    Vector3 dir;
    Vector3 up;
	Vector2 mousePos;
	void Update();

    int windowWidth;
    int windowHeight;

    float AngleH;
    float AngleV;

};