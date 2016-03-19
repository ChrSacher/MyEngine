#pragma once
#include <string>
#include <map>
enum MessageEventType
{
	Collision,
	Invalid,
	numEvents
};

MessageEventType stringToMET(std::string &convert);
struct MessageEvent
{
public:
	MessageEvent();
	~MessageEvent();
	MessageEventType type;
	static MessageEvent currentEvent;
};


