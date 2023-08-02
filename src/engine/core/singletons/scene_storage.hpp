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
#include <string>
#include "core/utilities/aliases.hpp"
#include <memory>
#include <engine_api.hpp>

namespace Omnia
{
	/* Loads and stores multiple Scenes at once. */
	class OMNIA_ENGINE_API SceneStorage
	{
	public:
		static void addScene(std::shared_ptr<Scene> scene);
		static void addScene(std::string filepath);
		static void changeToScene(std::shared_ptr<Scene> scene);
		static void changeToScene(std::string sceneName);
		static void removeScene(std::string sceneName);
		static void reloadActiveScene();
		static std::shared_ptr<Scene> getActiveScene();
		static std::string getActiveSceneName();
		static bool hasNoScenes();
		static bool hasScene(std::string sceneName);
		static bool hasActiveSceneChanged();
		static void clearScenes();
		static void finalizeUpdate();
		static std::shared_ptr<Scene> getSceneByName(std::string sceneName);
		static SceneStorage* getInstance();
	private:
		static SceneStorage* instance;
		std::unordered_map<std::string, std::shared_ptr<Scene>> scenes;
		std::unordered_map<std::string, std::shared_ptr<Scene>> removedScenes;
		std::string activeSceneName;
		bool activeSceneChanged = false;
	};
}