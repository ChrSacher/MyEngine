#include "InputManager.h"

void InputHandler::scriptCreated(Script* script)
{

	ChaiScript &Script = script->getState();
	Script.add(user_type<InputHandler>(), "InputHandler");
	Script.add_global(var(this), "Input");
	Script.add(fun(&InputHandler::isKeyDownS), "isKeyDown");
	Script.add(fun(&InputHandler::isKeyPressedS), "isKeyPressed");
	Script.add(fun(&InputHandler::isKeyReleasedS), "isKeyReleased");
	Script.add(fun(&InputHandler::getMouseCoords), "getMouseCoords");
};

void InputHandler::handle(SDL_Event &e)
{
		switch(e.type)
		{
			case SDL_MOUSEMOTION:
			{
				setMouseCoords(e.motion.x,e.motion.y);
			};
			case SDL_KEYUP:
			{
				releaseKey(e.key.keysym.sym);
			}break;
			case SDL_KEYDOWN:
			{
				pressKey(e.key.keysym.sym);
			}break;
			case SDL_MOUSEBUTTONDOWN:
			{
				pressKey(e.button.button);
			};break;
			case SDL_MOUSEBUTTONUP:
			{
				releaseKey(e.button.button);
			};break;
			
		}
		isChanged = true;
}
void InputHandler::update()
{
	for (auto& it : _keyMap)
	{
        _previousKeyMap[it.first] = it.second;
	}
}

void InputHandler::pressKey(unsigned int keyID)
{
	_keyMap[keyID]=true;
}

void InputHandler::releaseKey(unsigned int keyID)
{
	_keyMap[keyID]=false;
}

bool InputHandler::isKeyDown(unsigned int keyID)
{
	auto it=_keyMap.find(keyID);
	if(it != _keyMap.end())
	{
		return it->second;
	}
	else
	{
		return false;
	}
}

void InputHandler::setMouseCoords(float x,float y)
{
	_mousecoords[0] = x;
	_mousecoords[1] = y;
}

bool InputHandler::isKeyPressed(unsigned int keyID)
{
	if (isKeyDown(keyID) == true && wasKeyDown(keyID) == false)
	{
        return true;
    }
    return false;
}
bool InputHandler::isKeyReleased(unsigned int keyID)
{
	if (isKeyDown(keyID) != true && wasKeyDown(keyID) != false)
	{
        return true;
    }
    return false;
}
bool InputHandler::wasKeyDown(unsigned int keyID) 
{
    // We dont want to use the associative array approach here
    // because we don't want to create a key if it doesnt exist.
    // So we do it manually
    auto it = _previousKeyMap.find(keyID);
    if (it != _previousKeyMap.end()) 
	{
        // Found the key
        return it->second;
    } 
	else 
	{
        // Didn't find the key
        return false;
    }
}


void InputHandler::generate_input(std::vector<Command*> &command_queue)
{
	for (auto& it : _keyMap)
	{
		if(it.second)
		{
			auto& r = commands.find(it.first);
			if(r != commands.end())
			{
				switch(r->second->type)
				{
					case KEYDOWN:
					{
						if(isKeyDown(it.first)) keyListPressed.push_back(it.first);

					}break;
					case KEYPRESSED:
					{
						if(isKeyPressed(it.first)) keyListPressed.push_back(it.first);
					}break;
					case KEYRELEASED:
					{
						if(isKeyReleased(it.first)) keyListPressed.push_back(it.first);
					};break;
					default:
					{
						std::cout<<"Unknown Input Type"<<std::endl;
					}
				}
			}
		}
	}
	static unsigned int maxCommandsRemembered = 50;
	if(keyListPressed.size() == 0) return;
	if(lastCommands.size() > maxCommandsRemembered)
	{
		int toReduce = lastCommands.size() - maxCommandsRemembered;
		lastCommands.erase(lastCommands.begin(),lastCommands.begin() + toReduce);
	}
	for(unsigned int i = 0; i < keyListPressed.size();i++)
	{
		if(commands[keyListPressed[i]] == NULL) continue;
		command_queue.push_back(commands[keyListPressed[i]]);
		lastCommands.push_back(commands[keyListPressed[i]]);
	}
	keyListPressed.clear();
	
}
void InputHandler::bind(unsigned int key, Command* command)
{
	auto it = commands.find(key);
	if(it != commands.end())
	{
		delete(it->second);//delete if existing
		commands.erase(key);
	}
	commands.insert(std::make_pair(key,command));    // key points to newly assigned command )
}

Command* InputHandler::getCommand(GLuint ID)
{
	auto r = commands.find(ID);
	if(r == commands.end()) return NULL;
	return r->second;
}

bool InputHandler::isKeyDownS(const std::string &string)
{
	auto &r = stringKeyMap.find(string);
	if (r == stringKeyMap.end()) return false;
	return isKeyDown(r->second);
}

bool InputHandler::isKeyPressedS(const std::string &string)
{
	auto &r = stringKeyMap.find(string);
	if (r == stringKeyMap.end()) return false;
	return isKeyPressed(r->second);
}
bool InputHandler::isKeyReleasedS(const std::string &string)
{
	auto &r = stringKeyMap.find(string);
	if (r == stringKeyMap.end()) return false;
	return isKeyReleased(r->second);
}