#include "MusicPlayer.h"
//removes leading and trailing whitespace
void trim(std::string& s)
{
    size_t p = s.find_first_not_of(" \t");
    s.erase(0, p);

   p = s.find_last_not_of(" \t");
   if (std::string::npos != p)
       s.erase(p + 1);
}

MusicPlayer::MusicPlayer(std::string path)
{
	paused = false;
	currentID = 0;
	timer = 0;
	currentSong = new YSE::sound();
	loadSongList(path);
	play(songList[currentID].path);
}
void MusicPlayer::update()
{
	if (currentSong == NULL) return;
	if(currentSong->isReady() && (timer + 500 > Time::getTick()) ) if( currentSong->isStopped()) nextSong();
}
MusicPlayer::~MusicPlayer()
{
	if (currentSong == NULL) return; 
	currentSong->stop();
	delete currentSong;
}
void MusicPlayer::pause()
{
	if (currentSong == NULL) return;
	if(!paused)
	{
		currentSong->pause();
		paused = true;
	}
	else
	{
		currentSong->play();
		paused = false;
	}

}
void MusicPlayer::play(std::string path)
{
	if(paused)
	{
		ServiceLocator::getAudio().play2D(currentSong,path,1.0f);
		currentSong->play();
	}
	else
	{
		ServiceLocator::getAudio().play2D(currentSong,path,  1.0f);
		currentSong->play();
		timer = Time::getTick();
	}
	
}
void MusicPlayer::nextSong()
{
	currentID++;
	currentSong->stop();
	if(currentID >= songList.size()) currentID = 0;
	play(songList[currentID].path);
}
void MusicPlayer::previousSong()
{
	if (currentSong != NULL) currentSong->stop();
	
	if(timer + 5000 > Time::getTick())
	{
		currentSong->restart();
	}
	else
	{
		currentID--;
		if(currentID < 0) 
		{
			currentID = songList.size() - 1;
		}
		play(songList[currentID].path);
	}
	
}
void MusicPlayer::loadSongList(std::string path)
{
	songList.clear();
	std::string line;
	std::ifstream encodefile(path.c_str());
	while (std::getline(encodefile, line))
	{
		auto pos = line.rfind(";");
		if (pos != std::string::npos)
		{
			line.erase(pos);

		}
		pos = line.rfind("//");
		if (pos != std::string::npos)
		{
			line.erase(pos);
		}
		std::vector<std::string> lines;
		split(line, ',', lines);
		if (lines.size() < 3) continue;
		
		trim(lines[0]);
		trim(lines[1]);
		trim(lines[2]); //lazy
		songList.push_back(SongInformation(lines[1], lines[2], std::stoi(lines[0])));
	}
	return;
	
}

inline void MusicPlayer::split(const std::string& s, char c, std::vector<std::string>& v)
{
   std::string::size_type i = 0;
   std::string::size_type j = s.find(c);

   while (j != std::string::npos)
   {
      v.push_back(s.substr(i, j-i));
      i = ++j;
      j = s.find(c, j);
	  if (j == std::string::npos && s.substr(i, s.length()).length() != 0)
         v.push_back(s.substr(i, s.length()));
   }
}