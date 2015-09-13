#include "PhysicsEngine.h"


/*PhysicsEngine::PhysicsEngine(void)
{
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	overlappingPairCache = new btDbvtBroadphase();
	solver = new btSequentialImpulseConstraintSolver();
	world = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);
	world->setGravity(btVector3(0,-9.81,0));
}
PhysicsEngine& PhysicsEngine::get()
{
	static PhysicsEngine en;
	return en;
}

PhysicsEngine::~PhysicsEngine(void)
{
	delete(collisionConfiguration);
	delete(dispatcher);
	delete(overlappingPairCache);
	delete solver;
	delete world;
}*/
