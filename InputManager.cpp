#include "InputManager.h"

InputManager::InputManager(void)
{
}


InputManager::~InputManager(void)
{
}

void InputManager::update()
{
	for (auto& it : _keyMap)
	{
        _previousKeyMap[it.first] = it.second;
	}
}

void InputManager::pressKey(unsigned int keyID)
{
	_keyMap[keyID]=true;
}

void InputManager::releaseKey(unsigned int keyID)
{
	_keyMap[keyID]=false;
}

bool InputManager::isKeyDown(unsigned int keyID)
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

void InputManager::setMouseCoords(float x,float y)
{
	_mousecoords[0] = x;
	_mousecoords[1] = y;
}

bool InputManager::isKeyPressed(unsigned int keyID)
{
	if (isKeyDown(keyID) == true && wasKeyDown(keyID) == false)
	{
        return true;
    }
    return false;
}

bool InputManager::wasKeyDown(unsigned int keyID) 
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