#pragma once
#include "ServiceLocator.h"
#include "Component.h"
#include <vector>
#include "Component.h"
#include "PhysicsEngine.h"
class Engine
{
public:
	Engine(void);
	~Engine(void);
	static void startUp()
	{
		audio = new LoggedAudio();
		audio->initialize();
		text =  new Text();
		text->initialize();
		lua = new LuaEngine();
		lua->initialize();
		ComponentManager::get().startup();
		ServiceLocator::initialize();
		ServiceLocator::provide(audio);
		ServiceLocator::provide(text);
		ServiceLocator::provide(lua);
		//PhysicsEngine::get().world->getDebugDrawer()->setDebugMode(btIDebugDraw::DBG_DrawWireframe);

	};	
	static void update(double delta)
	{
		return;//for now
	};
	static void shutDown()
	{	

		text->destroy();
		audio->destroy();
		ComponentManager::get().shutdown();
		delete(audio);
		delete(text);

	};
	static Audio* audio;
	static Text* text;
	static LuaEngine* lua;
};

