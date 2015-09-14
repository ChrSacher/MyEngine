#pragma once
#include <SDL.h>
struct Time
{
public:
	static const float &start;
	static const float &end;
	static const float &delta;
	static const float &frameTime;
	static const float &updateTime;
	static const unsigned int &counter;
	
	static void begin(int FPS)
	{
		Time::_start= 0.0f;
		Time::_end = SDL_GetTicks();
		Time::_counter = 0;
		Time::_delta = 0;
		Time::_frameTime = 0;
		Time::_updateTime = (float)(1.0f / (float)FPS) * 1000.0f;
	}
	static void startFrame()
	{
		Time::_start = SDL_GetTicks();
		Time::_frameTime = (Time::_start - Time::_end);
		Time::_delta += Time::_frameTime;
	}
	static bool frameStep()
	{
		if (Time::_delta >= Time::_updateTime)
		{
			Time::_delta = Time::_delta - Time::_updateTime;
			return true;
		}
		return false;
	}
	static void endFrame()
	{
		Time::_end = Time::_start;
		Time::_counter++;
	}
private:
	static float _start;
	static float _end;
	static float _delta;
	static float _frameTime;
	static float _updateTime;
	static unsigned int _counter;
	
};

