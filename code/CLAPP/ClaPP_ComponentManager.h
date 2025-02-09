// Author:	Manoel McCadden
// File:	ClaPP_ComponentManager.h
// 
// ECS REFERENCE: https://austinmorlan.com/posts/entity_component_system/#what-is-an-ecs

#pragma once

#include <array>
#include <vector>
#include <unordered_map>

#include "ClaPP_Global.h"
#include "ClaPP_Memory.h"
#include "ClaPP_Component.h"

namespace ClaPP
{
	class ComponentManager
	{
	public:
		static ComponentManager& GetInstance()
		{
			static ComponentManager instance;
			return instance;
		}
		// Adds a component to the unordered map array
		void AddComponent(const Entity& entity, Component* data)
		{
			COMPONENTTYPE type = data->GetComponentType();
			components[type][entity] = data;
		}
		// Gets a raw pointer to the static memory component...
		//	must dynamic_cast to desired component
		inline Component* GetComponent(const Entity& entity, const COMPONENTTYPE& type)
		{
			// We can get an array of the desired component type
			//	and then use the build in find function to figure
			//	out if there is a component of that type in the 
			//	entity
			auto& componentArray = components[type];
			auto it = componentArray.find(entity);

			// If there is return that
			if (it != componentArray.end())
			{
				return it->second.GetPtr();
			}
			// else return a nullptr!
			return nullptr;
		}
		void DeleteComponent(const Entity& entity, const COMPONENTTYPE& _type)
		{
			components[_type][entity] = nullptr;
		}
		void DeleteAllEntityComponents(const Entity& entity)
		{
			// Search all component maps
			for (auto& it : components)
			{
				// If it finds a value for the entity
				//	erase it and remove it from the map!
				auto comp = it.find(entity);
				if (comp != it.end())
				{
					it.erase(comp);
				}
			}
		}
	private:
		ComponentManager() {}
		~ComponentManager() {}

		ComponentManager(const ComponentManager&) = delete;
		ComponentManager& operator=(const ComponentManager&) = delete;
		// This is an array that has an index for each type
		//	and an unordered map with entities and memory pointers
		//	to a component
		std::array<std::unordered_map<Entity, Static_Memory<Component>>
			, CT_COUNT> components;
	};

}