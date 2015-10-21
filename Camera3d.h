#pragma once

// Std. Includes
#include <vector>

// GL Includes
#include <GL\glew.h>
 
 
#include "Math/3DMath.h"
#include "Transform.h"
#include <math.h>
#include "Shader.h"
#include "Audio.h"
#include "LuaEngine.h"

#define PI 3.14159265358979323846f
struct Ray
{
	Vector3 pos;
	Vector3 dir;
	Ray(Vector3 x = Vector3(),Vector3 y = Vector3()){ pos = x; dir = y;}
};
class Camera3d :public LuaEngine::Listener
{
public:
	class Listener
    {
    public:

        virtual ~Listener() { }

        /**
         * Handles when an camera settings change.
         *
         * @param camera The camera that was changed.
         */
        virtual void cameraChanged(Camera3d* camera) = 0;
    };
	void scriptCreated(LuaScript* script);
	void cameraChanged();
	/**
    * Adds a camera listener.
    *
    * @param listener The listener to add.
    */
    void addListener(Camera3d::Listener* listener);

    /**
     * Removes a camera listener.
     *
     * @param listener The listener to remove.
     */
    void removeListener(Camera3d::Listener* listener);
    Camera3d(Vector3 &Pos,float fov ,int width,int height,float zNear = 0.1f,float zFar = 1000.0f);
	~Camera3d(void);
	Matrix4 projectionMatrix;
	Matrix4 viewMatrix;
	Matrix4 viewProjection;
	float cameraspeed;

	//should be called when mouse ahs moved
	//will update the Camera Angles
    void OnMouse(int x, int y,bool ignore = false);
	//load variables into Shader
	void update(Shader *shader);
	//set new Projection
	void updateProjectionMatrix(float fov,int width,int height,float zNear,float zFar);
	//start the camera
	void init();
	//convert screenpos to world ray
	Ray getDirClick(int x,int y);
	Matrix4& GetViewProjection();
	Vector3 getDir();
	Vector3 getPos();
	Vector3 getUp();
	Vector2 getZ(){return Vector2(zNear,zFar);}
	Vector2 getSize(){return Vector2((float)windowWidth, (float)windowHeight);}
	float getFov(){return fov;}
	void setDir(const Vector3 &Dir){dir = Dir;}
	void setPos(const Vector3 &Pos){pos = Pos;}
	void setUp(const Vector3 &UP){up = UP;}
	void setFov(float Nfov);
	
	//returns pos as reference to track it
	const Vector3& trackPos(){return pos;}
	//movement
	void moveforward(const float distance = 0);
	void movebackward(const float distance = 0);
	void raise(const float distance = 0);
	void sink(const float distance = 0);
	void turnright();
	void turnleft();
	void strafeleft(const float distance = 0);
	void straferight(const float distance = 0);
	//checks
	void setScript(ChaiPosition* script);
	void setScript(std::string Path);
	ChaiPosition* getScript();
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
	ChaiPosition* cameraScript;
    float AngleV;
	std::list<Camera3d::Listener*>* _listeners;
	static Camera3d *currentCamera;
};