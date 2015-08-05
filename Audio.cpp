#include "Audio.h"


MusicPlayer::MusicPlayer(std::string path)
{
	paused = true;
	currentID = 0;
	timer = 0;
	loadSongList(path);
	play(songList[currentID].path);
}
void MusicPlayer::update()
{
	if(currentSong->sound->isFinished()) nextSong();
}
MusicPlayer::~MusicPlayer()
{
	currentSong->stop();
}
void MusicPlayer::pause()
{
	currentSong->sound->setIsPaused(true);
	paused = true;
}
void MusicPlayer::unpause()
{
	currentSong->sound->setIsPaused(false);
	paused = false;
	timer = SDL_GetTicks();
}
void MusicPlayer::play(std::string path)
{
	if(paused)
	{
		currentSong = audio::get().play2D(path,true,1.0f);
	}
	else
	{
		currentSong = audio::get().play2D(path,false,1.0f);
		timer = SDL_GetTicks();
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
	
	currentSong->stop();
	if(timer + 5 > SDL_GetTicks())
	{
		audio::get().play2D(songList[currentID].path);
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
	std::vector<std::string> temp = find_files(path);
	for(unsigned int i = 0; i < temp.size() ; i++)
	{
		std::string title = temp[i].substr(0,temp[i].find("."));
		for(unsigned int j = 0; j < 10; j++)
		{
			auto forward = title.find("/");
			auto backward = title.find("\\");
			if(backward == title.npos && forward == title.npos) break;
			if(forward != title.npos) title = title.substr(forward + 1,title.npos);
			if(backward != title.npos) title = title.substr(backward + 1,title.npos);
			
		}
		songList.push_back(SongInformation(title,temp[i]));
	}
	
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
std::wstring MusicPlayer::s2ws(const std::string& s)
{
    int len;
    int slength = (int)s.length() + 1;
    len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0); 
    wchar_t* buf = new wchar_t[len];
    MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
    std::wstring r(buf);
    delete[] buf;
    return r;
}
std::vector<std::string> MusicPlayer::find_files(std::string path)
	{

	  WIN32_FIND_DATA FindFileData;
	  std::vector<std::string> temp;
	  std::wstring stemp = std::wstring(path.begin(), path.end());
	  LPCWSTR sw = stemp.c_str();
	  HANDLE hFind = FindFirstFile(sw, &FindFileData);
	  path =  path.substr(0,path.find("*"));
	  if(hFind == INVALID_HANDLE_VALUE){
		return temp;
	  } 
	  else while (FindNextFile(hFind, &FindFileData))
	  {
	 
			  std::wstring ws(FindFileData.cFileName);
			  std::string temp2(ws.begin(),ws.end());
			  if(temp2 != ".." && temp2 != "." && temp2.find(".mp3") != temp2.npos) temp.push_back(path + temp2);;
	
	  };
	  FindClose(hFind);
	  return temp;
}