#pragma once

#include "Math/3DMath.h"
 
class Transform
{
public:
	Transform(const Vector3 &pos = Vector3() ,const Vector3 &rot = Vector3(),const Vector3 &scale = Vector3(1.0f,1.0f,1.0f));

	~Transform(void);
	
	Matrix4 modelMatrix;
	Vector3 getPos(){return pos;};
	Vector3 getRot(){return rot;};
	Vector3 getScale(){return sca;};
	bool update;
	void setPos(Vector3& Pos);
	void setRot(Vector3& Rot);
	void setScale(Vector3& Scale);
	Matrix4 getMatrix();

	void calculateMatrix();
private:
	Vector3 pos;
	Vector3 rot;
	Vector3 sca;
};

