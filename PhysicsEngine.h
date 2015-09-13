#pragma once
#include <vector>
#include <map>
#include <bullet\btBulletDynamicsCommon.h>
#include <bullet\btBulletCollisionCommon.h>

class PhysicsEngine
{
public:
	
  /*  btDiscreteDynamicsWorld* world;
	
	static	PhysicsEngine& get();

private:
	PhysicsEngine(void);
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btBroadphaseInterface* overlappingPairCache;
	btSequentialImpulseConstraintSolver* solver;
	std::vector<btCollisionShape *> collisionShapes;
	std::map<unsigned int, btRigidBody *> physicsAccessors;*/
	~PhysicsEngine(void);
};

