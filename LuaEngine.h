#pragma once

#include <selene.h>
#include <string>
#include <vector>
using namespace sel;
class LuaScript
{
public:
	LuaScript(std::string Path) :state(true), path(Path)
	{
		state.Load(Path);
		state["startUp"]();
		updateFun = state["update"];
	}
	LuaScript() :state(true),path("NA")
	{
	}
	~LuaScript()
	{
		state["shutDown"]();
	}
	void update()
	{
		state["update"]();
	}
	void executeFile(std::string Path)
	{
		path = Path;
	}
	void reload()
	{
		state = sel::State(true);
	}
	State* getState() { return &state; }
	State state;
	std::string path;
	std::map<std::string, Selector> variables;
	Selector* updateFun;
};
using namespace sel;

class LuaEngine
{
public:
	LuaEngine() {}
	LuaScript* createScript(std::string Path)
	{
		LuaScript* temp = new LuaScript(Path);
		scripts.insert(std::make_pair(temp,temp));
		return temp;
	}
	std::map<LuaScript*,LuaScript*> scripts;
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
};

class NullLuaEngine : public LuaEngine
{
	LuaScript* createScript(std::string Path)
	{
		return new LuaScript();
	}
	std::map<LuaScript*, LuaScript*> scripts;
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
}




