#pragma once

#include <string>
#include "ServiceLocator.h"

struct SongInformation
{
	SongInformation(std::string &Path, std::string &Title)
	{
		title =Title;
		path = Path;
	}
	std::string title;
	std::string path;
};
//litteraly the thing it says
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
	void saveSongList(std::string path);
	void update();
	//getters
	//returns the song number geinning from 1
	GLuint getSongNumber(){return currentID + 1;}
	std::string& getCurrentTitle(){return songList[currentID].title;}
	bool isPaused() {return paused;}
	
private:
	std::vector<SongInformation> songList;
	YSE::sound* currentSong;
	int currentID;
	unsigned int timer;
	bool paused;
	bool isWorking;
	std::vector<std::string> find_files(std::string path);
	inline void split(const std::string& s, char c, std::vector<std::string>& v);
};

