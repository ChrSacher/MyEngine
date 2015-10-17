#include "Time.h"


float Time::_delta = 0.017;
float Time::_start = 0.017;
unsigned int Time::_counter = 0.017;
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
