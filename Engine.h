#pragma once
#include "ServiceLocator.h"
#include "Component.h"
#include <vector>
const float MAXCOMPONENTS = 200;
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
		ServiceLocator::initialize();
		ServiceLocator::provide(audio);
		ServiceLocator::provide(text);

	};
	
	static void update(double delta)
	{
		return;//for now
	};
	static void shutDown()
	{	

		text->destroy();
		audio->destroy();
		delete(audio);
		delete(text);

	};
	static Audio* audio;
	static Text* text;
};

