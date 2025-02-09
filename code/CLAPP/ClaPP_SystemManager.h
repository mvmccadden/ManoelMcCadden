// Author:	Manoel McCadden
// File:	ClaPP_SystemManager.h
// 
// ECS REFERENCE: https://austinmorlan.com/posts/entity_component_system/#what-is-an-ecs

#pragma once
#include <vector>

#include "ClaPP_Global.h"

namespace ClaPP
{
	typedef class System System;

	class SystemManager
	{
	public:
		static SystemManager& GetInstance()
		{
			static SystemManager instance;
			return instance;
		}
		// Run all active systems
		void RunSystems();
		// Exit all active systems
		void ReverseRunSystems();
		// Push back a pointer to a system
		//  As this is a raw pointer it should not
		//	be allocated in the heap as we are not using
		//	ClaPP_Memory to handle operations
		// WARNING: NOT FULLY IMPLEMENTED!!!!
		void AddSystem(System* system);
		// Copies the value since it has to add it to the entity vector
		//	and it can't add a const value
		void AddEntityToSystems(const Entity& entity);
		// Remove an entity from all systems
		void RemoveEntityFromSystems(const Entity& entity);
	private:
		SystemManager();
		~SystemManager() {}

		SystemManager(const SystemManager&) = delete;
		SystemManager& operator=(const SystemManager&) = delete;
		// Systems are simply instances which are static therefore they can
		//	be raw pointers!
		std::vector<System*> systems;
	};
}