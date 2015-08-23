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

//#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll

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
	class Listener
	{
		virtual ~Listener() { }

        /**
         * Handles when an camera settings change.
         *
         * @param camera The camera that was changed.
         */
        virtual void audioChanged(Audio* audio) = 0;
	};
	void initialize()
	{
		 engine =  irrklang::createIrrKlangDevice();
		 if (!engine) fatalError("Failed to create Audio Device");
		 engine->update();
		 last = 0;
		 _listeners = NULL;
	};
	void destroy()
	{
		engine->drop();
		for(auto &it = info.begin();it != info.end();it++)
		 {
			 delete(it->second);
		 }
		 if(_listeners) delete(_listeners);
		
		 info.clear();
	}
	~Audio(void)
	{
		
		
		
	}
	
	GLuint play2D(std::string audiopath,bool startpaused = false,float Volume = 1.0f)
	{
		irrklang::ISound* temp = engine->play2D(audiopath.c_str(),false,startpaused,true);
		if(temp) 
		{
				temp->setVolume(Volume);
				temp->grab();
				info.insert(std::make_pair(++last,temp));
				engine->update();
				return last;
		}
		return -1;
		
	}
	GLuint play3D(std::string audiopath,Vector3 position,float Volume = 1.0f,bool startpaused = false)
	{
		irrklang::ISound* temp = engine->play3D(audiopath.c_str(),vec3df(position),false,startpaused,true);
		if(temp) 
		{
				temp->grab();
				temp->setVolume(Volume);
				info.insert(std::make_pair(++last,temp));
				engine->update();
				return last;
		}
		return -1;
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
	void update(Vector3 Pos = Vector3(),Vector3 Dir = Vector3(0,0,-1),Vector3 Vel = Vector3(), Vector3 Up= Vector3(0,0,1))
	{
			engine->setListenerPosition(vec3df(Pos),vec3df(Dir),vec3df(Vel),vec3df(Up));
	}
	irrklang::ISound* getSound(GLuint ID)
	{
		auto it = info.find(ID);
		if(it != info.end())
		{
			return it->second;
		}
		else
			return NULL;
	}
private:
	irrklang::ISoundEngine* engine;
	std::map<GLuint,irrklang::ISound*> info;
	GLuint last;
	Vector3 pos;
protected:
	std::list<Camera3d::Listener*>* _listeners;
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
  irrklang::ISound* play2D(std::string audiopath,bool startpaused = false,float Volume = 1.0f,float timetolive = 999999)
	{
	}
	irrklang::ISound* play3D(std::string audiopath,Vector3 position,float Volume = 1.0f,bool startpaused = false,float timetolive = 99999999)
	{
	}
	void stopSounds()
	{
	}
	void pauseSounds(bool paused)
	{
	}
	void update(Camera3d* cam)
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
	void initialize()
	{
		 engine =  irrklang::createIrrKlangDevice();
		 if (!engine) fatalError("Failed to create Audio Device");
		 std::cout<< "Initiliazing Audio"<<std::endl;
		 engine->update();
		 last = 0;
		 _listeners = NULL;
	};
	void destroy()
	{
		 std::cout<< "Destroying Audio"<<std::endl;
		for(auto &it = info.begin();it != info.end();it++)
		 {
			 delete(it->second);
		 }
		 info.clear();
		 if(_listeners) delete(_listeners);
		 engine->drop();
		 
	}
	LoggedAudio(void)
	{
		
	};
	~LoggedAudio(void)
	{
		
	}
	irrklang::ISoundEngine* engine;
	std::map<GLuint,irrklang::ISound*> info;
	GLuint last;
	Vector3 pos;
	GLuint play2D(std::string audiopath,bool startpaused = false,float Volume = 1.0f)
	{
		irrklang::ISound* temp = engine->play2D(audiopath.c_str(),false,startpaused,true);
		if(temp) 
		{
				temp->setVolume(Volume);
				temp->grab();
				std::cout << "Loading Sound " + audiopath<<std::endl;
				info.insert(std::make_pair(++last,temp));
				engine->update();
				return last;
		}
		return -1;
		
	}
	GLuint play3D(std::string audiopath,Vector3 position,float Volume = 1.0f,bool startpaused = false)
	{
		irrklang::ISound* temp = engine->play3D(audiopath.c_str(),vec3df(position),false,startpaused,true);
		if(temp) 
		{
				temp->grab();
				temp->setVolume(Volume);
				std::cout << "Loading Sound " + audiopath<<std::endl;
				info.insert(std::make_pair(++last,temp));
				engine->update();
				return last;
		}
		return -1;
	}
	void stopSounds()
	{
		engine->stopAllSounds();
		std::cout<<"Stopping all Sounds"<<std::endl;
		engine->update();
	}
	void pauseSounds(bool paused)
	{
		engine->setAllSoundsPaused(paused);
		std::cout<<"Pausing all Sounds"<<std::endl;
		engine->update();
	}
};
