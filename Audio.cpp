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
	dummy.create("res/sound/Music/MUS_Aint_That_A_Kick_In_the_Head.mp3");
	dummy.play();
	dummy.pause();
	auto r = YSE::System().getDevices();

	return;
};
	/*
	Destroy all loaded things and all sounds which are currently loaded
	*/
	void Audio::destroy()
	{
		YSE::System().close();
		for (auto &it = info.begin(); it != info.end(); it++)
		{
			delete(it->second);
		}
		info.clear();
	}
	Audio::~Audio(void)
	{



	}
	//Play 2d Sound
	// #return will return an Int with location of the current sound
	// if -1 then sound couldn't be loaded
	YSE::sound* Audio::play2D(std::string audiopath, bool store, float Volume)
	{
		YSE::sound *temp = new YSE::sound();
		temp->create(audiopath.c_str(),nullptr ,false,Volume,true);
		if (temp->isValid())
		{
			temp->setVolume(Volume);
			temp->set2D(true);
			temp->play();
			if (store) info.insert(std::make_pair(++last, temp));
			else empty.insert(std::make_pair(++last, temp));
			update();
			return temp;
		}
		return &dummy;

	}
	//Play 3d Sound
	// #return will return an Int with location of the current sound
	// if -1 then sound couldn't be loaded
	YSE::sound* Audio::play3D(std::string audiopath, Vector3 position, float Volume, bool store)
	{
		YSE::sound *temp = new YSE::sound();
		temp->create(audiopath.c_str());
		if (temp->isValid())
		{

			temp->setVolume(Volume);
			temp->setPosition(Vec(position));
			temp->play();
			if (store) info.insert(std::make_pair(++last, temp));
			else empty.insert(std::make_pair(++last, temp));
			update();
			return temp;
		}
		return &dummy;
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
		for (auto &it = empty.begin(); it != empty.end(); it++)
		{
			if (it->second->isStopped()) delete(it->second);
		}
	}






	LoggedAudio::LoggedAudio(void)
	{

	};
	LoggedAudio::~LoggedAudio(void)
	{

	}
	YSE::sound* LoggedAudio::play2D(std::string audiopath, bool store, float Volume)
	{
		YSE::sound *temp = new YSE::sound();
		temp->create(audiopath.c_str());
		if (temp->isValid())
		{

			temp->setVolume(Volume);
			temp->set2D(true);
			temp->play();
			std::cout << "Loading Sound " + audiopath << std::endl;

			if (store) info.insert(std::make_pair(++last, temp));
			else empty.insert(std::make_pair(++last, temp));
			update();
			return temp;
		}
		std::cout << "Failed Loading Sound " + audiopath << std::endl;
		return &dummy;


	}
	YSE::sound* LoggedAudio::play3D(std::string audiopath, Vector3 position, float Volume, bool store)
	{
		YSE::sound *temp = new YSE::sound();
		temp->create(audiopath.c_str());
		if (temp->isValid())
		{
			temp->setPosition(Vec(position));
			temp->setVolume(Volume);
			temp->play();
			std::cout << "Loading Sound " + audiopath << std::endl;
			if (store) info.insert(std::make_pair(++last, temp));
			else empty.insert(std::make_pair(++last, temp));
			update();
			return temp;
		}
		std::cout << "Failed Loading Sound " + audiopath << std::endl;
		return &dummy;
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
