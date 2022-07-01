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
#include "haptic_signal_buffer.hpp"
#include "entity.hpp"
#include "utilities/rectangle.hpp"
#include <unordered_map>
#include "utilities/aliases.hpp"
#include <string>
#include "collision_registry.hpp"
#include "event_bus.hpp"
#include "component_property_pool.hpp"

#include <scene/assets/animation.hpp>
#include <scene/assets/audio_stream.hpp>
#include "scene/assets/cpp_native_script.hpp"
#include <scene/assets/font.hpp>
#include <scene/assets/image.hpp>
#include <scene/assets/material.hpp>
#include <scene/assets/mesh.hpp>
#include "scene/assets/python_vm_script.hpp"
#include <scene/assets/rig.hpp>
#include <scene/assets/shader.hpp>
#include <scene/assets/text.hpp>

#include "components/acoustic_model_container.hpp"
#include "components/behaviour_tree.hpp"
#include "components/sight_perception.hpp"
#include "components/sound_perception.hpp"
#include "components/sprite_container.hpp"
#include "components/audio_listener.hpp"
#include "components/audio_source.hpp"
#include "components/camera.hpp"
#include "components/collider.hpp"
#include "components/constant_force.hpp"
#include "components/countdown_timer.hpp"
#include "components/cpu_particles.hpp"
#include "components/gpu_particles.hpp"
#include "components/character_body.hpp"
#include "components/directional_light.hpp"
#include "components/navigation_mesh_agent.hpp"
#include "components/navigation_mesh_box_obstacle.hpp"
#include "components/navigation_path.hpp"
#include "components/omnidirectional_light.hpp"
#include "components/property_animation.hpp"
#include "components/model_container.hpp"
#include "components/rigid_body.hpp"
#include "components/script_collection.hpp"
#include "components/soft_body.hpp"
#include "components/spot_light.hpp"
#include "components/static_fluid.hpp"
#include "components/transform.hpp"
#include "components/ui_accordion.hpp"
#include "components/ui_breadcrumbs.hpp"
#include "components/ui_dropdown.hpp"
#include "components/ui_button.hpp"
#include "components/ui_graph.hpp"
#include "components/ui_grid_view.hpp"
#include "components/ui_separator.hpp"
#include "components/ui_slider.hpp"
#include "components/ui_hover_card.hpp"
#include "components/ui_list.hpp"
#include "components/ui_loader.hpp"
#include "components/ui_panel.hpp"
#include "components/ui_placeholder.hpp"
#include "components/ui_progress_bar.hpp"
#include "components/ui_reveal.hpp"
#include "components/ui_search_field.hpp"
#include "components/ui_spin_box.hpp"
#include "components/ui_step.hpp"
#include "components/ui_tab.hpp"
#include "components/ui_text_field.hpp"
#include "components/ui_text_label.hpp"
#include "components/ui_tree_view.hpp"
#include "components/ui_toggle.hpp"
#include "components/ui_toggle_group.hpp"
#include "components/ui_viewport.hpp"

#include <memory>

namespace Omnia
{
	enum class CallType
	{
		START,
		UPDATE,
		FINISH
	};

	class SceneTree
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
		std::shared_ptr<Transform> getEntityTransform(EntityID entityID);
		std::shared_ptr<Entity> getEntity(EntityID entityID);
		std::shared_ptr<Entity> getEntityByName(std::string name);
		std::shared_ptr<Entity> getLastEntity();
		std::unordered_map<EntityID, std::shared_ptr<Entity>>& getEntities();
		std::shared_ptr<Component> getComponent(ComponentID componentID);
		std::shared_ptr<CollisionRegistry> getCollisionRegistry();
		std::shared_ptr<EventBus> getEventBus();
		std::shared_ptr<HapticSignalBuffer> getHapticSignalBuffer();
		bool getHasShadersChanged();
		bool getHasScriptsChanged();
		SceneTreeID getID();
		std::string getName();

		template <class T>
		std::shared_ptr<T> getComponent(EntityID entityID)
		{
			std::shared_ptr<T> typedComponent = std::shared_ptr<T>(new T());
			std::shared_ptr<Entity> entity = this->getEntity(entityID);

			if (entity->componentIDs.count(T::TYPE_STRING) > 0)
			{
				std::shared_ptr<Component> component = this->getComponent(entity->componentIDs.at(T::TYPE_STRING));
				typedComponent = std::dynamic_pointer_cast<T>(component);
			}

			return typedComponent;
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

		std::shared_ptr<ComponentPropertyPool> componentPropertyPool;

		std::queue<EntityID> startEntitiesQueue;
		std::queue<EntityID> finishEntitiesQueue;

		EntityID lastEntityID = 0;

		std::shared_ptr<CollisionRegistry> collisionRegistry;
		std::shared_ptr<EventBus> eventBus;
		std::shared_ptr<HapticSignalBuffer> hapticSignalBuffer;

		std::unordered_map<std::string, std::vector<size_t>> componentIndexCaches;
		std::vector<size_t> renderOrderIndexCache;

		uint8_t localLightCount = 0;
		uint8_t directionalLightCount = 0;

		const uint8_t allowableLocalLights = 8;
		const uint8_t allowableDirectionalLights = 1;
	};
}