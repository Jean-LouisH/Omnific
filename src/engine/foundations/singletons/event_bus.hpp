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

#include <unordered_map>
#include <vector>
#include <string>
#include <engine_api.hpp>
#include <stdint.h>
#include <scene/components/component.hpp>

#define OMNIFIC_EVENT_ENTITY_ADDED "entity_added"
#define OMNIFIC_EVENT_ENTITY_NAME_SET "entity_name_set"
#define OMNIFIC_EVENT_ENTITY_TAG_SET "entity_tag_set"
#define OMNIFIC_EVENT_ENTITY_REMOVED "entity_removed"
#define OMNIFIC_EVENT_ENTITY_MODIFIED "entity_modified"
#define OMNIFIC_EVENT_COMPONENT_ADDED "component_added"
#define OMNIFIC_EVENT_COMPONENT_REMOVED "component_removed"
#define OMNIFIC_EVENT_SCENE_MODIFIED "scene_modified"
#define OMNIFIC_EVENT_ACTIVE_SCENE_CHANGED "active_scene_changed"
#define OMNIFIC_EVENT_SCENE_LOADED "scene_loaded"
#define OMNIFIC_EVENT_SCENE_REMOVED "scene_removed"
#define OMNIFIC_EVENT_ACTIVE_SCENE_RELOADED "active_scene_reloaded"

#define OMNIFIC_EVENT_ENTITY_IS_ON_COLLISION "entity_is_on_collision"
#define OMNIFIC_EVENT_ENTITY_IS_COLLIDING "entity_is_colliding"
#define OMNIFIC_EVENT_ENTITY_IS_OFF_COLLISION "entity_is_off_collision"

#define OMNIFIC_EVENT_HAPTIC_SIGNAL "haptic_signal"

#define OMNIFIC_EVENT_FILE_DROPPED_ON_WINDOW "file_dropped_on_window"

namespace Omnific
{
	class OMNIFIC_ENGINE_API Event
	{
	public:
		typedef struct Parameters
		{
			std::unordered_map<std::string, double> numbers;
			std::unordered_map<std::string, std::string> strings;
			std::unordered_map<std::string, bool> bools;
			std::unordered_map<std::string, std::shared_ptr<Component>> components;
			std::string key;
		};

		Event(std::string name, Parameters parameters);
		Event(std::string name);
		Event();

		/*Event data is read only to prevent multiple Systems or Scripts
		from overwriting it directly from reference from the EventQueue.*/

		std::string get_name();
		uint64_t get_timestamp();
		Parameters get_parameters();

	private:
		std::string name;
		uint64_t timestamp;
		Parameters parameters;
	};

	class OMNIFIC_ENGINE_API EventBus
	{
		friend class Engine;
		friend class SceneStorage;
	public:
		static void publish_event(
			Event event,
			bool is_continuous = false);

		static void publish_event(
			std::string event_name,
			std::unordered_map<std::string, std::string> strings = {},
			std::unordered_map<std::string, double> numbers = {},
			std::unordered_map<std::string, bool> booleans = {},
			std::unordered_map<std::string, std::shared_ptr<Component>> components = {},
			std::string key = "",
			bool is_continuous = false);

		static void remove_continuous_event(std::string event_name, std::string event_key);
		static std::vector<Event> query_events(std::string event_name);
		static std::unordered_map<std::string, Event> query_continuous_events(std::string event_name);
		static Event query_continuous_event(std::string event_name, std::string event_key);
		static std::vector<double> get_numbers(std::string event_name, std::string event_parameter_key);
		static std::vector<std::string> get_strings(std::string event_name, std::string event_parameter_key);
		static std::vector<bool> get_bools(std::string event_name, std::string event_parameter_key);
		static std::vector<std::shared_ptr<Component>> get_components(std::string event_name, std::string event_parameter_key);
		static std::vector<Event> query_events_with_number_parameter(std::string event_name, std::string event_parameter_key, double parameter_value);
		static std::vector<Event> query_events_with_string_parameter(std::string event_name, std::string event_parameter_key, std::string parameter_value);
		static std::vector<Event> query_events_with_bool_parameter(std::string event_name, std::string event_parameter_key, bool parameter_value);
		static std::vector<Event> query_events_with_component_parameter(std::string event_name, std::string event_parameter_key, std::shared_ptr<Component> parameter_value);
		static bool has_continuous_event(std::string event_name, std::string event_key);
		static uint64_t query_event_count(std::string event_name);
		static uint64_t query_event_count_with_parameter_key(std::string event_name, std::string event_parameter_key);
		static bool has_event(std::string event_name);
		static bool has_event_with_parameter_key(std::string event_name, std::string event_parameter_key);
		static EventBus* get_instance();
	private:
		static EventBus* instance;
		std::unordered_map<std::string, std::vector<Event>> instant_events;
		std::unordered_map<std::string, std::unordered_map<std::string, Event>> continuous_events;

		static void clear_instant_events();
		static void clear_continuous_events();
		static void publish_with_parameters(
			Event event,
			bool is_continuous);
	};
}