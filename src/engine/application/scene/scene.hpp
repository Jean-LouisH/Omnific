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
#include "component.hpp"
#include "application/scripting/virtual_machine/script_call_batch.hpp"
#include "asset_cache.hpp"
#include "haptic_signal_buffer.hpp"
#include "entity.hpp"
#include "utilities/rectangle.hpp"
#include <unordered_map>
#include "utilities/aliases.hpp"
#include <string>
#include "event_bus.hpp"

#include <application/scene/assets/animation.hpp>
#include <application/scene/assets/audio_stream.hpp>
#include <application/scene/assets/font.hpp>
#include <application/scene/assets/image.hpp>
#include <application/scene/assets/material.hpp>
#include <application/scene/assets/mesh.hpp>
#include <application/scene/assets/rig.hpp>
#include <application/scene/assets/shader.hpp>
#include <application/scene/assets/text.hpp>

#include "components/behaviour_tree.hpp"
#include "components/sight_perception.hpp"
#include "components/sound_perception.hpp"
#include "components/animated_sprite.hpp"
#include "components/audio_listener.hpp"
#include "components/audio_stream_source.hpp"
#include "components/camera.hpp"
#include "components/ball_collider.hpp"
#include "components/constant_directional_force.hpp"
#include "components/constant_point_force.hpp"
#include "components/countdown_timer.hpp"
#include "components/kinematic_body.hpp"
#include "components/light.hpp"
#include "components/navigation_mesh_agent.hpp"
#include "components/navigation_mesh_box_obstacle.hpp"
#include "components/navigation_path.hpp"
#include "components/physics_constraint.hpp"
#include "components/physics_thruster.hpp"
#include "components/property_animation.hpp"
#include "components/box_collider.hpp"
#include "components/box_mesh.hpp"
#include "components/box_trigger_space.hpp"
#include "components/regular_polytopal_mesh.hpp"
#include "components/rigid_body.hpp"
#include "components/sprite.hpp"
#include "components/static_fluid.hpp"
#include "components/transform.hpp"
#include "components/ui_button.hpp"
#include "components/ui_rectangle.hpp"
#include "components/ui_graph_edit.hpp"
#include "components/ui_graph_node.hpp"
#include "components/ui_scrollbar.hpp"
#include "components/ui_separator.hpp"
#include "components/ui_slider.hpp"
#include "components/ui_hover_card.hpp"
#include "components/ui_item_list.hpp"
#include "components/ui_panel.hpp"
#include "components/ui_progress_bar.hpp"
#include "components/ui_spin_box.hpp"
#include "components/ui_tab.hpp"
#include "components/ui_text_edit.hpp"
#include "components/ui_text_label.hpp"
#include "components/ui_tree.hpp"
#include "components/ui_viewport.hpp"

#include <memory>

namespace Esi
{
	class Scene
	{
	public:
		Scene();

		void addEntity(Entity entity);
		void addEmptyEntity();
		void addComponent(EntityID entityID, std::shared_ptr<Component>  component);
		void addComponentToLastEntity(std::shared_ptr<Component>  component);

		void removeEntity(EntityID entityID);
		void removeComponent(EntityID entityID, std::string type);

		std::vector<ScriptCallBatch> generateOnStartCallBatches();
		std::vector<ScriptCallBatch> generateOnInputCallBatches();
		std::vector<ScriptCallBatch> generateOnFrameCallBatches();
		std::vector<ScriptCallBatch> generateOnComputeCallBatches();
		std::vector<ScriptCallBatch> generateOnOutputCallBatches();
		std::vector<ScriptCallBatch> generateOnFinishBatches();

		std::vector<std::shared_ptr<Component>>& getComponents();
		std::vector<size_t> getRenderOrderIndexCache();
		std::unordered_map<std::string, std::vector<size_t>> getComponentIndexCaches();
		Transform& getEntityTransform(EntityID entityID);
		Entity& getEntity(EntityID entityID);
		Entity& getEntityByName(std::string name);
		Entity& getLastEntity();
		std::unordered_map<EntityID, Entity>& getEntities();
		Component& getComponent(ComponentID componentID);
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
		std::vector<std::shared_ptr<Component>> components;

		std::queue<EntityID> startEntitiesQueue;
		std::queue<EntityID> finishEntitiesQueue;

		EntityID dummyEntityID = DUMMY_ENTITY;
		EntityID lastEntityID = DUMMY_ENTITY;

		std::shared_ptr<AssetCache> assetCache;
		std::shared_ptr<EventBus> eventBus;
		std::shared_ptr<HapticSignalBuffer> hapticSignalBuffer;

		std::unordered_map<std::string, std::vector<size_t>> componentIndexCaches;
		std::vector<size_t> renderOrderIndexCache;

		Entity& getDummyEntity();
	};
}