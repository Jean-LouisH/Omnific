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
#include <foundations/singletons/profiler.hpp>

Omnific::SceneStorage* Omnific::SceneStorage::instance = nullptr;

void Omnific::SceneStorage::load_scene(std::shared_ptr<Scene> scene)
{
	SceneStorage* scene_storage = SceneStorage::get_instance();
	EventBus::publish_event(OMNIFIC_EVENT_CHANGE_SCENE_REQUESTED, {{"scene_name", scene->get_name()}});
	scene_storage->scene_change_request = scene;
	if (scene_storage->active_scene_name == "")
		scene_storage->service_scene_change_requests();
}

void Omnific::SceneStorage::load_scene(std::string scene_name)
{
	SceneStorage* scene_storage = SceneStorage::get_instance();
	EventBus::publish_event(OMNIFIC_EVENT_CHANGE_SCENE_REQUESTED, {{"scene_name", scene_name}});
	scene_storage->scene_change_request_name = scene_name;
	if (scene_storage->active_scene_name == "")
		scene_storage->service_scene_change_requests();
}

void Omnific::SceneStorage::remove_scene(std::string scene_name)
{
	SceneStorage* scene_storage = SceneStorage::get_instance();

	if (scene_name != "")
	{
		if (scene_storage->has_scene(scene_name))
		{
			std::shared_ptr<Scene> scene_to_remove = scene_storage->scenes.at(scene_name);

			if (scene_storage->removed_scenes.count(scene_name))
			{
				scene_storage->removed_scenes.erase(scene_name);
			}

			scene_storage->removed_scenes.emplace(scene_name, scene_to_remove);

			if (scene_name == scene_storage->get_active_scene_name())
			{
				scene_storage->load_scene("");
			}

			scene_storage->scenes.erase(scene_name);
			Platform::get_logger().write("Removed Scene: \"" + scene_name + "\"");
		}

		EventBus::publish_event(OMNIFIC_EVENT_SCENE_REMOVED);
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
		scene_storage->remove_scene(active_scene_name);
		scene_storage->active_scene_name = "";
		scene_storage->load_scene(std::shared_ptr<Scene>(new Scene(active_scene_name)));
		EventBus::publish_event(OMNIFIC_EVENT_ACTIVE_SCENE_RELOADED);
	}
}

void Omnific::SceneStorage::service_scene_change_requests()
{
	SceneStorage* scene_storage = SceneStorage::get_instance();
	std::shared_ptr<Scene> scene;

	if (scene_storage->scene_change_request_name != "")
	{
		EventBus::publish_event(OMNIFIC_EVENT_SERVICING_SCENE_CHANGE, {{"scene_name", scene_storage->scene_change_request_name}});
		scene = std::make_shared<Scene>(scene_storage->scene_change_request_name);
	}
	else
	{
		scene = scene_storage->scene_change_request;
	}

	if (scene != nullptr)
	{
		std::string scene_name = scene->get_name();

		if (scene_storage->has_scene(scene_name))
			scene_storage->remove_scene(scene_name);

		scene_storage->scenes.emplace(scene_name, scene);
		
		scene_storage->active_scene_name = scene_name;
		scene_storage->active_scene_changed = true;
		std::string active_scene_filepath = Platform::get_file_access().find_path(scene_name);
		scene_storage->active_scene_last_modified_time = Platform::get_file_access().get_last_modified_time(active_scene_filepath);
		EventBus::clear_continuous_events();
		EventBus::publish_event(OMNIFIC_EVENT_ACTIVE_SCENE_CHANGED);
		Platform::get_logger().write("Changed to Scene: \"" + scene_name + "\"");
		scene_storage->scene_change_request = nullptr;
		scene_storage->scene_change_request_name = "";
	}
}

std::shared_ptr<Omnific::Scene> Omnific::SceneStorage::get_active_scene()
{
	SceneStorage* scene_storage = SceneStorage::get_instance();
	std::shared_ptr<Scene> scene = scene_storage->scenes.at(scene_storage->active_scene_name);
	const int monitor_time_period = 1;
	scene_storage->modified_active_scene_monitor_clock->set_end();

	if (scene_storage->modified_active_scene_monitor_clock->get_delta_in_seconds() >= monitor_time_period)
	{
		FrameID current_frame_id = Profiler::get_frame_count();
		scene_storage->modified_active_scene_monitor_clock->set_start();

		if (Platform::get_file_access().get_last_modified_time(Platform::get_file_access().find_path(scene->get_name())) != 
			scene_storage->active_scene_last_modified_time && current_frame_id != scene_storage->last_reload_frame_id)
		{
			scene_storage->reload_active_scene();
			scene = scene_storage->scenes.at(scene_storage->active_scene_name);
			scene_storage->last_reload_frame_id = current_frame_id;
		}
	}

	return scene;
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
		std::shared_ptr<Scene> dummy_scene(std::shared_ptr<Scene>(new Scene()));
		instance->scenes.emplace(dummy_scene->get_name(), dummy_scene);
		instance->modified_active_scene_monitor_clock = std::make_shared<Clock>();
		instance->modified_active_scene_monitor_clock->set_start();
		instance->last_reload_frame_id = 0;
	}
	return instance;
}
