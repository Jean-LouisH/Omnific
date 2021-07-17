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
#include <queue>
#include <unordered_map>
#include "component_variant.hpp"
#include "application/scripting/virtual_machine/script_call_batch.hpp"
#include "asset_cache.hpp"
#include "haptic_signal_buffer.hpp"
#include "entity.hpp"
#include "utilities/rectangle.hpp"
#include <unordered_map>
#include "utilities/aliases.hpp"
#include <string>
#include "event_bus.hpp"

namespace Lilliputian
{
	class Scene
	{
	public:
		Scene();

		void addEntity(Entity entity);
		void addEmptyEntity();
		void addComponent(EntityID entityID, ComponentVariant componentVariant);
		void addComponentToLastEntity(ComponentVariant componentVariant);

		void removeEntity(EntityID entityID);
		void removeComponent(EntityID entityID, ComponentVariant::Type type);

		std::vector<ScriptCallBatch> generateOnStartCallBatches();
		std::vector<ScriptCallBatch> generateOnInputCallBatches();
		std::vector<ScriptCallBatch> generateOnFrameCallBatches();
		std::vector<ScriptCallBatch> generateOnComputeCallBatches();
		std::vector<ScriptCallBatch> generateOnOutputCallBatches();
		std::vector<ScriptCallBatch> generateOnFinishBatches();

		std::vector<ComponentVariant>& getComponentVariants();
		std::vector<size_t> getRenderOrderIndexCache();
		std::unordered_map<ComponentVariant::Type, std::vector<size_t>> getComponentIndexCaches();
		Transform& getEntityTransform(EntityID entityID);
		Entity& getEntity(EntityID entityID);
		Entity& getEntityByName(std::string name);
		Entity& getLastEntity();
		std::unordered_map<EntityID, Entity>& getEntities();
		ComponentVariant& getComponent(ComponentID componentID);
		Entity::SpatialDimension getComponentSpatialDimension(ComponentID componentID);
		AssetCache& getAssetCache();
		EventBus& getEventBus();
		HapticSignalBuffer& getHapticSignalBuffer();
		SceneID getID();

		void unload();
	private:
		SceneID ID = 0;

		/*Entities are stored in maps for fast random access
		when Components invoke changes in other Components
		attached to the Entity.*/
		std::unordered_map<EntityID, Entity> entities;

		/*Components are stored in vectors for fast linear access
		in engine system process loops.*/
		std::vector<ComponentVariant> componentVariants;

		std::queue<EntityID> startEntitiesQueue;
		std::queue<EntityID> finishEntitiesQueue;

		EntityID dummyEntityID = DUMMY_ENTITY;
		EntityID lastEntityID = DUMMY_ENTITY;

		AssetCache assetCache;
		EventBus eventBus;
		HapticSignalBuffer hapticSignalBuffer;

		std::unordered_map<ComponentVariant::Type, std::vector<size_t>> componentIndexCaches;
		std::vector<size_t> renderOrderIndexCache;

		Entity& getDummyEntity();
	};
}