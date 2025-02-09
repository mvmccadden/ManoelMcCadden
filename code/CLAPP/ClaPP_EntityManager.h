// Author:	Manoel McCadden
// File:	ClaPP_EntityManager.h
// 
// ECS REFERENCE: https://austinmorlan.com/posts/entity_component_system/#what-is-an-ecs

#pragma once

#include <array>
#include <queue>
#include <bitset>

#include "ClaPP_Global.h"

namespace ClaPP
{

	class EntityManager
	{
	public:
		// a singleton instance of the entitymanager!
		static EntityManager& GetInstance()
		{
			static EntityManager instance;
			return instance;
		}


		// Creates a new entity if there are still unused entity ID's avaliable
		Entity CreateEntity()
		{
			if (entityCount >= MAX_ENTITIES)
			{
				DebugLogger::GetInstance().NewTraceMessage(
					"ENTITY CAPCITY REACHED: UNABLE TO SPAWN MORE ENTITIES"
					, DebugLogger::ERROR);
				assert(false);
			}

			Entity newId = unusedEntities.front();
			unusedEntities.pop();
			entityCount++;

			return newId;
		}
		// Delets an entity and removes its component signature
		void DeleteEntity(const Entity& entity)
		{
			if (entity >= MAX_ENTITIES)
			{
				DebugLogger::GetInstance().NewTraceMessage(
					"ATTEMPTED TO DELETE ENTITY OUT OF RANGE"
					, DebugLogger::ERROR);
				return;
			}

			unusedEntities.push(entity);
			// sets all the bits to false so that it is
			//	setup for the next time this entity ID is used.
			entityArray[entity].reset();
			entityCount--;
		}
		// Sets the component signature/tells the entity
		//	system what type of components are enabled/avaliable
		void SetComponentSignature(const Entity& entity, const ComponentSignature& signature)
		{
			if (entity >= MAX_ENTITIES)
			{
				DebugLogger::GetInstance().NewTraceMessage(
					"ATTEMPTED TO SET COMPONENT SIGNATURE OF ENTITY OUT OF RANGE"
					, DebugLogger::ERROR);
				return;
			}

			entityArray[entity] = signature;
		}
		// Gets the signature of bits of the an entity which
		//	are avaliable or valid or enabled
		ComponentSignature& GetSignature(const Entity& entity)
		{
			if (entity >= MAX_ENTITIES)
			{
				DebugLogger::GetInstance().NewTraceMessage(
					"ATTEMPTED TO GET COMPONENT SIGNATURE OF ENTITY OUT OF RANGE"
					, DebugLogger::ERROR);
				assert(false);
			}

			return entityArray[entity];
		}
	private:
		EntityManager()
		{
			for (Entity entity = 0; entity < MAX_ENTITIES; ++entity)
			{
				unusedEntities.push(entity);
			}
		}
		~EntityManager() {}

		EntityManager(const EntityManager&) = delete;
		EntityManager& operator=(const EntityManager&) = delete;

		// a queue of entity id's that are current unsued
		std::queue<Entity> unusedEntities;
		// an array of component signatures to define what components
		//	an ID has
		std::array<ComponentSignature, MAX_ENTITIES> entityArray;
		uint32_t entityCount = 0;
	};
}
