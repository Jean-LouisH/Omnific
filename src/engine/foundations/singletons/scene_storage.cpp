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

#include "foundations/singletons/scene_storage.hpp"
#include <foundations/singletons/event_bus.hpp>
#include "platform/platform.hpp"

Omnific::SceneStorage* Omnific::SceneStorage::instance = nullptr;

void Omnific::SceneStorage::pre_load_scene(std::shared_ptr<Scene> scene)
{
	SceneStorage* scene_storage = SceneStorage::get_instance();
	std::string scene_name = scene->get_name();

	if (!scene_storage->has_scene(scene_name))
	{
		scene_storage->scenes.emplace(scene_name, scene);
		Platform::get_logger().write("Preloaded Scene: \"" + scene_name + "\"");
		EventBus::publish(OMNIFIC_EVENT_SCENE_PRELOADED);
	}
	else
	{
		Platform::get_logger().write("Error: Attempted to add Scene: \"" + scene_name + "\" which already exists.");
	}
}

void Omnific::SceneStorage::pre_load_scene(std::string filepath)
{
	SceneStorage* scene_storage = SceneStorage::get_instance();

	if (!scene_storage->has_scene(filepath))
	{
		std::shared_ptr<Scene> scene(new Scene(filepath));
		scene_storage->pre_load_scene(scene);
	}
}

void Omnific::SceneStorage::change_to_scene(std::shared_ptr<Scene> scene)
{
	SceneStorage* scene_storage = SceneStorage::get_instance();
	std::string scene_name = scene->get_name();

	if (!scene_storage->has_scene(scene_name))
	{
		scene_storage->pre_load_scene(scene);
	}

	scene_storage->active_scene_name = scene_name;
	Platform::get_logger().write("Changed to Scene: \"" + scene_name + "\"");
	scene_storage->active_scene_changed = true;
	EventBus::clear_instant_events();
	EventBus::clear_continuous_events();
	EventBus::publish(OMNIFIC_EVENT_ACTIVE_SCENE_CHANGED);
}

void Omnific::SceneStorage::change_to_scene(std::string scene_name)
{
	SceneStorage* scene_storage = SceneStorage::get_instance();
	std::shared_ptr<Scene> scene(new Scene(scene_name));
	scene_storage->change_to_scene(scene);
}

void Omnific::SceneStorage::load_scene(std::shared_ptr<Scene> scene)
{
	SceneStorage* scene_storage = SceneStorage::get_instance();
	std::string scene_name = scene->get_name();

	if (!scene_storage->has_scene(scene_name))
	{
		scene_storage->scenes.emplace(scene_name, scene);
	}
	{
		scene_storage->scenes.at(scene_name)->reload();
	}

	scene_storage->active_scene_name = scene_name;
	scene_storage->active_scene_changed = true;
	EventBus::clear_instant_events();
	EventBus::clear_continuous_events();
	EventBus::publish(OMNIFIC_EVENT_ACTIVE_SCENE_CHANGED);
	EventBus::publish(OMNIFIC_EVENT_SCENE_LOADED);
	Platform::get_logger().write("Loaded Scene: \"" + scene_name + "\"");
}

void Omnific::SceneStorage::load_scene(std::string scene_name)
{
	SceneStorage* scene_storage = SceneStorage::get_instance();
	std::shared_ptr<Scene> scene(new Scene(scene_name));
	scene_storage->load_scene(scene);
}

void Omnific::SceneStorage::remove_scene(std::string scene_name)
{
	SceneStorage* scene_storage = SceneStorage::get_instance();

	if (scene_name != "")
	{
		if (scene_storage->has_scene(scene_name))
		{
			/*Removed Scenes are stored to allow the ouput thread to access their data
			before registering a new active scene, without the use of synchronization. */
			std::shared_ptr<Scene> scene_to_remove = scene_storage->scenes.at(scene_name);

			if (scene_storage->removed_scenes.count(scene_name))
			{
				scene_storage->removed_scenes.erase(scene_name);
			}

			scene_storage->removed_scenes.emplace(scene_name, scene_to_remove);

			if (scene_name == scene_storage->get_active_scene_name())
			{
				scene_storage->change_to_scene("");
			}

			scene_storage->scenes.erase(scene_name);
			Platform::get_logger().write("Removed Scene: \"" + scene_name + "\"");
		}

		EventBus::publish(OMNIFIC_EVENT_SCENE_REMOVED);
	}
	else
	{
		Platform::get_logger().write("Error: Attempted to remove empty Scene.");
	}
}

void Omnific::SceneStorage::reload_active_scene()
{
	SceneStorage* scene_storage = SceneStorage::get_instance();
	std::string active_scene_name = scene_storage->get_active_scene_name();

	if (active_scene_name != "" &&
		scene_storage->has_scene(active_scene_name))
	{
		if (scene_storage->removed_scenes.count(active_scene_name))
		{
			std::shared_ptr<Scene> previously_removed_scene = scene_storage->removed_scenes.at(active_scene_name);
			scene_storage->remove_scene(active_scene_name);
			previously_removed_scene->reload();
			scene_storage->change_to_scene(previously_removed_scene);
		}
		else
		{
			scene_storage->remove_scene(active_scene_name);
			scene_storage->change_to_scene(std::shared_ptr<Scene>(new Scene(active_scene_name)));
		}

		EventBus::publish(OMNIFIC_EVENT_ACTIVE_SCENE_RELOADED);
	}
}

std::shared_ptr<Omnific::Scene> Omnific::SceneStorage::get_active_scene()
{
	SceneStorage* scene_storage = SceneStorage::get_instance();
	return scene_storage->scenes.at(scene_storage->active_scene_name);
}

std::string Omnific::SceneStorage::get_active_scene_name()
{
	return SceneStorage::get_instance()->active_scene_name;
}

bool Omnific::SceneStorage::has_no_scenes()
{
	/*Accounting for Scenes other than the dummy scene.*/
	return SceneStorage::get_instance()->scenes.size() <= 1;
}

bool Omnific::SceneStorage::has_scene(std::string scene_name)
{
	return SceneStorage::get_instance()->scenes.count(scene_name);
}

bool Omnific::SceneStorage::has_active_scene_changed()
{
	SceneStorage* scene_storage = SceneStorage::get_instance();
	bool result = scene_storage->active_scene_changed;
	return result;
}

void Omnific::SceneStorage::clear_scenes()
{
	SceneStorage* scene_storage = SceneStorage::get_instance();
	scene_storage->scenes.clear();
	scene_storage->active_scene_name = "";
}

std::shared_ptr<Omnific::Scene> Omnific::SceneStorage::get_scene_by_name(std::string scene_name)
{
	SceneStorage* scene_storage = SceneStorage::get_instance();
	std::shared_ptr<Scene> scene;

	if (scene_storage->has_scene(scene_name))
	{
		scene = scene_storage->scenes.at(scene_name);
	}

	return scene;
}

Omnific::SceneStorage* Omnific::SceneStorage::get_instance()
{
	if (instance == nullptr)
	{
		instance = new SceneStorage();
		instance->pre_load_scene(std::shared_ptr<Scene>(new Scene()));
	}
	return instance;
}