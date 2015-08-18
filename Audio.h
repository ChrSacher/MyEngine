#pragma once
#include <conio.h>
#include <irrKlang.h>
#include "Errors.h"
#include <vector>
#include "Math\3DMath.h"
#include "Camera3d.h"
#include <map>
#include <SDL.h>
#include <tchar.h>
#include <fstream>
#include <Windows.h>

#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll

struct SoundComponent
{
	float timetolive;
	irrklang::ISound *sound;
	SoundComponent(irrklang::ISound *sound,float TTL)
	{
		timetolive = TTL;
		this->sound = sound;
		toBeStopped = false;
	}
	void stop()
	{
		toBeStopped = true;
	}
	bool toBeStopped;
};

inline irrklang::vec3df vec3df(Vector3 x)
{
	return irrklang::vec3df(x.x,x.y,x.z);
}

class Audio
{
public:
	Audio(void)
	{
		
	};
	void initialize()
	{
		 engine =  irrklang::createIrrKlangDevice();
		 if (!engine) fatalError("Failed to create Audio Device");
		 engine->update();
		 last = 0;
	};
	void destroy()
	{
		for(auto &it = info.begin();it != info.end();it++)
		 {
			 delete(it->second);
		 }
		 engine->drop();
		 info.clear();
	}
	~Audio(void)
	{
		
		 
		
	}
	
	SoundComponent* play2D(std::string audiopath,bool startpaused = false,float Volume = 1.0f,float timetolive = 999999)
	{
		irrklang::ISound* temp = engine->play2D(audiopath.c_str(),false,startpaused,true);
		if(temp) 
		{
				temp->setVolume(Volume);
				SoundComponent* temp2 = new SoundComponent(temp,timetolive);
				info.insert(std::make_pair(++last,temp2));
				engine->update();
				return temp2;
		}
		return NULL;
		
	}
	SoundComponent* play3D(std::string audiopath,Vector3 position,float Volume = 1.0f,bool startpaused = false,float timetolive = 99999999)
	{
		irrklang::ISound* temp = engine->play3D(audiopath.c_str(),vec3df(position),false,startpaused,true);
		if(temp) 
		{
				temp->setVolume(Volume);
				SoundComponent* temp2 = new SoundComponent(temp,timetolive);
				info.insert(std::make_pair(++last,temp2));
				engine->update();
				return temp2;
		}
		return NULL;
	}
	void stopSounds()
	{
		engine->stopAllSounds();
		engine->update();
	}
	void pauseSounds(bool paused)
	{
		engine->setAllSoundsPaused(paused);
		engine->update();
	}
	void update(float delta,Camera3d* cam)
	{
		if(cam)
		{
			engine->setListenerPosition(irrklang::vec3df(cam->getPos().x,cam->getPos().y,cam->getPos().z),irrklang::vec3df(cam->getDir().x,cam->getDir().y,cam->getDir().z),irrklang::vec3df(),irrklang::vec3df(cam->getUp().x,cam->getUp().y,cam->getUp().z));
		}
		std::vector<GLuint> toErase;
		for(auto &it = info.begin(); it != info.end();it++)
		{
			it->second->timetolive -= delta/1000;
			if(it->second->timetolive < 0 || it->second->toBeStopped)
			{
				it->second->sound->stop();
			}
		}
		for(unsigned int i = 0; i < toErase.size();i++) info.erase(toErase[i]);
		

	}
private:
	irrklang::ISoundEngine* engine;
	std::map<GLuint,SoundComponent*> info;
	GLuint last;
	Vector3 pos;
};

struct SongInformation
{
	SongInformation(std::string &string,std::string &string2)
	{
		title = string;
		path = string2;
	}
	std::string title;
	std::string path;
};

class NullAudio: public Audio
{
public:
  SoundComponent* play2D(std::string audiopath,bool startpaused = false,float Volume = 1.0f,float timetolive = 999999)
	{
	}
	SoundComponent* play3D(std::string audiopath,Vector3 position,float Volume = 1.0f,bool startpaused = false,float timetolive = 99999999)
	{
	}
	void stopSounds()
	{
	}
	void pauseSounds(bool paused)
	{
	}
	void update(float delta,Camera3d* cam)
	{
	}
};

class LoggedAudio: public Audio
{
public:
	LoggedAudio(void)
	{
		 engine =  irrklang::createIrrKlangDevice();
		 if (!engine) fatalError("Failed to create Audio Device");
		 engine->update();
		 last = 0;
	};
	~LoggedAudio(void)
	{
		
		 for(auto &it = info.begin();it != info.end();it++)
		 {
			 delete(it->second);
		 }
		 engine->drop();
		 info.clear();
		
	}
	irrklang::ISoundEngine* engine;
	std::map<GLuint,SoundComponent*> info;
	GLuint last;
	Vector3 pos;
	SoundComponent* play2D(std::string audiopath,bool startpaused = false,float Volume = 1.0f,float timetolive = 999999)
	{
		irrklang::ISound* temp = engine->play2D(audiopath.c_str(),false,startpaused,true);
		if(temp) 
		{
				temp->setVolume(Volume);
				SoundComponent* temp2 = new SoundComponent(temp,timetolive);
				info.insert(std::make_pair(++last,temp2));
				std::cout << "Loading Sound " + audiopath<<std::endl;
				engine->update();
				return temp2;
		}
		return NULL;
		
	}
	SoundComponent* play3D(std::string audiopath,Vector3 position,float Volume = 1.0f,bool startpaused = false,float timetolive = 99999999)
	{
		irrklang::ISound* temp = engine->play3D(audiopath.c_str(),vec3df(position),false,startpaused,true);
		if(temp) 
		{
				temp->setVolume(Volume);
				SoundComponent* temp2 = new SoundComponent(temp,timetolive);
				info.insert(std::make_pair(++last,temp2));
				std::cout << "Loading Sound " + audiopath<<std::endl;
				engine->update();
				return temp2;
		}
		return NULL;
	}
	void stopSounds()
	{
		engine->stopAllSounds();
		engine->update();
	}
	void pauseSounds(bool paused)
	{
		engine->setAllSoundsPaused(paused);
		engine->update();
	}
	void update(float delta,Camera3d* cam)
	{
		if(cam)
		{
			engine->setListenerPosition(irrklang::vec3df(cam->getPos().x,cam->getPos().y,cam->getPos().z),irrklang::vec3df(cam->getDir().x,cam->getDir().y,cam->getDir().z),irrklang::vec3df(),irrklang::vec3df(cam->getUp().x,cam->getUp().y,cam->getUp().z));
		}
		std::vector<GLuint> toErase;
		for(auto &it = info.begin(); it != info.end();it++)
		{
			it->second->timetolive -= delta/1000;
			if(it->second->timetolive < 0 || it->second->toBeStopped)
			{
				it->second->sound->stop();
			}
		}
		for(unsigned int i = 0; i < toErase.size();i++) info.erase(toErase[i]);
		

	}
};
