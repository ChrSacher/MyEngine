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


	static void begin(int FPS);
	static void startFrame();
	static unsigned int getTick();
	static void updateSystemTime()
	{
		struct tm timeinfo;
		time_t now = time(0);
		localtime_s(&timeinfo, &now);
		_year = 1900 + timeinfo.tm_year;
		_month  = timeinfo.tm_mon + 1;
		_day  = timeinfo.tm_mday + 1;
		_hour = timeinfo.tm_hour +1;
		_minutes =  1 + timeinfo.tm_min;
		_seconds =  1 + timeinfo.tm_sec;
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
	static unsigned int getCounter()
	{
		return _counter;
	}
private:
	static float _start;
	static float _end;

	//time passed in seconds
	static float _delta;

	static unsigned int _counter;
	static unsigned int _day;
	static unsigned int _month;
	static unsigned int _year;
	static unsigned int _hour;
	static unsigned int _minutes;
	static unsigned int _seconds;
	
};
