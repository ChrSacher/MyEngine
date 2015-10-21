#pragma once

#include <unordered_map>
#include <map>
#include "Math/3DMath.h"
#include <SDL.h>
#include "Commands.h"
#include "LuaEngine.h"

class InputHandler :public LuaEngine::Listener
{
public:
	void scriptCreated(LuaScript* script);
	InputHandler(){generateKeyMap();}
	  ~InputHandler()
	  {
		  for(auto &i = commands.begin(); i != commands.end();i++)
		  {
			 delete(i->second);
		  }
	  }
	
	void bind(unsigned int key, Command* command);
	void generate_input(std::vector<Command*> &command_queue);
	
public:

	void update();
	void pressKey(unsigned int keyID);
	void releaseKey(unsigned int keyID);
	bool isKeyDown(unsigned int keyID);
	bool isKeyPressed(unsigned int keyID);
	bool isKeyReleased(unsigned int keyID);
	static InputHandler& get() { static InputHandler in; return in; }
bool isKeyDownS(const std::string &string);//copy because lua to c++
bool isKeyPressedS(const std::string &string);
bool isKeyReleasedS(const std::string &string);

//this is needed for LUA
void generateKeyMap()
{
	
	stringKeyMap.insert(std::make_pair("BS",SDLK_BACKSPACE));
	stringKeyMap.insert(std::make_pair("TAB",SDLK_TAB));
	stringKeyMap.insert(std::make_pair("RETURN",SDLK_RETURN));
	stringKeyMap.insert(std::make_pair("PAUSE",SDLK_PAUSE));
	stringKeyMap.insert(std::make_pair("ESC",SDLK_ESCAPE));
	stringKeyMap.insert(std::make_pair(" ",SDLK_SPACE));
	stringKeyMap.insert(std::make_pair(",",SDLK_COMMA));
	stringKeyMap.insert(std::make_pair("-",SDLK_MINUS));
	stringKeyMap.insert(std::make_pair(".",SDLK_PERIOD));
	stringKeyMap.insert(std::make_pair("/",SDLK_SLASH));
	stringKeyMap.insert(std::make_pair("0",SDLK_0));
	stringKeyMap.insert(std::make_pair("1",SDLK_1));
	stringKeyMap.insert(std::make_pair("2",SDLK_2));
	stringKeyMap.insert(std::make_pair("3",SDLK_3));
	stringKeyMap.insert(std::make_pair("4",SDLK_4));
	stringKeyMap.insert(std::make_pair("5",SDLK_5));
	stringKeyMap.insert(std::make_pair("6",SDLK_6));
	stringKeyMap.insert(std::make_pair("7",SDLK_7));
	stringKeyMap.insert(std::make_pair("8",SDLK_8));
	stringKeyMap.insert(std::make_pair("9",SDLK_9));
	stringKeyMap.insert(std::make_pair(":",SDLK_COLON));
	stringKeyMap.insert(std::make_pair(";",SDLK_SEMICOLON));
	stringKeyMap.insert(std::make_pair("=",SDLK_EQUALS));
	stringKeyMap.insert(std::make_pair("[",SDLK_LEFTBRACKET));
	stringKeyMap.insert(std::make_pair("BACKSLASH",SDLK_BACKSLASH));
	stringKeyMap.insert(std::make_pair("a",SDLK_a));
	stringKeyMap.insert(std::make_pair("b",SDLK_b));
	stringKeyMap.insert(std::make_pair("c",SDLK_c));
	stringKeyMap.insert(std::make_pair("d",SDLK_d));
	stringKeyMap.insert(std::make_pair("e",SDLK_e));
	stringKeyMap.insert(std::make_pair("f",SDLK_f));
	stringKeyMap.insert(std::make_pair("g",SDLK_g));
	stringKeyMap.insert(std::make_pair("h",SDLK_h));
	stringKeyMap.insert(std::make_pair("i",SDLK_i));
	stringKeyMap.insert(std::make_pair("j",SDLK_j));
	stringKeyMap.insert(std::make_pair("k",SDLK_k));
	stringKeyMap.insert(std::make_pair("l",SDLK_l));
	stringKeyMap.insert(std::make_pair("m",SDLK_m));
	stringKeyMap.insert(std::make_pair("n",SDLK_n));
	stringKeyMap.insert(std::make_pair("o",SDLK_o));
	stringKeyMap.insert(std::make_pair("p",SDLK_p));
	stringKeyMap.insert(std::make_pair("q",SDLK_q));
	stringKeyMap.insert(std::make_pair("r",SDLK_r));
	stringKeyMap.insert(std::make_pair("s",SDLK_s));
	stringKeyMap.insert(std::make_pair("t",SDLK_t));
	stringKeyMap.insert(std::make_pair("u",SDLK_u));
	stringKeyMap.insert(std::make_pair("v",SDLK_v));
	stringKeyMap.insert(std::make_pair("w",SDLK_w));
	stringKeyMap.insert(std::make_pair("x",SDLK_x));
	stringKeyMap.insert(std::make_pair("y",SDLK_y));
	stringKeyMap.insert(std::make_pair("z",SDLK_z));
	stringKeyMap.insert(std::make_pair("DEL",SDLK_DELETE));
	stringKeyMap.insert(std::make_pair("KP.",SDLK_KP_PERIOD));
	stringKeyMap.insert(std::make_pair("KP/",SDLK_KP_DIVIDE));
	stringKeyMap.insert(std::make_pair("KP-",SDLK_KP_MINUS));
	stringKeyMap.insert(std::make_pair("KP*",SDLK_KP_MULTIPLY));
	stringKeyMap.insert(std::make_pair("KP+",SDLK_KP_PLUS));
	stringKeyMap.insert(std::make_pair("KPENT",SDLK_KP_ENTER));
	stringKeyMap.insert(std::make_pair("KP=",SDLK_KP_EQUALS));
	stringKeyMap.insert(std::make_pair("UP",SDLK_UP));
	stringKeyMap.insert(std::make_pair("DOWN",SDLK_DOWN));
	stringKeyMap.insert(std::make_pair("RIGHT",SDLK_RIGHT));
	stringKeyMap.insert(std::make_pair("LEFT",SDLK_LEFT));
	stringKeyMap.insert(std::make_pair("INSERT",SDLK_INSERT));
	stringKeyMap.insert(std::make_pair("HOME",SDLK_HOME));
	stringKeyMap.insert(std::make_pair("END",SDLK_END));
	stringKeyMap.insert(std::make_pair("PAGEUP",SDLK_PAGEUP));
	stringKeyMap.insert(std::make_pair("PAGEDOWN",SDLK_PAGEDOWN));
	stringKeyMap.insert(std::make_pair("F1",SDLK_F1));
	stringKeyMap.insert(std::make_pair("F2",SDLK_F2));
	stringKeyMap.insert(std::make_pair("F3",SDLK_F3));
	stringKeyMap.insert(std::make_pair("F4",SDLK_F4));
	stringKeyMap.insert(std::make_pair("F5",SDLK_F5));
	stringKeyMap.insert(std::make_pair("F6",SDLK_F6));
	stringKeyMap.insert(std::make_pair("F7",SDLK_F7));
	stringKeyMap.insert(std::make_pair("F8",SDLK_F8));
	stringKeyMap.insert(std::make_pair("F9",SDLK_F9));
	stringKeyMap.insert(std::make_pair("F10",SDLK_F10));
	stringKeyMap.insert(std::make_pair("F11",SDLK_F11));
	stringKeyMap.insert(std::make_pair("F12",SDLK_F12));
	stringKeyMap.insert(std::make_pair("RSHIFT",SDLK_RSHIFT));
	stringKeyMap.insert(std::make_pair("LSHIFT",SDLK_LSHIFT));
	stringKeyMap.insert(std::make_pair("RCTRL",SDLK_RCTRL));
	stringKeyMap.insert(std::make_pair("LCTRL",SDLK_LCTRL));
	stringKeyMap.insert(std::make_pair("RALT",SDLK_RALT));
	stringKeyMap.insert(std::make_pair("LALT",SDLK_LALT));
	stringKeyMap.insert(std::make_pair("SYSREQ",SDLK_SYSREQ));
	stringKeyMap.insert(std::make_pair("MENU",SDLK_MENU));
	stringKeyMap.insert(std::make_pair("POWER",SDLK_POWER));
	
}
	bool wasKeyDown(unsigned int keyID) ;
	void setMouseCoords(float x,float y);
	void handle(SDL_Event &e);
	Vector2 getMouseCoords() {return _mousecoords;};
	std::vector<unsigned int> keyListPressed; //will be cleared once Handler got them
	Command* getCommand(GLuint ID);
private:
	std::unordered_map<unsigned int,bool> _keyMap;
	std::unordered_map<unsigned int,bool> _previousKeyMap;
	std::map<std::string,unsigned int> stringKeyMap;
	bool isChanged;
	Vector2 _mousecoords;
	std::map<unsigned int,Command*> commands;
	std::vector<Command*> lastCommands;

};

