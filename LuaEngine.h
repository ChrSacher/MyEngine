#pragma once

#include <selene.h>
#include <string>
#include <vector>
#include "Math\3DMath.h"
using namespace sel;
//a script which will be used by the Engine and regulary updated
class LuaScript
{
public:
	//Path to LuaScript which will be used
	LuaScript(std::string Path) :state(true), path(Path), updateFun(state["update"]), hasUpdate(false)
	{

		bool found = state.Load(Path);
		if (!found)
		{
			std::cout << "LuaScript not found" << std::endl;
			return;
		}
		if (!state.CheckNil("update")) hasUpdate = true;
		
	}
	// start the startUp function if exists
	void begin()
	{
		if (!state.CheckNil("startUp"))
		{
			state["startUp"]();
		}
	}
	//load empty Script
	LuaScript() :state(true),path("NA"),updateFun(state[""])
	{
		hasUpdate = false;
	}
	//reload the Script with new state and file
	//new objects need to be loaded in again
	void reload(std::string &Path)
	{
		path = Path;
		state = State(true);
		state.Load(Path);
	}
	~LuaScript()
	{
		if (!state.CheckNil("shutDown"))
		{
			state["shutDown"]();
		}
	}
	//uuse update function
	void update()
	{
		updateFun();
	}
	//loads new empty state
	void reload()
	{
		state = sel::State(true);
		state.Load(path);
	}
	sel::State& getState() { return state; }
protected:
	sel::State state;
	std::string path;
	std::map<std::string, sel::Selector> variables;
	sel::Selector updateFun;
	bool hasUpdate;
};

//Class for keeping track and updating all scripts
class LuaEngine
{
public:
	LuaEngine() { _listeners = NULL; }
	~LuaEngine() {}
	LuaScript* createScript(std::string Path)
	{
		LuaScript* temp = new LuaScript(Path);
		scripts.insert(std::make_pair(temp,temp));
		return temp;
	}
	class Listener
	{
	public:
		virtual ~Listener() { }

		/**
		* Handles when an camera settings change.
		*
		* @param camera The camera that was changed.
		*/
		virtual void scriptCreated(LuaScript* script) = 0;
	};
	void scriptCreated(LuaScript* script);
	/**
	* Adds a camera listener.
	*
	* @param listener The listener to add.
	*/
	void addListener(LuaEngine::Listener* listener);
	void removeListener(LuaEngine::Listener* listener);
	void deleteScript(LuaScript* script)
	{
		auto pos = scripts.find(script);
		if (pos != scripts.end())
		{
			delete (pos->second);
			scripts.erase(pos);
		}
	}
	void initialize()
	{

	}
	void update()
	{
		for (auto it = scripts.begin(); it != scripts.end();it++)
		{
			it->second->update();
		}
	}
	void destroy()
	{
		for (auto it = scripts.begin(); it != scripts.end(); it++)
		{
			  delete(it->second);
		}		
	}
protected:
	std::map<LuaScript*, LuaScript*> scripts;
	std::list<LuaEngine::Listener*>* _listeners;
};

class NullLuaEngine : public LuaEngine
{
public:
	NullLuaEngine() {}
	~NullLuaEngine() {}
	LuaScript* createScript(std::string Path)
	{
		return new LuaScript();
	}
	void deleteScript(LuaScript* script)
	{
	}
	void initialize()
	{

	}
	void update()
	{
		
	}
	void destroy()
	{
	}
};



