#pragma once
#include <chaiscript\chaiscript.hpp>
#include <chaiscript\chaiscript_stdlib.hpp>
#include <string>
#include <vector>
#include "Math\3DMath.h"
#include "Time.h"
#include <exception>

using namespace chaiscript;
//a script which will be used by the Engine and regulary updated
class LuaEngine;
class LuaScript
{
public:
	friend LuaEngine;
	//Path to LuaScript which will be used
	LuaScript(std::string Path) :hasTestRun(false),state(chaiscript::Std_Lib::library()), path(Path),updateS("update()"), hasUpdate(true)
	{
		isValid = true;
		static unsigned int I = 0;
		ID = ++I;
		try
		{
			state.eval_file(Path);
		}
		catch (const std::exception &e)
		{
			std::cout << e.what() << std::endl;
			isValid = false;
			hasUpdate = false;
			hasTestRun = true;
			return;
		}
		state.add(user_type<Vector2>(), "vec2");
		state.add(fun(&Vector2::x), "x");
		state.add(fun(&Vector2::y), "y");
		state.add(fun(&Vector2::distance), "distance");
		state.add(fun(&Vector2::dot), "dot");
		state.add(fun(&Vector2::set), "set");
		state.add(fun(&Vector2::length), "length");
		state.add(fun(&Vector2::normalize), "normalize");
		state.add(fun(&Vector2::operator+), "+");
		state.add(fun(&Vector2::operator/), "/");
		state.add(fun(&Vector2::mul), "*");
		state.add(fun(&Vector2::sub), "-");
		state.add(fun(&Vector2::operator==), "==");
		state.add(fun(&Vector2::operator!=), "!=");

		state.add(user_type<Vector3>(), "vec3");
		state.add(fun(&Vector3::x), "x");
		state.add(fun(&Vector3::y), "y");
		state.add(fun(&Vector3::z), "z");
		state.add(fun(&Vector3::distance), "distance");
		state.add(fun(&Vector3::dot), "dot");
		state.add(fun(&Vector3::set), "set");
		state.add(fun(&Vector3::length), "length");
		state.add(fun(&Vector3::normalize), "normalize");
		state.add(fun(&Vector3::operator+), "+");
		state.add(fun(&Vector3::operator/), "/");
		state.add(fun(&Vector3::mul), "*");
		state.add(fun(&Vector3::sub), "-");
		state.add(fun(&Vector3::operator==), "==");
		state.add(fun(&Vector3::operator!=), "!=");

		state.add(user_type<Vector4>(), "vec4");
		state.add(fun(&Vector4::distance), "distance");
		state.add(fun(&Vector4::x), "x");
		state.add(fun(&Vector4::y), "y");
		state.add(fun(&Vector4::z), "z");
		state.add(fun(&Vector4::w), "w");

		state.add(fun(&Vector4::dot), "dot");
		state.add(fun(&Vector4::set), "set");
		state.add(fun(&Vector4::length), "length");
		state.add(fun(&Vector4::normalize), "normalize");
		state.add(fun(&Vector4::operator+), "+");
		state.add(fun(&Vector4::operator/), "/");
		state.add(fun(&Vector4::mul), "*");
		state.add(fun(&Vector4::sub), "-");
		state.add(fun(&Vector4::operator==), "==");
		state.add(fun(&Vector4::operator!=), "!=");

		state.add_global_const(const_var(Time::delta),"delta");
		begin();
	}
	// start the startUp function if exists
	void begin()
	{
		//if(isValid)	state("startUp()");
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
		//if (isValid) state("shutDown()");
	}
	~LuaScript()
	{
		
	}
	//uuse update function
	void update()
	{
		if (!hasTestRun)
		{
			try
			{
				state(updateS);
				
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
				state(updateS);
			}
		}
		
	}
	chaiscript::ChaiScript& getState() { return state; }
	void operator=(LuaScript& other) {}
	LuaScript(LuaScript& script);
protected:
	unsigned int ID;
	chaiscript::ChaiScript state;
	std::string path;
	std::string updateS;
	bool hasUpdate;
	bool hasTestRun;
	bool isValid;
	
private:

};
struct ChaiPosition
{
	ChaiPosition(unsigned int Pos): pos(Pos){ static unsigned int id = 0; ID = id++; }
	unsigned int pos;
	unsigned int ID;
};
//Class for keeping track and updating all scripts
class LuaEngine
{
public:
	friend LuaScript;
	LuaEngine();
	~LuaEngine() {}
	bool init;
	ChaiPosition* createScript(std::string Path);
	class Listener
	{
	public:
		virtual ~Listener() { }
		virtual void scriptCreated(LuaScript* script) = 0;
	};
	void scriptCreated(LuaScript* script);
	void addListener(LuaEngine::Listener* listener);
	void removeListener(LuaEngine::Listener* listener);
	void deleteScript(ChaiPosition* script);
	LuaScript* find(ChaiPosition* pos);
	void initialize();
	void update();
	void destroy();
protected:
	std::vector<LuaScript> Scripts;
	std::map<unsigned int, ChaiPosition*> positions;
	std::list<LuaEngine::Listener*>* _listeners;
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



