#include "PhysicsEngine.h"
#include "Entity.h"
PhysicsData::PhysicsData(PhysicsType Type, float Mass, std::vector<float> Data , std::string Path, std::vector<Vertex>& Vert) 
	:data(Data), type(Type), vertices(Vert), mass(Mass) {};


std::string PhysicsData::typeToString(PhysicsType Type)
{
	static std::map< PhysicsType, std::string> types{
		std::make_pair(PTBOX,"PTBOX"),
		std::make_pair(PTSPHERE,"PTSPHERE"),
		std::make_pair(PTCYLINDER,"PTCYLINDER"),
		std::make_pair(PTCAPSULE,"PTCAPSULE"),
		std::make_pair(PTCONE,"PTCONE"),
		std::make_pair(PTCONVEXHULL,"PTCONVEXHULL"),
		std::make_pair(PTCONVEXTRIANGLE,"PTCONVEXTRIANGLE"),
		std::make_pair(PTTRIANGLEMESH,"PTTRIANGLEMESH"),
		std::make_pair(PTPLANE,"PTPLANE"),
		std::make_pair(PTEMPTY,"PTEMPTY")
	};
	auto x = types.find(Type);
	if (x == types.end()) return "PTSPHERE";
	return x->second;


}

PhysicsType PhysicsData::stringToType(std::string Type)
{
	static std::map<std::string, PhysicsType> types{
		std::make_pair("PTBOX",PTBOX),
		std::make_pair("PTSPHERE",PTSPHERE),
		std::make_pair("PTCYLINDER",PTCYLINDER),
		std::make_pair("PTCAPSULE",PTCAPSULE),
		std::make_pair("PTCONE",PTCONE),
		std::make_pair("PTCONVEXHULL",PTCONVEXHULL),
		std::make_pair("PTCONVEXTRIANGLE",PTCONVEXTRIANGLE),
		std::make_pair("PTTRIANGLEMESH",PTTRIANGLEMESH),
		std::make_pair("PTPLANE",PTPLANE),
		std::make_pair("PTEMPTY",PTEMPTY)
	};
	auto x = types.find(Type);
	if (x == types.end()) return PTSPHERE;
	return x->second;
}


PhysicsEngine::PhysicsEngine(void)
{
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	overlappingPairCache = new btDbvtBroadphase();
	solver = new btSequentialImpulseConstraintSolver();
	world = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);
	world->setGravity(btVector3(0,-9.81,0));

	groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 1);
	groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));
	btRigidBody::btRigidBodyConstructionInfo
		groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
	groundRigidBody = new btRigidBody(groundRigidBodyCI);
	world->addRigidBody(groundRigidBody);
	draw = new MyDebugDraw();
	//draw->setDebugMode( btIDebugDraw::DBG_DrawWireframe);//breakes the fps
	world->setDebugDrawer(draw);

}
void PhysicsEngine::update()
{
	world->stepSimulation(Time::delta, 10);
	int numManifolds = world->getDispatcher()->getNumManifolds();
	for (int i = 0; i<numManifolds; i++)
	{
		//2
		btPersistentManifold* contactManifold = world->getDispatcher()->getManifoldByIndexInternal(i);

		//3
		int numContacts = contactManifold->getNumContacts();
		if (numContacts > 0)
		{
			const btCollisionObject* obA = contactManifold->getBody0();
			const btCollisionObject* obB = contactManifold->getBody1();

			//6
			Entity* pnA = (Entity*)obA->getUserPointer();
			Entity* pnB = (Entity*)obB->getUserPointer();

			//TODO SEND EVENT HERE COLLISION OCCURED
		}
	}
}

PhysicsEngine::~PhysicsEngine(void)
{
	world->removeRigidBody(groundRigidBody);
	delete(groundShape);
	delete(groundRigidBody);
	delete(groundMotionState);
	delete(draw);
	delete world;
	delete(collisionConfiguration);
	delete(dispatcher);
	delete(overlappingPairCache);
	delete solver;
}
void MyDebugDraw::drawLine(const btVector3& from, const btVector3& to
	, const btVector3& color)
{
	points.push_back(from);
	points.push_back(to);
	
}
void MyDebugDraw::drawContactPoint(const btVector3& PointOnB
	, const btVector3& normalOnB, btScalar distance
	, int lifeTime, const btVector3& color) {}
void MyDebugDraw::reportErrorWarning(const char* warningString) {}
void MyDebugDraw::draw3dText(const btVector3& location, const char* textString) {}
void MyDebugDraw::setDebugMode(int debugMode) { m_debugMode = debugMode; }
int  MyDebugDraw::getDebugMode() const { return m_debugMode; }