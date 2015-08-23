#pragma once
#include "Math/3DMath.h"
#include "Camera3d.h"
#include "RenderUtil.h"
#include "Scene.h"
#include "Window.h"
#include "MusicPlayer.h"
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
		Entity* newEntity = Entity::create(scene->getCamera()->getPos());
		newEntity->addComponent(GraphicsComponent::create("","res/texture/normal_map.jpg","res/models/box.obj"));
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