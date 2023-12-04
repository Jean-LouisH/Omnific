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

#include <memory>
#include <unordered_map>
#include "registerable.hpp"
#include <core/singletons/os/os.hpp>

/// Include custom class headers here
////////////////////////////////////////////////////





// Core class headers
////////////////////////////////////////////////////
#include "core/components/audio_listener.hpp"
#include "core/components/audio_source.hpp"
#include "core/components/camera.hpp"
#include "core/components/collider.hpp"
#include "core/components/gui.hpp"
#include "core/components/light.hpp"
#include "core/components/model.hpp"
#include "core/components/physics_body.hpp"
#include "core/components/property_animation.hpp"
#include "core/components/script_collection.hpp"
#include "core/components/sprite.hpp"
#include "core/components/timer.hpp"
#include "core/components/transform.hpp"
#include "core/components/viewport.hpp"

#include <core/systems/animation_system/animation_system.hpp>
#include <core/systems/audio_system/audio_system.hpp>
#include <core/systems/cpp_scripting_system/cpp_scripting_system.hpp>
#include <core/systems/gui_system/gui_system.hpp>
#include <core/systems/haptic_system/haptic_system.hpp>
#include <core/systems/physics_system/physics_system.hpp>
#include <core/systems/python_scripting_system/python_scripting_system.hpp>
#include <core/systems/rendering_system/rendering_system.hpp>

namespace Omnia
{
	/* Provides an easy way to define lists of classes used throughout 
	   the Engine to enable customization and extension. */
	class OMNIA_ENGINE_API ClassRegistry
	{
	public:
		using DerivedClassName = std::string;
		using BaseClassName = std::string;

		static void add_class_definitions()
		{
			OS::get_logger().write("Loading class definitions to ClassRegistry...");
			ClassRegistry* registry = get_instance();

			// Add custom classes here.
			////////////////////////////////////////////




			// Core classes
			///////////////////////////////////////////
			registry->add<AudioListener, Component>();
			registry->add<AudioSource, Component>();
			registry->add<Camera, Component>();
			registry->add<Collider, Component>();
			registry->add<GUI, Component>();
			registry->add<Light, Component>();
			registry->add<PhysicsBody, Component>();
			registry->add<PropertyAnimation, Component>();
			registry->add<Model, Component>();
			registry->add<ScriptCollection, Component>();
			registry->add<Sprite, Component>();
			registry->add<Timer, Component>();
			registry->add<Transform, Component>();
			registry->add<Viewport, Component>();

			registry->add<AnimationSystem, System>();
			registry->add<AudioSystem, System>();
			registry->add<CPPScriptingSystem, System>();
			registry->add<GUISystem, System>();
			registry->add<HapticSystem, System>();
			registry->add<PhysicsSystem, System>();
			registry->add<PythonScriptingSystem, System>();
			registry->add<RenderingSystem, System>();

			////////////////////////////////////////////
		};

		static void initialize();
		static void finalize();

		template <class Derived, class Base>
		static void add()
		{
			ClassRegistry* registry = get_instance();

			if (registry->classes.count(Base::TYPE_STRING))
			{
				std::unordered_map<DerivedClassName, std::shared_ptr<Registerable>>& derived_classes = registry->classes.at(Base::TYPE_STRING);
				derived_classes.emplace(Derived::TYPE_STRING, std::static_pointer_cast<Registerable>(std::shared_ptr<Derived>(new Derived())));
			}
			else
			{
				std::unordered_map<DerivedClassName, std::shared_ptr<Registerable>> derived_classes;
				registry->classes.emplace(Base::TYPE_STRING, derived_classes);
			}
		}

		template <class Base>
		static std::unordered_map<DerivedClassName, std::shared_ptr<Registerable>> query_all()
		{
			ClassRegistry* registry = get_instance();

			std::unordered_map<DerivedClassName, std::shared_ptr<Registerable>> derived_classes;
			if (registry->classes.count(Base::TYPE_STRING))
				derived_classes = registry->classes.at(Base::TYPE_STRING);
			return derived_classes;
		}

		template <class Base>
		static std::shared_ptr<Registerable> query(std::string derived_class_name)
		{
			std::shared_ptr<Registerable> registerable;
			std::unordered_map<std::string, std::shared_ptr<Registerable>> bases = get_instance()->query_all<Base>();
			
			if (bases.count(derived_class_name))
				registerable = bases.at(derived_class_name);

			return registerable;
		}

		static ClassRegistry* get_instance();

	private:
		static ClassRegistry* instance;
		std::unordered_map<BaseClassName, std::unordered_map<DerivedClassName, std::shared_ptr<Registerable>>> classes;
	};
}