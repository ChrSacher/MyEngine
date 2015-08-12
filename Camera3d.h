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
 
struct Ray
{
	Vector3 pos;
	Vector3 dir;
	Ray(Vector3 x = Vector3(),Vector3 y = Vector3()){ pos = x; dir = y;}
};
class Camera3d
{
public:
    Camera3d(Vector3 Pos,float fov ,int width,int height,float zNear = 0.1f,float zFar = 1000.0f);
	~Camera3d(void);
	Camera3d(){}
	Matrix4 projectionMatrix;
	Matrix4 viewMatrix;
	Matrix4 viewProjection;
	float cameraspeed;

    void OnMouse(int x, int y,bool ignore = false);
	void update(Shader *shader);
	void updateProjectionMatrix(float fov,int width,int height,float zNear,float zFar);
	void init();
	Ray getDirClick(int x,int y);
	Matrix4& GetViewProjection();
	Vector3 getDir();
	Vector3 getPos();
	Vector3 getUp();
	Vector2 getZ(){return Vector2(zNear,zFar);}
	Vector2 getSize(){return Vector2(windowWidth,windowHeight);}
	float getFov(){return fov;}
	void setDir(Vector3 Dir){dir = Dir;}
	void setPos(Vector3 Pos){pos = Pos;}
	void setUp(Vector3 UP){up = UP;}
	void setFov(float Nfov);
	
	const Vector3& trackPos(){return pos;}
	//movement
	void moveforward(float distance = 0);
	void movebackward(float distance = 0);
	void raise(float distance = 0);
	void sink(float distance = 0);
	void turnright();
	void turnleft();
	void strafeleft();
	void straferight();
	 

	//checks
	bool isBehind(Vector3 pos)
	{
		return (pos - this->pos).dot(dir) <= 0;
	}
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