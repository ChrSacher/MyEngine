#include "MessageEvent.h"

MessageEvent MessageEvent::currentEvent;
MessageEventType stringToMET(std::string &convert)
{
	std::map<std::string, MessageEventType> converter;
	converter.insert(std::make_pair("Collision", Collision));

	auto r = converter.find(convert);
	if (r == converter.end()) return Invalid;
	return r->second;
}
MessageEvent::MessageEvent()
{
}


MessageEvent::~MessageEvent()
{
}
