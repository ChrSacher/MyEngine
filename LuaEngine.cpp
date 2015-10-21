#pragma once
#include "LuaEngine.h"

LuaScript::LuaScript(LuaScript& script):state(), hasUpdate(true)
{

}
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


ChaiPosition*  LuaEngine::createScript(std::string Path)
{
	Scripts.emplace_back(Path);
	scriptCreated(&Scripts.back());
	ChaiPosition* temp = new ChaiPosition(Scripts.size() - 1);
	positions.insert(std::make_pair(temp->ID, temp));
	return temp;
}

void LuaEngine::deleteScript(ChaiPosition* script)
{
	auto pos = positions.find(script->ID);
	if (pos != positions.end())
	{
		unsigned int i = (pos->second->pos);
		for (auto &it = ++pos; it != positions.end(); it)
		{
			it->second->pos -= 1;
		}
		Scripts[i].end();
		Scripts.erase(Scripts.begin() + i);
		positions.erase(--pos);
		delete (script);
		
	}
}

LuaScript*  LuaEngine::find(ChaiPosition* pos)
{
	auto r = positions.find(pos->ID);
	if (r != positions.end()) return &Scripts[r->second->pos];
	return NULL;
}
LuaEngine::LuaEngine() { _listeners = NULL; init = false; Scripts.reserve(50); }

void  LuaEngine::initialize()
{
}
void  LuaEngine::update()
{
	for (unsigned int i = 0; i < Scripts.size(); i++)
	{
			Scripts[i].update();
	}
}
void  LuaEngine::destroy()
{
	Scripts.clear();
	for (auto &it = positions.begin(); it != positions.end(); it++)
	{
		delete(it->second);
	}
}
