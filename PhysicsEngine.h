#pragma once
#include <vector>
#include <map>
#include <bullet\btBulletDynamicsCommon.h>
#include <BulletCollision\CollisionShapes\btHeightfieldTerrainShape.h>
#include <bullet\btBulletCollisionCommon.h>
#include "Timer.h"
#include "Math\3DMath.h"


class MyDebugDraw : public btIDebugDraw 
{
	int m_debugMode;

public:
	virtual void drawLine(const btVector3& from, const btVector3& to
		, const btVector3& color);
	virtual void drawContactPoint(const btVector3& PointOnB
		, const btVector3& normalOnB, btScalar distance
		, int lifeTime, const btVector3& color);
	virtual void reportErrorWarning(const char* warningString);
	virtual void draw3dText(const btVector3& location, const char* textString);
	virtual void setDebugMode(int debugMode);
	virtual int  getDebugMode() const;
	std::vector<btVector3> points;
	static MyDebugDraw& get() { static MyDebugDraw draw; return draw; }
};

class PhysicsEngine
{
public:

	btDiscreteDynamicsWorld* world;

	PhysicsEngine(void);
	~PhysicsEngine(void);
	void initialize() {};
	void update();
	MyDebugDraw* draw;
private:

	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btBroadphaseInterface* overlappingPairCache;
	btSequentialImpulseConstraintSolver* solver;
	btIDebugDraw* debug;

	//used for basic ground collision
	btCollisionShape* groundShape;
	btRigidBody* groundRigidBody;
	btDefaultMotionState* groundMotionState;

	std::vector<btCollisionShape *> collisionShapes;
	std::map<unsigned int, btRigidBody *> physicsAccessors;
	

};
class NullPhysicsEngine : public PhysicsEngine
{
public:
	NullPhysicsEngine() {}
};