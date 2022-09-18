// MIT License
// 
// Copyright (c) 2020 Jean-Louis Haywood
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

#include <vector>
#include <stack>
#include <memory>
#include <queue>
#include <unordered_map>
#include "components/component.hpp"
#include "haptic_signal_buffer.hpp"
#include "entity.hpp"
#include "utilities/rectangle.hpp"
#include <unordered_map>
#include "utilities/aliases.hpp"
#include <string>
#include "collision_registry.hpp"
#include "event_bus.hpp"

#include <omnia_engine_api.hpp>

#include <memory>

namespace Omnia
{
	enum class CallType
	{
		START,
		UPDATE,
		FINISH
	};

	class OMNIA_ENGINE_API SceneTree
	{
	public:
		bool is2D = false;
		bool hasRenderableComponentsChanged = false;
		std::string name;

		SceneTree();

		void addEntity(std::shared_ptr<Entity> entity);
		void addEmptyEntity();
		void setEntityName(EntityID entityID, std::string name);
		void addEntityTag(EntityID entityID, std::string tag);
		void addComponent(EntityID entityID, std::shared_ptr<Component> component);
		void addComponentToLastEntity(std::shared_ptr<Component> component);

		void removeEntity(EntityID entityID);
		void removeComponent(EntityID entityID, std::string type);

		void clearStartEntityQueue();
		void clearFinishEntityQueue();

		std::queue<EntityID> getStartEntityQueue();
		std::queue<EntityID> getFinishEntityQueue();

		std::vector<std::shared_ptr<Component>> getComponents();
		std::vector<size_t> getRenderOrderIndexCache();
		std::unordered_map<std::string, std::vector<size_t>> getComponentIndexCaches();
		std::shared_ptr<Entity> getEntity(EntityID entityID);
		std::shared_ptr<Entity> getEntityByName(std::string name);
		std::shared_ptr<Entity> getLastEntity();
		std::unordered_map<EntityID, std::shared_ptr<Entity>>& getEntities();
		std::shared_ptr<Component> getComponentByID(ComponentID componentID);
		std::shared_ptr<Component> getComponent(std::string type, EntityID entityID);
		std::vector<std::shared_ptr<Component>> getComponentHierarchy(std::string type, EntityID entityID);
		std::shared_ptr<CollisionRegistry> getCollisionRegistry();
		std::shared_ptr<EventBus> getEventBus();
		std::shared_ptr<HapticSignalBuffer> getHapticSignalBuffer();
		bool getHasShadersChanged();
		bool getHasScriptsChanged();
		SceneTreeID getID();
		std::string getName();

		template <class T>
		std::shared_ptr<T> getComponentByType(EntityID entityID)
		{
			return std::dynamic_pointer_cast<T>(this->getComponent(T::TYPE_STRING, entityID));
		}

		template <class T>
		std::vector<std::shared_ptr<T>> getComponentHierarchyByType(EntityID entityID, bool replaceMissingComponents = true)
		{
			std::vector<std::shared_ptr<T>> typedComponentHierarchy;
			std::vector<std::shared_ptr<Component>> componentHierarchy = this->getComponentHierarchy(T::TYPE_STRING, entityID);

			for (std::shared_ptr<Component> component : componentHierarchy)
			{
				std::shared_ptr<T> typedComponent = std::dynamic_pointer_cast<T>(component);
				if (typedComponent == nullptr && replaceMissingComponents)
					typedComponent = std::shared_ptr<T>(new T());
				typedComponentHierarchy.push_back(typedComponent);
			}

			return typedComponentHierarchy;
		}

		template <class T>
		std::vector<std::shared_ptr<T>> getComponentsByType()
		{
			std::vector<std::shared_ptr<T>> componentsByType;
			std::vector<std::shared_ptr<Component>> components = this->getComponents();
			std::vector<size_t> componentIndices;
			std::unordered_map<std::string, std::vector<size_t>> componentIndexCaches = this->getComponentIndexCaches();

			if (componentIndexCaches.count(T::TYPE_STRING))
				componentIndices = componentIndexCaches.at(T::TYPE_STRING);

			size_t componentIndexCount = componentIndices.size();

			for (size_t i = 0; i < componentIndexCount; i++)
				componentsByType.push_back(std::dynamic_pointer_cast<T>(components.at(componentIndices.at(i))));

			return componentsByType;
		}
	private:
		SceneTreeID id = 0;

		/*Entities are stored in maps for fast random access
		when Components invoke changes in other Components
		attached to the Entity.*/
		std::unordered_map<EntityID, std::shared_ptr<Entity>> entities;
		std::unordered_map<std::string, EntityID> entityNames;
		std::unordered_map<std::string, EntityID> entityTags;

		/*Components are stored in vectors for fast linear access
		in engine system process loops.*/
		std::vector<std::shared_ptr<Component>> components;

		std::queue<EntityID> startEntitiesQueue;
		std::queue<EntityID> finishEntitiesQueue;

		EntityID lastEntityID = 0;

		std::shared_ptr<CollisionRegistry> collisionRegistry;
		std::shared_ptr<EventBus> eventBus;
		std::shared_ptr<HapticSignalBuffer> hapticSignalBuffer;

		std::unordered_map<std::string, std::vector<size_t>> componentIndexCaches;
		std::vector<size_t> renderOrderIndexCache;
	};
}