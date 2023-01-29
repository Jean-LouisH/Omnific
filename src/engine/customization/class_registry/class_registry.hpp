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
#include <core/systems/gui_system/gui_system.hpp>
#include <core/systems/haptic_system/haptic_system.hpp>
#include <core/systems/physics_system/physics_system.hpp>
#include <core/systems/rendering_system/rendering_system.hpp>
#include <core/systems/scripting_system/scripting_system.hpp>

/////////////////////////////////////////////////////

namespace Omnia
{
	/* Provides an easy way to define lists of classes used throughout 
	   the Engine to enable customization and extension. */
	class OMNIA_ENGINE_API ClassRegistry
	{
	public:
		using DerivedClassName = std::string;
		using BaseClassName = std::string;

		static void addClassDefinitions()
		{
			OS::getLogger().write("Loading class definitions to ClassRegistry...");
			ClassRegistry* registry = getInstance();

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
			registry->add<Model, Component>();
			registry->add<PhysicsBody, Component>();
			registry->add<PropertyAnimation, Component>();
			registry->add<ScriptCollection, Component>();
			registry->add<Sprite, Component>();
			registry->add<Timer, Component>();
			registry->add<Transform, Component>();
			registry->add<Viewport, Component>();

			registry->add<AnimationSystem, System>();
			registry->add<AudioSystem, System>();
			registry->add<GUISystem, System>();
			registry->add<HapticSystem, System>();
			registry->add<PhysicsSystem, System>();
			registry->add<ScriptingSystem, System>();
			registry->add<RenderingSystem, System>();

			////////////////////////////////////////////
		};

		static void initialize();
		static void finalize();

		template <class Derived, class Base>
		static void add()
		{
			ClassRegistry* registry = getInstance();

			if (registry->classes.count(Base::TYPE_STRING))
			{
				std::unordered_map<DerivedClassName, std::shared_ptr<Registerable>>& derivedClasses = registry->classes.at(Base::TYPE_STRING);
				derivedClasses.emplace(Derived::TYPE_STRING, std::static_pointer_cast<Registerable>(std::shared_ptr<Derived>(new Derived())));
			}
			else
			{
				std::unordered_map<DerivedClassName, std::shared_ptr<Registerable>> derivedClasses;
				registry->classes.emplace(Base::TYPE_STRING, derivedClasses);
			}
		}

		template <class Base>
		static std::unordered_map<DerivedClassName, std::shared_ptr<Registerable>> queryAll()
		{
			ClassRegistry* registry = getInstance();

			std::unordered_map<DerivedClassName, std::shared_ptr<Registerable>> derivedClasses;
			if (registry->classes.count(Base::TYPE_STRING))
				derivedClasses = registry->classes.at(Base::TYPE_STRING);
			return derivedClasses;
		}

		template <class Base>
		static std::shared_ptr<Registerable> query(std::string derivedClassName)
		{
			std::shared_ptr<Registerable> registerable;
			std::unordered_map<std::string, std::shared_ptr<Registerable>> bases = getInstance()->queryAll<Base>();
			
			if (bases.count(derivedClassName))
				registerable = bases.at(derivedClassName);

			return registerable;
		}

		static ClassRegistry* getInstance();

	private:
		static ClassRegistry* instance;
		std::unordered_map<BaseClassName, std::unordered_map<DerivedClassName, std::shared_ptr<Registerable>>> classes;
	};
}