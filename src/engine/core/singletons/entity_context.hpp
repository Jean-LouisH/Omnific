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
#include "os/os.hpp"
#include <string>

#include <memory>
#include <core/component.hpp>
#include <core/scene/scene.hpp>
#include <core/singletons/scene_storage.hpp>
#include <core/utilities/aliases.hpp>
#include <core/scene/event_bus.hpp>
#include <engine_api.hpp>

namespace Omnia
{
	class OMNIA_ENGINE_API EntityContext
	{
	public:
		static void bindEntity(SceneTreeID sceneTreeID, EntityID entityID);
		static bool hasComponent(std::string type);
		static std::shared_ptr<Entity> getEntity();
		static std::shared_ptr<Scene> getScene();
		static std::shared_ptr<SceneTree> getSceneTree();
		static std::shared_ptr<Component> getComponent(std::string type);
		static float getTimeDelta();

		static EntityContext* getInstance();
	private:
		static EntityContext* instance;

		SceneTreeID boundSceneTreeID = 0;
		EntityID boundEntityID = 0;
	};
}

