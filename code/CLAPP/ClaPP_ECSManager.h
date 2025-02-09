// Author:	Manoel McCadden
// File:	ClaPP_ECSManager.h
// 
// ECS REFERENCE: https://austinmorlan.com/posts/entity_component_system/#what-is-an-ecs

#pragma once

#include "ClaPP_Global.h"
#include "ClaPP_Component.h"
#include "set"
#include "array"

namespace ClaPP
{
	typedef class EntityManager EntityManager;
	typedef class ComponentManager ComponentManager;
	typedef class SystemManager SystemManager;

	class ECSManager
	{
	public:
		static ECSManager& GetInstance()
		{
			static ECSManager instance;
			return instance;
		}
		// Returns the new value of the created entity
		Entity CreateEntity();
		// Adds a heap allocated component and handles the memory
		//	, system, component, and entity functionality
		void AddComponent(const Entity& entity, Component* data);
		void RemoveComponent(const Entity& entity
			, const COMPONENTTYPE& type);
		// Gets the components based on the type
		Component* GetComponent(const Entity& entity
			, const COMPONENTTYPE& type);
		// Gets the signature of an entity
		ComponentSignature& GetSignature(const Entity& entity);
		// Deletes an entity and all instances of it in the
		//	entity, component, and system mangers
		void DeleteEntity(const Entity& entity);

	private:
		ECSManager();
		~ECSManager();

		ECSManager(const ECSManager&) = delete;
		ECSManager& operator=(const ECSManager&) = delete;

		// As these are pointers to the singleton instance
		//	they can be raw
		EntityManager* entityManager;
		SystemManager* systemManager;
		ComponentManager* componentManager;
	};

}