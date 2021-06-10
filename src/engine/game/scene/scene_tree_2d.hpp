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

#include "entity_2d.hpp"
#include "components/component_variant.hpp"
#include "utilities/rectangle.hpp"
#include <vector>
#include <map>
#include <queue>
#include "utilities/aliases.hpp"
#include "game/scripting/virtual_machine/script_call_batch.hpp"
#include <string>
#include "event_bus.hpp"

namespace Lilliputian
{
	class SceneTree2D
	{
	public:
		SceneTree2D();

		void addEntity2D(Entity2D entity2D);
		void addEmptyEntity2D();
		void addComponent(EntityID entityID, ComponentVariant componentVariant);
		void addComponentToLastEntity(ComponentVariant componentVariant);

		void removeEntity2D(EntityID entityID);
		void removeComponent(EntityID entityID, ComponentVariant::Type type);

		void changeCurrentCamera(ComponentID newCurrentCameraID);

		std::vector<ScriptCallBatch> generateOnStartCallBatches();
		std::vector<ScriptCallBatch> generateOnInputCallBatches();
		std::vector<ScriptCallBatch> generateOnFrameCallBatches();
		std::vector<ScriptCallBatch> generateOnComputeCallBatches();
		std::vector<ScriptCallBatch> generateOnLateCallBatches();
		std::vector<ScriptCallBatch> generateOnFinalCallBatches();

		ComponentID getCurrentCameraID();
		std::vector<ComponentVariant>& getComponentVariants();
		Transform2D& getEntityTransform(EntityID entityID);
		Entity2D& getEntity2D(EntityID entityID);
		Entity2D& getEntity2DByName(std::string name);
		Entity2D& getLastEntity2D();
		std::map<EntityID, Entity2D>& getEntity2Ds();
		EventBus& getEventBus();
		SceneTreeID getID();

		void unload();
	private:
		SceneTreeID ID = 0;

		/*Entities are stored in maps for fast random access
		when Components invoke changes in other Components
		attached to the Entity.*/
		std::map<EntityID, Entity2D> entities2D;

		/*Components are stored in vectors for fast linear access
		in engine system process loops.*/
		std::vector<ComponentVariant> componentVariants;
		std::vector<size_t> transform2DIndexCache;

		std::queue<EntityID> startEntitiesQueue;
		std::queue<EntityID> finishEntitiesQueue;

		ComponentID currentCameraID = -1;
		EntityID dummyEntityID = DUMMY_ENTITY;
		EntityID lastEntityID = DUMMY_ENTITY;

		EventBus eventBus;

		Entity2D& getDummyEntity2D();
	};
}