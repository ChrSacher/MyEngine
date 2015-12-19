#pragma once
#include <chaiscript\chaiscript.hpp>
#include <chaiscript\chaiscript_stdlib.hpp>
#include <string>
#include <vector>
#include "Math\3DMath.h"
#include "Timer.h"
#include <exception>

using namespace chaiscript;
//a script which will be used by the Engine and regulary updated
class ChaiEngine;
class Script
{
public:
	friend ChaiEngine;
	//Path to LuaScript which will be used
	Script(std::string &Path);
	// start the startUp function if exists
	void begin()
	{
		try 
		{
			if (isValid)	state("startUp()");
		}
		catch(std::exception e)
		{
		}
	}
	//reload the Script with new state and file
	//new objects need to be loaded in again
	void reload(std::string &Path)
	{
		path = Path;
		state.use(Path);
	}
	void end()
	{
		try
		{
			if (isValid) state("shutDown()");
		}
		catch (std::exception e)
		{
		}
	}
	~Script()
	{
		
	}
	//uuse update function
	void update()
	{
		if (!hasTestRun)
		{
			try
			{
				updateFun();
				
			}
			catch (const std::exception &e)
			{
				std::cout << e.what() << std::endl;
				hasTestRun = true;
				hasUpdate = false;
				return;
			}
		}
		else
		{
			if (hasUpdate)
			{
				
				updateFun();
			}
		}
		
	}
	chaiscript::ChaiScript& getState() { return state; }
	void operator=(Script& other) {}
	Script(Script& script);
protected:
	unsigned int ID;
	chaiscript::ChaiScript state;
	std::string path;
	std::string updateS;
	bool hasUpdate;
	bool hasTestRun;
	bool isValid;
	std::function<void() > updateFun;
private:

};
struct ChaiPosition
{
	ChaiPosition(unsigned int Pos): pos(Pos){ static unsigned int id = 0; ID = id++; }
	unsigned int pos;
	unsigned int ID;
	Script* get();
	void destroy();
};
//Class for keeping track and updating all scripts
class LuaEngine
{
public:
	friend Script;
	LuaEngine();
	~LuaEngine() {}
	bool init;
	ChaiPosition* createScript(std::string Path);
	class Listener
	{
	public:
		Listener() { static unsigned int i = 0; ListenerID = i++; }
		unsigned int ListenerID;
		virtual ~Listener() { }
		virtual void scriptCreated(Script* script) = 0;
	};
	void scriptCreated(Script* script);
	void addListener(LuaEngine::Listener* listener);
	void removeListener(LuaEngine::Listener* listener);
	void deleteScript(ChaiPosition* script);
	Script* find(ChaiPosition* pos);
	void initialize();
	void update();
	void destroy();
protected:
	std::vector<Script> Scripts;
	std::map<unsigned int, ChaiPosition*> positions;
	std::list<LuaEngine::Listener*> _listeners;
};

class NullLuaEngine : public LuaEngine
{
public:
	NullLuaEngine() {}
	~NullLuaEngine() {}
	ChaiPosition* createScript(std::string Path)
	{
		return NULL;
	}
	void deleteScript(Script* script)
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



