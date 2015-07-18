#pragma once

// Std. Includes
#include <vector>

// GL Includes
#include <GL\glew.h>
 
 
#include "Math/3DMath.h"
#include "Transform.h"
#include <math.h>
#include "Shader.h"
#define PI 3.14159265358979323846f
 
class Camera3d
{
public:
    Camera3d(Vector3 Pos,float fov ,int width,int height,float zNear = 0.1f,float zFar = 1000.0f);
	~Camera3d(void);
	Camera3d(){}
	Matrix4 perspectiveMatrix;
	Matrix4 viewMatrix;
	float cameraspeed;

    void OnMouse(int x, int y);
	void update(Shader *shader);
	void updatePerspectiveMatrix(float fov,int width,int height,float zNear,float zFar);
	void init();

	Matrix4& GetViewProjection();
	Vector3 getDir();
	Vector3 getPos();
	Vector3 getUp();
	Vector2 getZ(){return Vector2(zNear,zFar);}
	float getFov(){return fov;}
	void setDir(Vector3 Dir){dir = Dir;}
	void setPos(Vector3 Pos){pos = Pos;}
	void setUp(Vector3 UP){up = UP;}
	void setFov(float Nfov);
	
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
	float fov;
	void Update();
	bool updateneeded;

    int windowWidth;
    int windowHeight;
	float zNear,zFar;
    float AngleH;
    float AngleV;

};