#pragma once

#include <string>
#include "ServiceLocator.h"

class MusicPlayer
{
public:
	MusicPlayer(std::string path);
	~MusicPlayer();
	void pause();
	void unpause();
	void play(std::string path);
	void nextSong();
	void previousSong();
	void loadSongList(std::string path);
	void update();
	//getters
	GLuint getSongNumber(){return currentID + 1;}
	std::string& getCurrentTitle(){return songList[currentID].title;}
	bool isPaused() {return paused;}
	
private:
	std::vector<SongInformation> songList;
	SoundComponent* currentSong;
	int currentID;
	unsigned int timer;
	bool paused;
	bool isWorking;
	std::vector<std::string> find_files(std::string path);
	inline void split(const std::string& s, char c, std::vector<std::string>& v);
	std::wstring s2ws(const std::string& s);
};

