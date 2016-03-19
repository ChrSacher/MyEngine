#include "Transform.h"


Transform::Transform(const Vector3 &Pos,const Vector3 &Rot,const Vector3 &Scale)
{
	pos=Pos;
	rot=Rot;
	sca=Scale;
	hasUpdate = lockedPos = lockedRot = lockedSca = false;
	calculateMatrix();
	
	
}
void Transform::set(Transform &transform)
{
	//lockedPos = transform.lockedPos;
	//lockedRot = transform.lockedRot;
	//lockedSca = transform.lockedSca;
	setPos(transform.pos);
	setRot(transform.rot);
	setScale(transform.sca);
	
	hasUpdate = true;
	calculateMatrix();
	transformChanged();
}
bool Transform::operator!=(Transform& other)
{
	return pos != other.pos || rot != other.rot || sca != other.sca;
}
Transform::~Transform(void)
{
}
void Transform::transformChanged()
{

	for (auto &itr = _listeners.begin(); itr != _listeners.end(); ++itr)
	{
		
		itr->second->transformChanged(this);
	}
}

void Transform::addListener(Transform::Listener* listener)
{
	_listeners.insert(std::make_pair(listener->ListenerID,listener));
}

void Transform::removeListener(Transform::Listener* listener)
{
	_listeners.erase(listener->ListenerID);
	
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
	//probaply a better way for this
	while(rot.x > 360) rot.x -= 360;
	while(rot.y > 360) rot.y -= 360;
	while(rot.z > 360) rot.z -= 360;
	while(rot.x < 0) rot.x += 360;
	while(rot.y < 0) rot.y += 360;
	while(rot.z < 0) rot.z += 360;

	modelMatrix = Matrix4().identity().scale(sca).rotateX(rot.x).rotateY(rot.y).rotateZ(rot.z).translate(pos);
	Matrix4 temp;
	temp.identity().rotateX(rot.x).rotateY(rot.y).rotateZ(rot.z);
	forward = Vector3(temp[8],temp[9],temp[10]);
	right = Vector3(temp[0], temp[1], temp[2]);
	up = Vector3(temp[4], temp[5], temp[6]);
	down = -up;
	left = -right;
	transformChanged();
}

void Transform::setPos(Vector3& Pos)
{
	if (lockedPos) return;
	pos = Pos;
	 hasUpdate = true;
}
void Transform::setRot(Vector3& Rot)
{
	if (lockedRot) 
	{
		return;
	}
	rot=Rot;
	hasUpdate = true;
}
void Transform::setScale(Vector3& Scale)
{
	if (lockedSca) return;
	sca = Scale;
	 hasUpdate = true;
}
