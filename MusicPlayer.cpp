#include "MusicPlayer.h"
#include <libconfig\libconfig.hh>
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
template <typename ValueType>
bool look(libconfig::Setting& setting, std::string place, ValueType &value)
{
	if (setting.exists(place))
	{
		return setting.lookupValue(place, value);

	}
	else std::cout << setting.getPath() + place << " does not exist. Value remains unchanged." << std::endl;
	return false;
}

/*bool look(libconfig::Setting& current, std::string place, Vector3& vec, std::string ent)
{
	using namespace libconfig;
	if (current.exists(place))
	{
		Setting& posit = current.lookup(place);
		vec[0] = posit[0];
		vec[1] = posit[1];
		vec[2] = posit[2];
		return true;
	}
	else
	{
		std::cout << place << " error in " << ent << std::endl;
		return false;
	}
}*/

void MusicPlayer::loadSongList(std::string path)
{
	using namespace libconfig;
	Config cfg;
	cfg.readFile(path.c_str());
	Setting& root = cfg.getRoot();
	if (!root.exists("Songs")) return;
	for (unsigned int i = 0; i < root["Songs"].getLength(); i++)
	{
		Setting& place = root["Songs"][i];
		std::string path = "", title = "";
		look(place, "Path", path);
		look(place, "Title", title);
		songList.push_back(SongInformation(path,title));
	}
}

void MusicPlayer::saveSongList(std::string path)
{
	using namespace libconfig;
	Config cfg;
	Setting& root = cfg.getRoot();
	root.add("Version", Setting::TypeFloat) = 1.0f;
	
	Setting&  list = root.add("Songs",Setting::TypeList);
	for (unsigned int i = 0; i < songList.size(); i++)
	{
		Setting& r = list.add(Setting::TypeGroup);
		r.add("Path",Setting::TypeString) = songList[i].path;
		r.add("Title",Setting::TypeString) = songList[i].title;
	}
	cfg.writeFile(path.c_str());
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