#pragma once

#include <unordered_map>
#include <map>
#include "Math/3DMath.h"
#include <SDL.h>
#include "Commands.h"

class InputManager
{

};

class InputHandler
{
public:
	InputHandler(){}
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
	bool wasKeyDown(unsigned int keyID) ;
	void setMouseCoords(float x,float y);
	void handle(SDL_Event &e);
	Vector2 getMouseCoords() const {return _mousecoords;};
	std::vector<unsigned int> keyListPressed; //will be cleared once Handler got them
	Command* getCommand(GLuint ID);
private:
	std::unordered_map<unsigned int,bool> _keyMap;
	std::unordered_map<unsigned int,bool> _previousKeyMap;
	bool isChanged;
	Vector2 _mousecoords;
	std::map<unsigned int,Command*> commands;
	std::vector<Command*> lastCommands;

};

