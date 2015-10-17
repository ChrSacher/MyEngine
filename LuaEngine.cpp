#pragma once
#include "LuaEngine.h"

void LuaEngine::scriptCreated(LuaScript* script)
{
	if (_listeners == NULL)
		return;

	for (std::list< LuaEngine::Listener*>::iterator itr = _listeners->begin(); itr != _listeners->end(); ++itr)
	{
		 LuaEngine::Listener* listener = (*itr);
		listener->scriptCreated(script);
	}
}

void LuaEngine::addListener( LuaEngine::Listener* listener)
{
	if (_listeners == NULL)
		_listeners = new std::list< LuaEngine::Listener*>();
	if (listener) _listeners->push_back(listener);
}

void LuaEngine::removeListener( LuaEngine::Listener* listener)
{

	if (_listeners)
	{
		for (std::list< LuaEngine::Listener*>::iterator itr = _listeners->begin(); itr != _listeners->end(); ++itr)
		{
			if ((*itr) == listener)
			{
				_listeners->erase(itr);
				break;
			}
		}
	}
}






