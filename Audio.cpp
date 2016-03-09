#include "Audio.h"


Audio::Audio(void)
	{

	};
	/* Start the Engine and load all necessary things
	*/
void Audio::initialize() 
{

	YSE::System().init();
	YSE::System().update();
	last = 0;
	dummy.create("");

	auto r = YSE::System().getDevices();

	return;
};
	/*
	Destroy all loaded things and all sounds which are currently loaded
	*/
	void Audio::destroy()
	{
		YSE::System().close();
		for (unsigned int i = 0; i < empty.size(); i++) delete(empty[i]);
		empty.clear();
	}
	Audio::~Audio(void)
	{



	}
	//Play 2d Sound
	// #return will return an Int with location of the current sound
	// if -1 then sound couldn't be loaded
	void Audio::play2D(YSE::sound* sound , std::string audiopath, float Volume)
	{
		if (!sound)
		{
			YSE::sound* temp = new YSE::sound;
			temp->setVolume(Volume);
			temp->play();
			empty.push_back(temp);
			update();
			return;
		}
		sound->stop();
		sound->create(audiopath.c_str(),nullptr ,false,Volume,false);
		if (sound->isValid())
		{
			sound->setVolume(Volume);
			sound->set2D(true);
			sound->play();
			update();
			return ;
		}
		return;

	}
	//Play 3d Sound
	// #return will return an Int with location of the current sound
	// if -1 then sound couldn't be loaded
	void  Audio::play3D(YSE::sound* sound,std::string audiopath, Vector3 position, float Volume)
	{
		if (!sound)
		{
			YSE::sound* temp = new YSE::sound;
			temp->setVolume(Volume);
			temp->play();
			empty.push_back(temp);
			update();
			return;
		}
		sound->stop();
		sound->create(audiopath.c_str(), nullptr, false, Volume, false);
		if (sound->isValid())
		{
			sound->setVolume(Volume);
			sound->play();
			update();
			return;
		}
		return;
	}
	void Audio::stopSounds()
	{

		update();
	}
	void Audio::pauseSounds(bool paused)
	{

		update();
	}
	void Audio::setPos(Vector3 Pos, Vector3 Dir, Vector3 Vel, Vector3 Up)
	{
		YSE::Listener().setPosition(Vec(Pos));
		YSE::Listener().setOrientation(Vec(Dir), Vec(Up));
	}
	void Audio::update()
	{
		YSE::System().update();
		for (unsigned int i = 0; i < empty.size(); i++)
		{
			if (empty[i]->isStopped()) 
			{
				delete(empty[i]);
				empty.erase(empty.begin() + i);
			}
		}
	}






	LoggedAudio::LoggedAudio(void)
	{

	};
	LoggedAudio::~LoggedAudio(void)
	{

	}
	void LoggedAudio::play2D(YSE::sound* sound, std::string audiopath, float Volume)
	{
		if (!sound)
		{
			YSE::sound* temp = new YSE::sound;
			temp->setVolume(Volume);
			temp->play();
			empty.push_back(temp);
			update();
			return;
		}
		//sound->stop();
		sound->create(audiopath.c_str(), nullptr, false, Volume, false);
		if (sound->isValid())
		{
			sound->setVolume(Volume);
			sound->set2D(true);
			sound->play();
			update();
			return;
		}
		return;


	}
	void LoggedAudio::play3D(YSE::sound* sound, std::string audiopath, Vector3 position, float Volume)
	{
		if (!sound)
		{
			YSE::sound* temp = new YSE::sound;
			temp->setVolume(Volume);
			temp->play();
			empty.push_back(temp);
			update();
			return;
		}
		//sound->stop();
		sound->create(audiopath.c_str(), nullptr, false, Volume, false);
		if (sound->isValid())
		{
			sound->setVolume(Volume);
			sound->play();
			update();
			return;
		}
		return;
	}
	void LoggedAudio::stopSounds()
	{

		std::cout << "Stopping all Sounds" << std::endl;
		update();
	}
	void LoggedAudio::pauseSounds(bool paused)
	{
		std::cout << "Pausing all Sounds" << std::endl;
		update();
	}
