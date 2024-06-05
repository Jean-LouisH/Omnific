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
#include <string>
#include "foundations/aliases.hpp"
#include <memory>
#include <engine_api.hpp>

namespace Omnia
{
	/* Loads and stores multiple Scenes at once. */
	class OMNIA_ENGINE_API SceneStorage
	{
	public:
		static void pre_load_scene(std::shared_ptr<Scene> scene);
		static void pre_load_scene(std::string filepath);
		static void change_to_scene(std::shared_ptr<Scene> scene);
		static void change_to_scene(std::string scene_name);
		static void load_scene(std::shared_ptr<Scene> scene);
		static void load_scene(std::string scene_name);
		static void remove_scene(std::string scene_name);
		static void reload_active_scene();
		static std::shared_ptr<Scene> get_active_scene();
		static std::string get_active_scene_name();
		static bool has_no_scenes();
		static bool has_scene(std::string scene_name);
		static bool has_active_scene_changed();
		static void clear_scenes();
		static std::shared_ptr<Scene> get_scene_by_name(std::string scene_name);
		static SceneStorage* get_instance();
	private:
		static SceneStorage* instance;
		std::unordered_map<std::string, std::shared_ptr<Scene>> scenes;
		std::unordered_map<std::string, std::shared_ptr<Scene>> removed_scenes;
		std::string active_scene_name;
		bool active_scene_changed = false;
	};
}