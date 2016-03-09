#include "ServiceLocator.h"

Audio* ServiceLocator::service_ = NULL;
NullAudio  ServiceLocator::nullService_;
Text* ServiceLocator::text_ = NULL;
NullText ServiceLocator::nullText_;
ComponentManager* ServiceLocator::manager = NULL;
NullComponentManager ServiceLocator::nullCM;
EntityManager* ServiceLocator::entity_ = NULL;
NullEntityManager ServiceLocator::nullEntity;
PhysicsEngine* ServiceLocator::physics_ = NULL;
NullPhysicsEngine ServiceLocator::nullPhysics;