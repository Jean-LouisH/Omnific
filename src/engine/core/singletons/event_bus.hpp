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

#define OMNIA_EVENT_ENTITY_ADDED "entity_added"
#define OMNIA_EVENT_ENTITY_NAME_SET "entity_name_set"
#define OMNIA_EVENT_ENTITY_TAG_SET "entity_tag_set"
#define OMNIA_EVENT_ENTITY_REMOVED "entity_removed"
#define OMNIA_EVENT_COMPONENT_ADDED "component_added"
#define OMNIA_EVENT_COMPONENT_REMOVED "component_removed"
#define OMNIA_EVENT_SCENE_PRELOADED "scene_preloaded"
#define OMNIA_EVENT_ACTIVE_SCENE_CHANGED "active_scene_changed"
#define OMNIA_EVENT_SCENE_LOADED "scene_loaded"
#define OMNIA_EVENT_SCENE_REMOVED "scene_removed"
#define OMNIA_EVENT_ACTIVE_SCENE_RELOADED "active_scene_reloaded"

namespace Omnia
{
	class OMNIA_ENGINE_API Event
	{
	public:
		typedef struct Parameters
		{
			std::unordered_map<std::string, double> numbers;
			std::unordered_map<std::string, std::string> strings;
		};

		Event(std::string name, uint64_t timestamp, Parameters parameters);
		Event(std::string name, uint64_t timestamp);

		/*Event data is read only to prevent multiple Systems or Scripts
		from overwriting it directly from reference from the EventQueue.*/

		std::string getName();
		uint64_t getTimestamp();
		Parameters getParameters();

	private:
		std::string name;
		uint64_t timestamp;
		Parameters parameters;
	};

	class OMNIA_ENGINE_API EventBus
	{
		friend class Engine;
	public:
		static void publish(
			std::string name,
			std::unordered_map<std::string, double> numbers,
			std::unordered_map<std::string, std::string> strings);

		static void publish(
			std::string name,
			std::unordered_map<std::string, double> numbers);

		static void publish(
			std::string name,
			std::unordered_map<std::string, std::string> strings);

		static void publish(
			std::string name);

		static std::vector<Event> query(std::string name);
		static std::vector<Event> queryOutputEvents(std::string name);
		static uint64_t queryCount(std::string name);
		static uint64_t queryOutputEventCount(std::string name);
		static EventBus* getInstance();
	private:
		static EventBus* instance;
		std::unordered_map<std::string, std::vector<Event>> updateEvents;
		std::unordered_map<std::string, std::vector<Event>> outputEvents;

		static void clear();
		static void clearOutputEvents();
		static void publishWithParameters(std::string name, Event::Parameters parameters);
	};
}