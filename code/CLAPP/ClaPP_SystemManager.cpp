// Author:	Manoel McCadden
// File:	ClaPP_SystemManager.cpp
// 
// ECS REFERENCE: https://austinmorlan.com/posts/entity_component_system/#what-is-an-ecs

#include "ClaPP_SystemManager.h"

#include "ClaPP_ECSManager.h"

#include "ClaPP_GraphicsSystem.h"
#include "ClaPP_EditorSystem.h"
#include "ClaPP_System_Physics.h"
#include "ClaPP_System_Collision.h"
#include "ClaPP_System_Audio.h"

#include "ClaPP_System.h"

using namespace ClaPP;

SystemManager::SystemManager()
{
	// Add all needed systems to the manager
	systems.push_back(&GraphicsSystem::GetInstance());
	DebugLogger::GetInstance().NewTraceMessage("Graphics System Added To System Manager", DebugLogger::INFORMATION);
	systems.push_back(&EditorSystem::GetInstance());
	DebugLogger::GetInstance().NewTraceMessage("Editor System Added To System Manager", DebugLogger::INFORMATION);
	systems.push_back(&PhysicsSystem::GetInstance());
	DebugLogger::GetInstance().NewTraceMessage("Physics System Added To System Manager", DebugLogger::INFORMATION);
	systems.push_back(&CollisionSystem::GetInstance());
	DebugLogger::GetInstance().NewTraceMessage("Collision System Added To System Manager", DebugLogger::INFORMATION);
	systems.push_back(&AudioSystem::GetInstance());
	DebugLogger::GetInstance().NewTraceMessage("Audio System Added To System Manager", DebugLogger::INFORMATION);
}

void SystemManager::RunSystems() 
{
	CLAPP_STATUS result;
	for (auto& system : systems)
	{
		result = system->Run();

		if (result == CLAPP_ERROR)
		{
			assert(false);
		}
	}
}

void SystemManager::ReverseRunSystems()
{
	CLAPP_STATUS result;
	for (auto system = systems.rbegin()
		; system != systems.rend(); ++system)
	{
		result = (*system)->Run();

		if (result == CLAPP_ERROR)
		{
			assert(false);
		}
	}
}

void SystemManager::AddSystem(System* system)
{
	systems.push_back(system);
}

// Adds only unique entites to the systems cause 
//  they are using  unordered_set container
void SystemManager::AddEntityToSystems(const Entity& entity)
{
	ComponentSignature entitySig = ECSManager::GetInstance().GetSignature(entity);

	for (auto& system : systems)
	{
		ComponentSignature systemSig = system->GetSystemSignature();

		if ((systemSig & entitySig) == systemSig)
		{
			system->AddEntityToSystem(entity);
		}
	}
}

void SystemManager::RemoveEntityFromSystems(const Entity& entity)
{
	for (auto& system : systems)
	{
		system->RemoveEntityFromSystem(entity);
	}
}