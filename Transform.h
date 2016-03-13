#pragma once

#include "Math/3DMath.h"
#include "Shader.h"
#include <map>
class Transform
{
public:
	Transform(const Vector3 &pos = Vector3() ,const Vector3 &rot = Vector3(),const Vector3 &scale = Vector3(1.0f,1.0f,1.0f));
	~Transform(void);
	friend class ScriptComponentSystem;
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
		virtual void transformChanged(Transform& transform) = 0;
		unsigned int ListenerID;
	};
	void addListener(Transform::Listener* listener);
	void removeListener(Transform::Listener* listener);
	void transformChanged();



	Vector3 getPos(){return pos;};
	Vector3 getRot(){return rot;};
	Vector3 getScale(){return sca;};
	//check if the transform has changed
	//will be turned to false once getMatrix() is called
	bool hasUpdate;
	void setPos(Vector3& Pos);
	void setRot(Vector3& Rot);
	void setScale(Vector3& Scale);
	void set(Transform &transform);
	inline void translate(Vector3 &trans) { pos += trans; }
	inline void rotate(Vector3 &Rot) {rot += Rot;}
	/*
		returns modelMatrix and sets needsupdate to false
	*/
	Matrix4* getMatrix();
	/*
		param shader the shader which is used for rendering
		Will update the modelMatrix in the shader
	*/
	void update(Shader* shader){shader->setUniform("modelMatrix",*getMatrix());};
	void calculateMatrix();
	bool needsUpdate(){return hasUpdate;}
	Vector3 forward;
	Vector3 right;
	Vector3 down;
	Vector3 up;
	Vector3 left;
private:
	std::map<unsigned int,Transform::Listener*> _listeners;
	Vector3 pos;
	Vector3 rot;
	Vector3 sca;
	Matrix4 modelMatrix;
};

