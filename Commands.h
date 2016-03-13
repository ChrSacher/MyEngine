#pragma once
#include "Math/3DMath.h"
#include "Camera3d.h"
#include "RenderUtil.h"
#include "Scene.h"
#include "Window.h"
#include "MusicPlayer.h"
#include "Timer.h"
class Window;
class Scene;

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
	KEYRELEASED // no use yet
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

struct SceneCommand : public Command
{
	void setScene(Scene* Scene)
	{
		scene = Scene;
	}
public:
	Scene* scene;
};


struct WindowCommand : public Command
{
	void setWindow(Window* Window)
	{
		window = Window;
	}
	Window* window;
};


struct MusicPlayerCommand : public Command
{
	void setCamera(MusicPlayer *music)
	{
		player = music;
	}
	MusicPlayer *player;
};


struct SceneAddObject :public SceneCommand
{
	SceneAddObject(Scene* Scene)
	{
		scene = Scene;
		type = KEYPRESSED;
	}
	virtual void execute()
	{
		Entity* newEntity = ServiceLocator::getEM().create("BOX",scene->getCamera()->getPos());
		newEntity->addComponent(ServiceLocator::getCM().createGraphics(std::string(""), std::string("res/Texture/normal_map.jpg"), std::string("res/models/box.obj")));
		scene->addEntity(newEntity);
	}
};

struct WindowFullScreen : public WindowCommand
{
	WindowFullScreen(Window* Window)
	{
		window = Window;
		type = KEYPRESSED;
	}
	virtual void execute()
	{
		static bool windowed = true;
		if(windowed)
		{
			window->SetFullScreen(true);
			windowed = false;
		}
		else
		{
			window->SetFullScreen(false);
			windowed = true;
		}
	}
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
		cam->raise(50 * Time::delta);
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
		cam->sink(50 * Time::delta);
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
		cam->straferight(50 * Time::delta);
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
		cam->strafeleft(50 * Time::delta);
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
		cam->moveforward(50 * Time::delta);
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
		cam->movebackward(50 * Time::delta);
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
		player->pause();
	
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