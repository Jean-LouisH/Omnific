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

/// Include class headers below
////////////////////////////////////////////////////

#include <asset_pipeline/assets/audio_stream.hpp>
#include "asset_pipeline/assets/cpp_script.hpp"
#include <asset_pipeline/assets/font.hpp>
#include <asset_pipeline/assets/image.hpp>
#include <asset_pipeline/assets/material.hpp>
#include <asset_pipeline/assets/mesh.hpp>
#include "asset_pipeline/assets/python_script.hpp"
#include <asset_pipeline/assets/rig.hpp>
#include <asset_pipeline/assets/shader.hpp>
#include <asset_pipeline/assets/text.hpp>

#include "scene/components/audio_listener.hpp"
#include "scene/components/audio_source.hpp"
#include "scene/components/camera.hpp"
#include "scene/components/collider.hpp"
#include "scene/components/gui.hpp"
#include "scene/components/light.hpp"
#include "scene/components/model_container.hpp"
#include "scene/components/physics_body.hpp"
#include "scene/components/property_animation.hpp"
#include "scene/components/script_collection.hpp"
#include "scene/components/sprite_container.hpp"
#include "scene/components/timer.hpp"
#include "scene/components/transform.hpp"
#include "scene/components/viewport.hpp"

#include <systems/animation_system/animation_system.hpp>
#include <systems/audio_system/audio_system.hpp>
#include <systems/gui_system/gui_system.hpp>
#include <systems/haptic_system/haptic_system.hpp>
#include <systems/physics_system/physics_system.hpp>
#include <systems/rendering_system/rendering_system.hpp>
#include <systems/scripting_system/scripting_system.hpp>

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
			ClassRegistry* registry = getInstance();

			// Add classes here.
			////////////////////////////////////////////

			registry->add<AudioStream, Asset>();
			registry->add<CPPScript, Asset>();
			registry->add<Font, Asset>();
			registry->add<Image, Asset>();
			registry->add<Material, Asset>();
			registry->add<Mesh, Asset>();
			registry->add<PythonScript, Asset>();
			registry->add<Rig, Asset>();
			registry->add<Shader, Asset>();
			registry->add<Text, Asset>();

			registry->add<AudioListener, Component>();
			registry->add<AudioSource, Component>();
			registry->add<Camera, Component>();
			registry->add<Collider, Component>();
			registry->add<GUI, Component>();
			registry->add<Light, Component>();
			registry->add<ModelContainer, Component>();
			registry->add<PhysicsBody, Component>();
			registry->add<PropertyAnimation, Component>();
			registry->add<ScriptCollection, Component>();
			registry->add<SpriteContainer, Component>();
			registry->add<Timer, Component>();
			registry->add<Transform, Component>();
			registry->add<Viewport, Component>();

			registry->add<AnimationSystem, System>();
			registry->add<AudioSystem, System>();
			registry->add<GUISystem, System>();
			registry->add<HapticSystem, System>();
			registry->add<PhysicsSystem, System>();
			registry->add<RenderingSystem, System>();
			registry->add<ScriptingSystem, System>();

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
		static std::unordered_map<DerivedClassName, std::shared_ptr<Registerable>> query()
		{
			ClassRegistry* registry = getInstance();

			std::unordered_map<DerivedClassName, std::shared_ptr<Registerable>> derivedClasses;
			if (registry->classes.count(Base::TYPE_STRING))
				derivedClasses = registry->classes.at(Base::TYPE_STRING);
			return derivedClasses;
		}

		static ClassRegistry* getInstance();

	private:
		static ClassRegistry* instance;
		std::unordered_map<BaseClassName, std::unordered_map<DerivedClassName, std::shared_ptr<Registerable>>> classes;
	};
}