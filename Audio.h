#pragma once
#include <conio.h>
#include <yse\yse.hpp>
#include "Errors.h"
#include <vector>
#include "Math\3DMath.h"
#include <map>
#include <SDL.h>
#include <tchar.h>
#include <fstream>
#include <Windows.h>
#include <gl/glew.h>
#include <gl/GL.h>

//#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll
inline YSE::Vec Vec(Vector3 x)
{
	return YSE::Vec(x.x,x.y,x.z);
}
//class for all audio in the Engine
class Audio
{
public:
	Audio(void);
	/* Start the Engine and load all necessary things
	*/
	void initialize();
	/*
		Destroy all loaded things and all sounds which are currently loaded
	*/
	void destroy();
	~Audio(void);
	//Play 2d Sound
	// #return will return an Int with location of the current sound
	// if -1 then sound couldn't be loaded
	virtual YSE::sound* play2D(std::string audiopath, bool store = false, float Volume = 1.0f);
	//Play 3d Sound
	// #return will return an Int with location of the current sound
	// if -1 then sound couldn't be loaded
	virtual YSE::sound* play3D(std::string audiopath, Vector3 position, float Volume = 1.0f, bool store = false);
	void stopSounds();
	void pauseSounds(bool paused);
	void setPos(Vector3 Pos = Vector3(), Vector3 Dir = Vector3(0, 0, -1), Vector3 Vel = Vector3(), Vector3 Up = Vector3(0, 0, 1));
	void update();

protected:
	std::map<int, YSE::sound*> info;
	std::map<int, YSE::sound*> empty;
	GLuint last;
	Vector3 pos;
	YSE::sound dummy;
};



class NullAudio: public Audio
{
public:
	YSE::sound* play2D(std::string audiopath,bool startpaused = false,float Volume = 1.0f,float timetolive = 999999)
	{

	}
	YSE::sound* play3D(std::string audiopath,Vector3 position,float Volume = 1.0f,bool startpaused = false,float timetolive = 99999999)
	{

	}
	void stopSounds()
	{
	}
	void pauseSounds(bool paused)
	{
	}
	void update(Vector3 Pos = Vector3(), Vector3 Dir = Vector3(0, 0, -1), Vector3 Vel = Vector3(), Vector3 Up = Vector3(0, 0, 1))
	{
	}
	void initialize()
	{
	};
	void destroy()
	{
	}
};

class LoggedAudio: public Audio
{
public:
	LoggedAudio(void);
	~LoggedAudio(void);
	YSE::sound* play2D(std::string audiopath, bool store = false, float Volume = 1.0f) override;
	YSE::sound* play3D(std::string audiopath, Vector3 position, float Volume = 1.0f, bool store = false) override;
	void stopSounds();
	void pauseSounds(bool paused);
};
