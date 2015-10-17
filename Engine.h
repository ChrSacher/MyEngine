#pragma once
#include "ServiceLocator.h"
#include "Component.h"
#include <vector>
#include "Component.h"
#include "PhysicsEngine.h"
#include "InputManager.h"

//Engine class which starts all Subsystem and keeps track of various things
class Engine
{
public:
	Engine(void);
	~Engine(void);

	//start the Engine
	//load subsstems into Serviceloctor
	//keep track of all subsystems
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
		lua->addListener( &InputHandler::get());
		//PhysicsEngine::get().world->getDebugDrawer()->setDebugMode(btIDebugDraw::DBG_DrawWireframe);

	};	
	//shutdown the Engine
	static void shutDown()
	{	

		text->destroy();
		audio->destroy();
		lua->destroy();
		ComponentManager::get().shutdown();
		TextureCache::deleteCache();
		GUI::deleteRenderer();
		delete(audio);
		delete(text);
		delete(lua);

	};
	static Audio* audio;
	static Text* text;
	static LuaEngine* lua;
};

