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

#include "foundations/singletons/scene_manager.hpp"
#include <foundations/singletons/event_bus.hpp>
#include "platform/platform.hpp"
#include <foundations/singletons/profiler.hpp>

Omnific::SceneManager* Omnific::SceneManager::instance = nullptr;

void Omnific::SceneManager::load_scene(std::shared_ptr<Scene> scene)
{
	SceneManager* scene_manager = SceneManager::get_instance();
	EventBus::publish_event(OMNIFIC_EVENT_CHANGE_SCENE_REQUESTED, {{"scene_name", scene->get_name()}});
	scene_manager->scene_change_request = scene;
	if (scene_manager->has_no_scenes())
		scene_manager->service_scene_change_requests();
}

void Omnific::SceneManager::load_scene(std::string scene_name)
{
	SceneManager* scene_manager = SceneManager::get_instance();
	EventBus::publish_event(OMNIFIC_EVENT_CHANGE_SCENE_REQUESTED, {{"scene_name", scene_name}});
	scene_manager->scene_change_request_name = scene_name;
	if (scene_manager->has_no_scenes())
		scene_manager->service_scene_change_requests();
}

void Omnific::SceneManager::remove_scene(std::string scene_name)
{
	SceneManager* scene_manager = SceneManager::get_instance();

	if (scene_manager->has_scene(scene_name))
	{
		std::shared_ptr<Scene> scene_to_remove = scene_manager->scenes.at(scene_name);

		if (scene_manager->removed_scenes.count(scene_name))
		{
			scene_manager->removed_scenes.erase(scene_name);
		}

		scene_manager->removed_scenes.emplace(scene_name, scene_to_remove);

		if (scene_name == scene_manager->get_active_scene_name())
		{
			scene_manager->load_scene("");
		}

		scene_manager->scenes.erase(scene_name);
		Platform::get_logger().write("Removed Scene: \"" + scene_name + "\"");
		EventBus::publish_event(OMNIFIC_EVENT_SCENE_REMOVED);
	}
}

void Omnific::SceneManager::reload_active_scene()
{
	SceneManager* scene_manager = SceneManager::get_instance();
	std::string active_scene_name = scene_manager->get_active_scene_name();

	if (active_scene_name != "" &&
		scene_manager->has_scene(active_scene_name))
	{
		scene_manager->remove_scene(active_scene_name);
		scene_manager->active_scene_name = "";
		scene_manager->load_scene(std::shared_ptr<Scene>(new Scene(active_scene_name)));
		EventBus::publish_event(OMNIFIC_EVENT_ACTIVE_SCENE_RELOADED);
	}
}

void Omnific::SceneManager::service_scene_change_requests()
{
	SceneManager* scene_manager = SceneManager::get_instance();
	std::shared_ptr<Scene> scene;

	if (scene_manager->scene_change_request_name != "")
	{
		EventBus::publish_event(OMNIFIC_EVENT_SERVICING_SCENE_CHANGE, {{"scene_name", scene_manager->scene_change_request_name}});
		scene = std::make_shared<Scene>(scene_manager->scene_change_request_name);
	}
	else
	{
		scene = scene_manager->scene_change_request;
	}

	if (scene != nullptr)
	{
		std::string scene_name = scene->get_name();

		if (scene_manager->has_scene(scene_name))
			scene_manager->remove_scene(scene_name);

		scene_manager->scenes.emplace(scene_name, scene);
		
		scene_manager->active_scene_name = scene_name;
		scene_manager->active_scene_changed = true;
		std::string active_scene_filepath = Platform::get_file_access().find_path(scene_name);
		scene_manager->active_scene_last_modified_time = Platform::get_file_access().get_last_modified_time(active_scene_filepath);
		EventBus::clear_continuous_events();
		EventBus::publish_event(OMNIFIC_EVENT_ACTIVE_SCENE_CHANGED);
		Platform::get_logger().write("Changed to Scene: \"" + scene_name + "\"");
		scene_manager->scene_change_request = nullptr;
		scene_manager->scene_change_request_name = "";
	}
}

std::shared_ptr<Omnific::Scene> Omnific::SceneManager::get_active_scene()
{
	SceneManager* scene_manager = SceneManager::get_instance();
	std::shared_ptr<Scene> scene = scene_manager->scenes.at(scene_manager->active_scene_name);
	const int monitor_time_period = 1;
	scene_manager->modified_active_scene_monitor_clock->set_end();

	if (scene_manager->modified_active_scene_monitor_clock->get_delta_in_seconds() >= monitor_time_period)
	{
		FrameID current_frame_id = Profiler::get_frame_count();
		scene_manager->modified_active_scene_monitor_clock->set_start();

		if (Platform::get_file_access().get_last_modified_time(Platform::get_file_access().find_path(scene->get_name())) != 
			scene_manager->active_scene_last_modified_time && current_frame_id != scene_manager->last_reload_frame_id)
		{
			scene_manager->reload_active_scene();
			scene = scene_manager->scenes.at(scene_manager->active_scene_name);
			scene_manager->last_reload_frame_id = current_frame_id;
		}
	}

	return scene;
}

std::string Omnific::SceneManager::get_active_scene_name()
{
	return SceneManager::get_instance()->active_scene_name;
}

bool Omnific::SceneManager::has_no_scenes()
{
	return SceneManager::get_instance()->scenes.size() == 0;
}

bool Omnific::SceneManager::has_scene(std::string scene_name)
{
	return SceneManager::get_instance()->scenes.count(scene_name);
}

bool Omnific::SceneManager::has_active_scene_changed()
{
	SceneManager* scene_manager = SceneManager::get_instance();
	bool result = scene_manager->active_scene_changed;
	return result;
}

void Omnific::SceneManager::clear_scenes()
{
	SceneManager* scene_manager = SceneManager::get_instance();
	scene_manager->scenes.clear();
	scene_manager->active_scene_name = "";
}

std::shared_ptr<Omnific::Scene> Omnific::SceneManager::get_scene_by_name(std::string scene_name)
{
	SceneManager* scene_manager = SceneManager::get_instance();
	std::shared_ptr<Scene> scene;

	if (scene_manager->has_scene(scene_name))
	{
		scene = scene_manager->scenes.at(scene_name);
	}

	return scene;
}

Omnific::SceneManager* Omnific::SceneManager::get_instance()
{
	if (instance == nullptr)
	{
		instance = new SceneManager();
		instance->modified_active_scene_monitor_clock = std::make_shared<Clock>();
		instance->modified_active_scene_monitor_clock->set_start();
		instance->last_reload_frame_id = 0;
	}
	return instance;
}
