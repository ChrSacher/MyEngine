#include "ServiceLocator.h"

Audio* ServiceLocator::service_ = NULL;
NullAudio  ServiceLocator::nullService_;
Text* ServiceLocator::text_ = NULL;
NullText ServiceLocator::nullText_;
LuaEngine* ServiceLocator::lua = NULL;
NullLuaEngine ServiceLocator::nullLua_;
ComponentManager* ServiceLocator::manager = NULL;
NullComponentManager ServiceLocator::nullCM;
EntityManager* ServiceLocator::entity_ = NULL;
NullEntityManager ServiceLocator::nullEntity;