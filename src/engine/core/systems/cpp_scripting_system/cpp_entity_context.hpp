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

#include "core/scene/scene.hpp"
#include <core/singletons/scene_storage.hpp>
#include <core/singletons/configuration.hpp>
#include "core/utilities/aliases.hpp"
#include <core/singletons/os/os.hpp>
#include <string>

#include <memory>
#include <core/component.hpp>
#include <core/scene/scene.hpp>
#include <core/singletons/scene_storage.hpp>
#include <core/utilities/aliases.hpp>
#include <core/singletons/event_bus.hpp>
#include <engine_api.hpp>

namespace Omnia
{
	class OMNIA_ENGINE_API CPPEntityContext
	{
	public:
		static void bind_entity(SceneLayerID scene_layer_id, EntityID entity_id);
		static bool has_component(std::string type);
		static std::shared_ptr<Entity> get_entity();
		static std::shared_ptr<Scene> get_scene();
		static std::shared_ptr<SceneLayer> get_scene_layer();
		static std::shared_ptr<Component> get_component(std::string type);
		static float get_time_delta();

		static CPPEntityContext* get_instance();

		template <class T>
		static std::shared_ptr<T> get_component_by_type()
		{
			return std::dynamic_pointer_cast<T>(get_instance()->get_component(T::TYPE_STRING));
		}
	private:
		static CPPEntityContext* instance;

		SceneLayerID bound_scene_layer_id = 0;
		EntityID bound_entity_id = 0;
	};
}

