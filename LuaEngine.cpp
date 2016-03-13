#pragma once
#include "LuaEngine.h"
#include "GameState.h"
#include "ServiceLocator.h"
int SeScript::eventHandlerID = 0;



SeScript::SeScript() :hasTestRun(false), state(chaiscript::Std_Lib::library()), path(""), updateS("update()"), hasUpdate(true)
{

}
void SeScript::end()
{
	try
	{
		if (isValid) state("shutDown()");
	}
	catch (std::exception e)
	{
		std::cout << e.what() << std::endl;
	}
}
void SeScript::reload(std::string &Path)
{
	path = Path;
	state.use(Path);
}
void SeScript::begin()
{
	try
	{
		if (isValid)	state("startUp()");
	}
	catch (std::exception e)
	{
	}
}
int SeScript::addEventHandler(std::string &Type, std::string &function, bool isFunction)
{

	eventHandlers[stringToMET(Type)].insert(std::make_pair(eventHandlerID, function));
	return eventHandlerID++;
}
int SeScript::addEventHandler(MessageEventType Type, std::string &function, bool isFunction )
{
	eventHandlers[Type].insert(std::make_pair(eventHandlerID, function));
	return eventHandlerID++;
}

void write(Vector2 x)
{
	std::cout << x.x << "  " << x.y << std::endl;
}



