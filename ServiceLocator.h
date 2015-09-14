#pragma once
#include "Audio.h"
#include "Text.h"
#include "LuaEngine.h"
//This class will hold all kinds of services for audio logging and so on
//This allows for fast new binding of different things

static class ServiceLocator
{
public:
	static void initialize()
	{
		service_ = &nullService_;
		text_ = &nullText_;
		lua = &nulllua;
	}

	static Audio& getAudio() { return *service_; }
	static Text& getText() { return *text_; }
  static LuaEngine& getLua() { return *lua; }
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
  static void provide(LuaEngine* engine)
  {
	  if (engine == NULL)
	  {
		  // Revert to null service.
		  lua = &nulllua;
	  }
	  else
	  {
		  lua = engine;
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
  static LuaEngine* lua;
  static NullLuaEngine nulllua;
};

