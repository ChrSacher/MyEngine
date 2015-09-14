#include "ServiceLocator.h"

Audio* ServiceLocator::service_;
NullAudio  ServiceLocator::nullService_;
Text* ServiceLocator::text_;
NullText ServiceLocator::nullText_;
LuaEngine* ServiceLocator::lua;
NullLuaEngine ServiceLocator::nulllua;