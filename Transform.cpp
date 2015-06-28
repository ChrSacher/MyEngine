#include "Transform.h"


Transform::Transform(const Vector3 &Pos,const Vector3 &Rot,const Vector3 &Scale)
{
	pos=Pos;
	rot=Rot;
	sca=Scale;
	update = false;
	calculateMatrix();
}


Transform::~Transform(void)
{
}



Matrix4 Transform::getMatrix() 
{
	if(update)
	{
		calculateMatrix();
		update=false;
	}
	return modelMatrix;
};

void Transform::calculateMatrix()
{
	modelMatrix = Matrix4().identity().scale(sca).rotateX(rot.x).rotateY(rot.y).rotateZ(rot.z).translate(pos);
}

void Transform::setPos(Vector3& Pos)
{
	pos = Pos;
	update = true;
}
void Transform::setRot(Vector3& Rot)
{
	rot=Rot;
	update = true;
}
void Transform::setScale(Vector3& Scale)
{
	sca = Scale;
	update = true;
}