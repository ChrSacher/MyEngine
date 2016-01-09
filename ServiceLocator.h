#pragma once
#include "Audio.h"
#include "Text.h"
#include "LuaEngine.h"
#include "ComponentManager.h"
#include "EntityManager.h"
//This class will hold all kinds of services for audio logging and so on
//This allows for fast new binding of different things

class LuaEngine;
class ServiceLocator
{
public:
	static Audio& getAudio() { return *service_; }
	static Text& getText() { return *text_; }
	static LuaEngine& getLua() { return *lua; }
	static ComponentManager& getCM() { return *manager; }
	static EntityManager& getEM() { return *entity_; }
	static PhysicsEngine& getPE() { return *physics_; }
	static void initialize()
	{
		service_ = &nullService_;
		text_ = &nullText_;
		lua = &nullLua_;
		manager = &nullCM;
	}

	
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
		  lua = &nullLua_;
	  }
	  else
	  {
		  lua = engine;
	  }
  }
  static void provide(ComponentManager* service)
  {
	  if (service == NULL)
	  {
		  // Revert to null service.
		 manager = &nullCM;
	  }
	  else
	  {
		 manager = service;
	  }
  }

  static void provide(PhysicsEngine* service)
  {
	  if (service == NULL)
	  {
		  // Revert to null service.
		  physics_ = &nullPhysics;
	  }
	  else
	  {
		  physics_ = service;
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

  static void provide(EntityManager* service)
  {
	  if (service == NULL)
	  {
		  // Revert to null service.
		  entity_ = &nullEntity;
	  }
	  else
	  {
		  entity_ = service;
	  }
  }

private:
  static Audio* service_;
  static NullAudio nullService_;
  static Text* text_;
  static EntityManager* entity_;

  static NullText nullText_;
  static LuaEngine* lua;
  static NullLuaEngine nullLua_;
  static ComponentManager* manager;
  static NullComponentManager nullCM;
  static NullEntityManager nullEntity;
  static PhysicsEngine* physics_;
  static NullPhysicsEngine nullPhysics;
};

