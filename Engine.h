#pragma once
#include "ServiceLocator.h"
#include "Component.h"
#include <vector>
#include "Component.h"
#include "PhysicsEngine.h"
#include "InputManager.h"
#include "Timer.h"
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
		CM = new ComponentManager();
		CM->initialize();
		EM = new EntityManager();
		EM->initialize();
		PE = new PhysicsEngine();
		PE->initialize();
		
		ServiceLocator::initialize();
		ServiceLocator::provide(CM);
		ServiceLocator::provide(audio);
		ServiceLocator::provide(text);
		ServiceLocator::provide(EM);
		ServiceLocator::provide(PE);
		//PE->world->getDebugDrawer()->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
		CM->addScriptListener( &InputHandler::get());

	};	
	static void update()
	{
		
	}
	//shutdown the Engine
	static void shutDown()
	{	

		text->destroy();
		audio->destroy();
		CM->destroy();
		TextureCache::deleteCache();
		GUI::deleteRenderer();
		delete(audio);
		delete(text);
		delete(CM);
		delete(PE);
	};
	static Audio* audio;
	static Text* text;
	static ComponentManager* CM;
	static EntityManager* EM;
	static PhysicsEngine* PE;
};

