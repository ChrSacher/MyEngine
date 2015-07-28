#pragma once

#include <unordered_map>
#include <map>
#include "Math/3DMath.h"
#include <SDL.h>
#include "Camera3d.h"
#include "Audio.h"
enum InputType
{
    ACTION,
    STATE,
    RANGE
};

enum Action
{
    EXECUTE = true,
    STOP = false
};

enum InputAcces
{
	KEYDOWN,
	KEYPRESSED,
	RELEASED // no use yet
};
struct Command
{
public:
  virtual ~Command() {}
  virtual void execute() = 0 ; //normally for your character
  InputAcces type;
};

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
	bool wasKeyDown(unsigned int keyID) ;
	void setMouseCoords(float x,float y);
	void handle(SDL_Event &e);
	Vector2 getMouseCoords() const {return _mousecoords;};
	std::vector<unsigned int> keyListPressed; //will be cleared once Handler got them
	
private:
	std::unordered_map<unsigned int,bool> _keyMap;
	std::unordered_map<unsigned int,bool> _previousKeyMap;
	bool isChanged;
	Vector2 _mousecoords;
	std::map<unsigned int,Command*> commands;
	std::vector<Command*> lastCommands;

};

//put Commands

struct CameraMoveUp : public Command
{
	CameraMoveUp(Camera3d *camera)
	{
		cam = camera;
		type = KEYDOWN;
	}
	virtual void execute()
	{
		cam->raise();
	}
	Camera3d *cam;
};

struct CameraMoveDown : public Command
{
	CameraMoveDown(Camera3d *camera)
	{
		cam = camera;
		type = KEYDOWN;
	}
	virtual void execute()
	{
		cam->sink();
	}
	Camera3d *cam;
};
struct CameraMoveRight: public Command
{
	CameraMoveRight(Camera3d *camera)
	{
		cam = camera;
		type = KEYDOWN;
	}
	virtual void execute()
	{
		cam->straferight();
	}
	Camera3d *cam;
};
struct CameraMoveLeft : public Command
{
	CameraMoveLeft(Camera3d *camera)
	{
		cam = camera;
		type = KEYDOWN;
	}
	virtual void execute()
	{
		cam->strafeleft();
	}
	Camera3d *cam;
};
struct CameraMoveForward : public Command
{
	CameraMoveForward(Camera3d *camera)
	{
		cam = camera;
		type = KEYDOWN;
	}
	virtual void execute()
	{
		cam->moveforward();
	}
	Camera3d *cam;
};
struct CameraMoveBackward : public Command
{
	CameraMoveBackward(Camera3d *camera)
	{
		cam = camera;
		type = KEYDOWN;
	}
	virtual void execute()
	{
		cam->movebackward();
	}
	Camera3d *cam;
};

struct PlayAudio: public Command
{
	PlayAudio(const Vector3 *pos)
	{
		type = KEYPRESSED;
		this->pos = pos;
	}
	virtual void execute()
	{
		Vector3 temp;
			temp.x = pos->x;
			temp.y = pos->y;
			temp.z = pos->z;
		audio::get().play3D("sound/Kalimba.mp3",temp,true,false);
	}
	const Vector3 *pos;
	const Vector3* getPos(){return pos;}
	void setPos(Vector3 *pos){this->pos = pos;}

};

struct StopAudio: public Command
{
	StopAudio()
	{
		type = KEYPRESSED;
	}
	virtual void execute()
	{
		
		audio::get().stopPlay(audio::get().last - 1);
		
	}

};