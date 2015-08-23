#include "Transform.h"


Transform::Transform(const Vector3 &Pos,const Vector3 &Rot,const Vector3 &Scale)
{
	pos=Pos;
	rot=Rot;
	sca=Scale;
	hasUpdate = false;
	calculateMatrix();
}


Transform::~Transform(void)
{
}



Matrix4* Transform::getMatrix() 
{
	if( hasUpdate)
	{
		calculateMatrix();
		hasUpdate=false;
	}
	return &modelMatrix;
};

void Transform::calculateMatrix()
{
	modelMatrix = Matrix4().identity().scale(sca).rotateX(rot.x).rotateY(rot.y).rotateZ(rot.z).translate(pos);
}

void Transform::setPos(Vector3& Pos)
{
	pos = Pos;
	 hasUpdate = true;
}
void Transform::setRot(Vector3& Rot)
{
	rot=Rot;
	 hasUpdate = true;
}
void Transform::setScale(Vector3& Scale)
{
	sca = Scale;
	 hasUpdate = true;
}