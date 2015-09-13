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
	LuaEngine() {}
	LuaScript* createScript(std::string Path)
	{
		  scripts.push_back(new LuaScript(Path));
		  return scripts.back();
	}
	std::vector<LuaScript*> scripts;
	void startUp()
	{

	}
	void update()
	{
		for(unsigned int i = 0;i < scripts.size();i++)
		{
			scripts[i]->update();
		}
	}
	void shutDown()
	{
		for(unsigned int i = 0;i < scripts.size();i++)
		{
			  delete(scripts[i]);
		}		
	}	
};




