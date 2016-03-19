#pragma once
#include <vector>
#include <map>
#include <bullet\btBulletDynamicsCommon.h>
#include <BulletCollision\CollisionShapes\btHeightfieldTerrainShape.h>
#include <bullet\btBulletCollisionCommon.h>
#include "Timer.h"
#include "Math\3DMath.h"
#include "Mesh.h"
enum PhysicsType
{
	PTBOX,
	PTSPHERE,
	PTCYLINDER,
	PTCAPSULE,
	PTCONE,
	PTCONVEXHULL,
	PTCONVEXTRIANGLE,
	PTTRIANGLEMESH,
	PTPLANE,
	PTEMPTY
};

enum PhysicsGroup
{
	PG_NOTHING = 0,
	PG_GROUP1 = 1,
	PG_GROUP2 = 2,
	PG_GROUP3 = 4,
	PG_GROUP4 = 8,
	PG_GROUP5 = 16,
	PG_GROUP6 = 32


};
struct PhysicsConversion
{
	static btVector3 BV3TV3(const Vector3 &x);
	static Vector3 V3TBT3(const btVector3 &x);
};
struct PhysicsGroupCollision
{
	int group1CollidesWith = PG_GROUP1;
	int group2CollidesWith = PG_GROUP2;
	int group3CollidesWith = PG_GROUP3;
	int group4CollidesWith = PG_GROUP4;
	int group5CollidesWith = PG_GROUP5;
	int group6CollidesWith = PG_GROUP6;
	static PhysicsGroupCollision coll;
};
struct PhysicsData
{
	PhysicsData() :type(PTSPHERE) { data.push_back(1.0f); vertices.push_back(Vertex(Vector3(1, 1, 1), Vector2(1, 1), Vector3(0, 1, 0), Vector3(0, 1, 0))); filePath = ""; mass = 1.0f; };
	PhysicsData(PhysicsType Type, float Mass = 0, std::vector<float> Data = std::vector<float>(), std::string Path = "", std::vector<Vertex>& Vert = std::vector<Vertex>());
	PhysicsType type;
	std::string filePath;
	std::vector<Vertex> vertices;
	bool hasVertices;
	float mass;
	std::vector<float> data; //will have different meaning depening on type is sued for collision shapes if simple
	static std::string typeToString(PhysicsType Type);
	static PhysicsType stringToType(std::string Type);
};
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
	btCollisionWorld::AllHitsRayResultCallback rayCast(Vector3 start, Vector3 end);
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

