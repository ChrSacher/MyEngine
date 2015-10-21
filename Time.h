#pragma once
#include <SDL.h>
#include <dos.h>
#include <ctime>
struct Time
{
public:
	static const float &start;
	static const float &end;
	static const float &delta;
	static const unsigned int &counter;


	static void begin(int FPS)
	{
		Time::_start= 0.0f;
		Time::_end = SDL_GetTicks();
		Time::_counter = 0;
		Time::_delta = 0;
	}
	static void startFrame()
	{
		Time::_start = SDL_GetTicks();
		Time::_delta = (Time::_start - Time::_end) / 1000;
	}

	static void updateSystemTime()
	{
		time_t now = time(0);
		tm *ltm = localtime(&now);
		_year = 1900 + ltm->tm_year;
		_month  = ltm->tm_mon + 1;
		_day  = ltm->tm_mday + 1;
		_hour = ltm->tm_hour +1;
		_minutes =  1 + ltm->tm_min;
		_seconds =  1 + ltm->tm_sec;
	}
	static unsigned int getDay()
	{
		updateSystemTime();
		return _day;
	}
	static unsigned int getMonth()
	{
		updateSystemTime();
		return _month;
	}
	static unsigned int getYear()
	{
		updateSystemTime();
		return _year;

	}
	static unsigned int getMinute()
	{
		updateSystemTime();
		return _minutes;

	}
	static unsigned int getSecond()
	{
		updateSystemTime();
		return _seconds;

	}
	static unsigned int getHour()
	{
		updateSystemTime();
		return _hour;
	}
	static void endFrame()
	{
		Time::_end = Time::_start;
		Time::_counter++;
	}
	static float getDelta()
	{
		return _delta;
	}
	static float getCounter()
	{
		return _counter;
	}
private:
	static float _start;
	static float _end;
	static float _delta;
	static unsigned int _counter;
	static unsigned int _day;
	static unsigned int _month;
	static unsigned int _year;
	static unsigned int _hour;
	static unsigned int _minutes;
	static unsigned int _seconds;
	
};
