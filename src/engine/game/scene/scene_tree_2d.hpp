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
#include "utilities/collections/vector.hpp"
#include "utilities/collections/map.hpp"
#include "utilities/collections/queue.hpp"
#include "utilities/aliases.hpp"
#include "game/scripting/virtual_machine/script_call_batch.hpp"
#include "utilities/string.hpp"

namespace Lilliputian
{
	class SceneTree2D
	{
	public:
		SceneTree2D();

		void addEntity2D(Entity2D entity2D);
		void addComponent(EntityID entityID, ComponentVariant componentVariant);
		void addScript(EntityID entityID, String script);

		Vector<ScriptCallBatch> generateOnStartCallBatches();
		Vector<ScriptCallBatch> generateOnInputCallBatches();
		Vector<ScriptCallBatch> generateOnFrameCallBatches();
		Vector<ScriptCallBatch> generateOnComputeCallBatches();
		Vector<ScriptCallBatch> generateOnLateCallBatches();
		Vector<ScriptCallBatch> generateOnFinalCallBatches();

		ComponentID getCurrentCameraIndex();
		Vector<ComponentVariant>& getComponentVariants();
		Transform2D& getEntityTransform(EntityID entityID);
		Entity2D& getEntity2D(EntityID entityID);

		void setID(SceneTreeID ID);

		void unload();
	private:
		SceneTreeID ID = 0;

		Map<EntityID, Entity2D> entities2D;
		Vector<ComponentVariant> componentVariants;
		Vector<ComponentID> transform2DIndexCache;

		Queue<EntityID> startEntitiesQueue;
		Queue<EntityID> finishEntitiesQueue;

		Rectangle windowDimensions;
		ComponentID currentCamera = -1;

		//Default, "empty" transform for rendering 
		//Transformless entities
		Transform2D* dummyTransform2D;
	};
}