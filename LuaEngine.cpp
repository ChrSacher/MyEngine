#pragma once
#include "LuaEngine.h"
#include "GameState.h"
#include "ServiceLocator.h"
int SeScript::eventHandlerID = 0;

SeScript* ChaiPosition::get()
{
	return ServiceLocator::getLua().find(this);
}
void ChaiPosition::destroy()
{
	ServiceLocator::getLua().deleteScript(this);
}

SeScript::SeScript(std::string &Path) :hasTestRun(false), state(chaiscript::Std_Lib::library()), path(Path), updateS("update()"), hasUpdate(true)
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
	for (int i = 0; i < numEvents; i++)
	{
		eventHandlers.push_back(std::map<int,std::string>());
	}
	state.add(user_type<GameState>(), "gameState");
	state.add(fun(&GameState::cameramove), "cameraMove");
	state.add(fun(&GameState::drawgrid), "drawGrid");
	state.add(fun(&GameState::escapemenu), "escapeMenu");
	state.add(fun(&GameState::mainmenu), "mainMenu");
	state.add(fun(&GameState::paused), "paused");
	state.add(fun(&GameState::playing), "playing");
	state.add(fun(&GameState::ray), "ray");
	state.add_global(var(&GameState::state), "state");




	state.add(user_type<Vector2>(), "vec2");
	state.add(constructor<Vector2(float ,float)>(), "vec2");
	state.add(constructor<Vector2()>(), "vec2");
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
	state.add(constructor<Vector3(float, float, float)>(), "vec3");
	state.add(constructor<Vector3()>(), "vec3");
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
	state.add(constructor<Vector4(float, float, float,float )>(), "vec4");
	state.add(constructor<Vector4()>(), "vec4");
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

	state.add_global_const(const_var(&Time::delta), "delta");
	state.add_global_const(const_var(&Time::counter), "counter");
	state.add_global_const(const_var(&Time::start), "timeSinceStart");
	updateFun = state.eval<std::function<void()>>("update");
	begin();

}
void SeScript::end()
{
	try
	{
		if (isValid) state("shutDown()");
	}
	catch (std::exception e)
	{
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
void LuaEngine::ScriptCreated(SeScript* SeScript)
{
	for (std::list< LuaEngine::Listener*>::iterator &itr = _listeners.begin(); itr != _listeners.end(); ++itr)
	{
		 LuaEngine::Listener* listener = (*itr);
		listener->ScriptCreated(SeScript);
	}
}

void LuaEngine::addListener( LuaEngine::Listener* listener)
{
	_listeners.push_back(listener);
}

void LuaEngine::removeListener( LuaEngine::Listener* listener)
{

		for (std::list< LuaEngine::Listener*>::iterator &itr = _listeners.begin(); itr != _listeners.end(); ++itr)
		{
			if ((*itr) == listener)
			{
				_listeners.erase(itr);
				break;
			}
		}
}

void write(Vector2 x)
{
	std::cout << x.x << "  " << x.y << std::endl;
}
ChaiPosition*  LuaEngine::createScript(std::string &Path)
{
	Scripts.emplace_back(Path);
	ScriptCreated(&Scripts[Scripts.size() - 1]);
	ChaiPosition* temp = new ChaiPosition(Scripts.size() - 1);
	positions.insert(std::make_pair(temp->ID, temp));
	return temp;
}

void LuaEngine::deleteScript(ChaiPosition* Script)
{
	auto pos = positions.find(Script->ID);
	if (pos != positions.end())
	{
		int i = (pos->second->pos);
		Scripts[i].end();
		//delete(Scripts[i]);
		Scripts.erase(Scripts.begin() + i);
		
		for (auto &it = pos; it != positions.end(); it++)
		{
			it->second->pos -= 1;
		}
		positions.erase(pos);
		delete (Script);
		
		
	}
}

SeScript*  LuaEngine::find(ChaiPosition* pos)
{
	auto r = positions.find(pos->ID);
	if (r != positions.end()) return &Scripts[r->second->pos];
	return NULL;
}
LuaEngine::LuaEngine() { init = false; Scripts.reserve(50); }

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
