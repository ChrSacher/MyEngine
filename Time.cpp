#include "Time.h"


float Time::_delta = 0.017;
float Time::_start = 0.017;
unsigned int Time::_counter = 0.017;
float Time::_end = 0.017;
float Time::_frameTime = 0.017;
float Time::_updateTime = 0.017;

const float &Time::delta = Time::_delta;
const float &Time::start = Time::_start;
const unsigned int &Time::counter = Time::_counter;
const float &Time::end = Time::_end;
const float &Time::frameTime = Time::_frameTime;
const float &Time::updateTime = Time::_updateTime;
