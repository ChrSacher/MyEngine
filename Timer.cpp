#include "Timer.h"


float Time::_delta = 0.017;
float Time::_start = 0.017;
unsigned int Time::_counter = 0;
float Time::_end = 0.017;
unsigned int _day = 0;
unsigned int _month = 0;
unsigned int _year = 0;
unsigned int _hour = 0;
unsigned int _minutes = 0;
unsigned int _seconds = 0;

const float &Time::delta = Time::_delta;
const float &Time::start = Time::_start;
const unsigned int &Time::counter = Time::_counter;
const float &Time::end = Time::_end;
void Time::begin(int FPS)
{
	Time::_start = 0;
	Time::_end = SDL_GetTicks();
	Time::_counter = 0;
	Time::_delta = 0;
}
void Time::startFrame()
{
	Time::_start = SDL_GetTicks();
	Time::_delta = (Time::_start - Time::_end) / 1000;
}

unsigned int  Time::getTick()
{
	return  SDL_GetTicks();
}