#pragma once
#include "Audio.h"
#include "Text.h"

//This class will hold all kinds of services for audio logging and so on
//This allows for fast new binding of different things

class ServiceLocator
{
public:
	ServiceLocator(){}
	~ServiceLocator(){}
  static void initialize() { service_ = &nullService_;
							 text_ = &nullText_;}

  static Audio& getAudio() { return *service_; }
  static Text& getText() { return *text_; }

  static void provide(Audio* service)
  {
    if (service == NULL)
    {
      // Revert to null service.
      service_ = &nullService_;
    }
    else
    {
      service_ = service;
    }
  }
  static void provide(Text* service)
  {
    if (service == NULL)
    {
      // Revert to null service.
      text_ = &nullText_;
    }
    else
    {
      text_ = service;
    }
  }

private:
  static Audio* service_;
  static NullAudio nullService_;
  static Text* text_;
  static NullText nullText_;
};

