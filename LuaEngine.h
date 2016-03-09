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
	friend class ChaiEngine;
	friend class Scene;
	friend class ScriptComponent;
	friend class ScriptComponentSystem;
	//Path to LuaScript which will be used
	SeScript();
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







