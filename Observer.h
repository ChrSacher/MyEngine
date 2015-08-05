#pragma once
#include <vector>
enum SE_Event
{
	Test1,
	test2,
	numEvent
};
struct Observer
{
public:
	virtual ~Observer(void);
	virtual void update(SE_Event &event);
};

class Subject
{
public:
	virtual ~Subject(void);
	
	SE_Event currentEvent;
	std::vector<Observer*> observers;
	void addObserver(Observer* newObserver);
	void removeObserver(Observer* removeObserver);
	void notify();	//tell all observer that stuff happend
	void setEvent(); // calls notify and changes state
};



