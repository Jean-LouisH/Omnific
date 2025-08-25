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

#ifdef ENABLE_PYTHON_BUILD

#pragma once

#include "scene/scene.hpp"
#include <foundations/singletons/scene_storage.hpp>
#include <foundations/singletons/configuration.hpp>
#include "foundations/aliases.hpp"
#include "foundations/singletons/platform/platform.hpp"
#include <string>

#include <memory>
#include <scene/components/component.hpp>
#include <scene/scene.hpp>
#include <foundations/singletons/scene_storage.hpp>
#include <foundations/aliases.hpp>
#include <foundations/singletons/event_bus.hpp>
#include <engine_api.hpp>

namespace Omnific
{
	class PythonEntityContext
	{
	public:
		static void bind_entity(EntityID entity_id);
		static void bind_time_delta(float time_delta);
		static bool has_component(std::string type);
		static std::shared_ptr<Entity> get_entity();
		static std::shared_ptr<Transform> get_transform();
		static std::shared_ptr<Scene> get_scene();
		static std::shared_ptr<Component> get_component(std::string type);
		static float get_time_delta();

		static PythonEntityContext* get_instance();
	private:
		static PythonEntityContext* instance;

		EntityID bound_entity_id = 0;
		float time_delta = 0.0;
	};
}

#endif /*ENABLE_PYTHON_BUILD*/
