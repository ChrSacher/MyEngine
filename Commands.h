#pragma once
#include "Math/3DMath.h"
#include "Camera3d.h"
#include "Audio.h"
#include "RenderUtil.h"
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

//put Commands
struct CameraCommand : public Command
{
	void setCamera(Camera3d* Cam)
	{
		cam = Cam;
	}
	Camera3d *cam;
};

struct MusicPlayerCommand : public Command
{
	void setCamera(MusicPlayer *music)
	{
		player = music;
	}
	MusicPlayer *player;
};

struct CameraMoveUp : public CameraCommand
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
	
};

struct CameraMoveDown : public CameraCommand
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
};
struct CameraMoveRight: public CameraCommand
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
struct CameraMoveForward : public CameraCommand
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
};
struct CameraMoveBackward : public CameraCommand
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
};


struct switchRender:public Command
{
	switchRender()
	{
		type = KEYPRESSED;
	}
	virtual void execute()
	{
		static bool type = true;
		if(type)
		{
			util.renderInWireframe();
			type = false;
		}
		else
		{
			util.renderInPolygone();
			type = true;
		}
		
	}
};

struct PlayMusic: public MusicPlayerCommand
{
	PlayMusic(MusicPlayer* music)
	{
		type = KEYPRESSED;
		player = music;
	}
	virtual void execute()
	{
		if(!player->isPaused())
		{
			player->pause();
		}
		else
		{
			player->unpause();
		}
	
	}

};

struct PreviousMusic: public MusicPlayerCommand
{
	PreviousMusic(MusicPlayer* music)
	{
		type = KEYPRESSED;
		player = music;
	}
	virtual void execute()
	{
		player->previousSong();
	}

};

struct NextMusic: public MusicPlayerCommand
{
	NextMusic(MusicPlayer* music)
	{
		type = KEYPRESSED;
		player = music;
	}
	virtual void execute()
	{
		player->nextSong();
	}

};