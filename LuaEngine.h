#pragma once
#include <chaiScript\chaiScript.hpp>
#include <chaiScript\chaiScript_stdlib.hpp>
#include <string>
#include <vector>
#include "Math\3DMath.h"
#include "Timer.h"
#include <exception>
#include "MessageEvent.h"
using namespace chaiscript;
//a SeScript which will be used by the Engine and regulary updated
class ChaiEngine;
class SeScript
{
public:
	friend ChaiEngine;
	//Path to LuaScript which will be used
	SeScript(std::string &Path);
	// start the startUp function if exists
	void begin();
	int addEventHandler(std::string &Type, std::string &function, bool isFunction = false);
	int addEventHandler(MessageEventType Type, std::string &function, bool isFunction = false);
	//reload the SeScript with new state and file
	//new objects need to be loaded in again
	void reload(std::string &Path);
	void end();
	~SeScript()
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
	SeScript& operator=(const SeScript& other) { return *this; };
	SeScript(const SeScript& other) {};
protected:
	unsigned int ID;
	chaiscript::ChaiScript state;
	std::string path;
	std::string updateS;
	bool hasUpdate;
	bool hasTestRun;
	bool isValid;
	std::function<void() > updateFun;
	static int eventHandlerID;

private:
	std::vector<std::map<int,std::string>> eventHandlers;
	
};

struct ChaiPosition
{
	ChaiPosition(unsigned int Pos): pos(Pos){ static unsigned int id = 0; ID = id++; }
	unsigned int pos;
	unsigned int ID;
	SeScript* get();
	void destroy();
};
//Class for keeping track and updating all Scripts
class LuaEngine
{
public:
	friend SeScript;
	LuaEngine();
	~LuaEngine() {}
	bool init;
	ChaiPosition* createScript(std::string &Path);
	class Listener
	{
	public:
		Listener() { static unsigned int i = 0; ListenerID = i++; }
		unsigned int ListenerID;
		virtual ~Listener() { }
		virtual void ScriptCreated(SeScript* Script) = 0;
	};
	void ScriptCreated(SeScript* Script);
	void addListener(LuaEngine::Listener* listener);
	void removeListener(LuaEngine::Listener* listener);
	void deleteScript(ChaiPosition* SeScript);
	SeScript* find(ChaiPosition* pos);
	void initialize();
	void update();
	void destroy();
protected:
	std::vector<SeScript> Scripts;
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
	void deleteScript(SeScript* SeScript)
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



