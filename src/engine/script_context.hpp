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

#include "scene/scene.hpp"
#include <scene_storage.hpp>
#include <configuration.hpp>
#include "utilities/aliases.hpp"
#include "os/os.hpp"
#include <string>

#include <memory>
#include <scene/component.hpp>
#include <scene/scene.hpp>
#include <scene_serializer.hpp>
#include <scene_storage.hpp>
#include <utilities/aliases.hpp>
#include <scene/event_bus.hpp>
#include <omnia_engine_api.hpp>

namespace Omnia
{
	class OMNIA_ENGINE_API ScriptContext
	{
	public:
		static void bindEntity(SceneTreeID sceneTreeID, EntityID entityID);

		static void loadScene(std::string scenepath);

		static bool hasComponent(std::string type);
		static std::shared_ptr<Entity> getEntity();
		static std::shared_ptr<Scene> getScene();
		static std::shared_ptr<SceneTree> getSceneTree();
		static std::shared_ptr<Component> getComponent(std::string type);

		static ScriptContext* getInstance();
	private:
		static ScriptContext* instance;

		SceneTreeID boundSceneTreeID = 0;
		EntityID boundEntityID = 0;
	};
}

