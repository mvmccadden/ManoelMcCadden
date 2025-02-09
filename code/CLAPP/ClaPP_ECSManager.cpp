// Author:	Manoel McCadden
// File:	ClaPP_ECSManager.cpp
// 
// ECS REFERENCE: https://austinmorlan.com/posts/entity_component_system/#what-is-an-ecs

#include "ClaPP_ECSManager.h"

#include "ClaPP_EntityManager.h"
#include "ClaPP_ComponentManager.h"
#include "ClaPP_SystemManager.h"

using namespace ClaPP;

ECSManager::ECSManager()
	: entityManager(&EntityManager::GetInstance())
	, systemManager(&SystemManager::GetInstance())
	, componentManager(&ComponentManager::GetInstance()) 
{
}

ECSManager::~ECSManager()
{
}

Entity ECSManager::CreateEntity()
{
	return entityManager->CreateEntity();
}

void ECSManager::AddComponent(const Entity& entity, Component* data)
{
	componentManager->AddComponent(entity, data);
	ComponentSignature& signature = entityManager->GetSignature(entity);
	signature.set(data->GetComponentType(), true);
	entityManager->SetComponentSignature(entity, signature);
	systemManager->AddEntityToSystems(entity);
}

Component* ECSManager::GetComponent(const Entity& entity
	, const COMPONENTTYPE& type)
{
	return componentManager->GetComponent(entity, type);
}

void ECSManager::RemoveComponent(const Entity& entity, const COMPONENTTYPE& type)
{
	componentManager->DeleteComponent(entity, type);
	ComponentSignature& signature = entityManager->GetSignature(entity);
	signature.set(type, false);
	// When removing a component find a way to remove it from all systems
	// with a system check!
	// Temporarly removing entity and readding it but is slow...
	systemManager->RemoveEntityFromSystems(entity);
	systemManager->AddEntityToSystems(entity);
}

ComponentSignature& ECSManager::GetSignature(const Entity& entity)
{
	return entityManager->GetSignature(entity);
}

void ECSManager::DeleteEntity(const Entity& entity)
{
	entityManager->DeleteEntity(entity);
	componentManager->DeleteAllEntityComponents(entity);
	systemManager->RemoveEntityFromSystems(entity);
}