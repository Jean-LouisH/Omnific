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

#include "haptic_system.hpp"
#include "foundations/singletons/platform/platform.hpp"
#include "foundations/singletons/event_bus.hpp"
#include <foundations/singletons/profiler.hpp>
#include <SDL.h>

#define HAPTIC_SYSTEM_ON_OUTPUT_FRAME_TIME_CLOCK_NAME "haptic_system_on_entity_finish_frame_time_clock"

Omnific::HapticSystem::~HapticSystem()
{
	this->finalize();
}

void Omnific::HapticSystem::initialize()
{
	SDL_InitSubSystem(SDL_INIT_HAPTIC);
	this->is_initialized = true;
	Profiler::add_clock(HAPTIC_SYSTEM_ON_OUTPUT_FRAME_TIME_CLOCK_NAME, {"haptic_system", "on_output_frame_time"});
	Platform::get_logger().write("Initialized Haptic System");
}

void Omnific::HapticSystem::on_output(std::shared_ptr<Scene> scene)
{
	std::shared_ptr<Clock> frame_time_clock = Profiler::get_clock(HAPTIC_SYSTEM_ON_OUTPUT_FRAME_TIME_CLOCK_NAME);
	frame_time_clock->set_start();
	Inputs& inputs = Platform::get_inputs();
	std::vector<Event> haptic_signal_events = EventBus::query_events(OMNIFIC_EVENT_HAPTIC_SIGNAL);

	for (size_t i = 0; i < haptic_signal_events.size(); i ++)
	{
		Event& haptic_signal_event = haptic_signal_events.at(i);
		Event::Parameters event_parameters = haptic_signal_event.get_parameters();
		std::unordered_map<std::string, double> event_numbers = haptic_signal_event.get_parameters().numbers;
		std::vector<SDL_Haptic*> haptics = inputs.get_haptics();

		PlayerID player_id = 0;
		uint16_t duration = 0;
		float strength = 0.0f;

		if (event_numbers.count("player_id"))
			player_id = event_numbers.at("player_id");
		if (event_numbers.count("duration"))
			duration = event_numbers.at("duration");
		if (event_numbers.count("strength"))
			strength = event_numbers.at("strength");

		if (player_id < haptics.size())
		{
			/*Every PlayerID event encountered will result in it and its 
		  	corresponding haptic_playback_hashtable being added.*/
			if (this->haptic_playback_hashtables.count(player_id) == 0)
			{	
				std::unordered_map<std::string, HapticPlayback> haptic_playback_hashtable;
				this->haptic_playback_hashtables.emplace(player_id, haptic_playback_hashtable);
			}

			std::unordered_map<std::string, HapticPlayback>& haptic_playback_hashtable = this->haptic_playback_hashtables.at(player_id);

			if (haptic_playback_hashtable.size() > 0)
			{
				/*While haptic_playbacks for a PlayerID are playing,
				  another one can be added to the PlayerID with a new key
				  and played over all previous ones.*/
				if (haptic_playback_hashtable.count(event_parameters.key) == 0)
				{
					HapticPlayback new_haptic_playback;
					new_haptic_playback.clock = std::make_shared<Clock>("haptic_playback");
					new_haptic_playback.event_key = event_parameters.key;
					new_haptic_playback.duration = duration;
					new_haptic_playback.strength = strength;
					new_haptic_playback.clock->set_start();
					haptic_playback_hashtable.emplace(event_parameters.key, new_haptic_playback);
				}

				/*Manage concurrently playing haptic_signal_events 
				  and then combine their strengths.*/
				float total_strength = 0.0;
				std::vector<std::string> haptic_playbacks_to_remove;

				for (auto haptic_playback_it : haptic_playback_hashtable)
				{
					HapticPlayback& haptic_playback = haptic_playback_it.second;
					haptic_playback.clock->set_end();

					if (haptic_playback.clock->get_delta() > haptic_playback.duration)
					{
						haptic_playbacks_to_remove.push_back(haptic_playback_it.first);
						EventBus::remove_continuous_event(OMNIFIC_EVENT_HAPTIC_SIGNAL, haptic_playback.event_key);
					}
					else
					{
						total_strength += haptic_playback.strength;
					}
				}

				for (size_t i = 0; i < haptic_playbacks_to_remove.size(); i++)
				{
					haptic_playback_hashtable.erase(haptic_playbacks_to_remove.at(i));
				}

				if (haptic_playback_hashtable.size() < 1)
				{
					SDL_HapticRumbleStop(haptics.at(player_id));
				}
				else
				{
					const float epsilon = 0.1f;

					if (!(previous_total_strength + epsilon > total_strength &&
						total_strength > previous_total_strength - epsilon))  
					{
						SDL_HapticRumbleStop(haptics.at(player_id));
						SDL_HapticRumblePlay(
							haptics.at(player_id),
							total_strength,
							duration);
						previous_total_strength = total_strength;
					}
				}
			}
			else
			{
				HapticPlayback haptic_playback;
				haptic_playback.clock = std::make_shared<Clock>("haptic_playback");
				haptic_playback.event_key = event_parameters.key;
				haptic_playback.duration = duration;
				haptic_playback.strength = strength;
				haptic_playback.clock->set_start();
				haptic_playback_hashtable.emplace(event_parameters.key, haptic_playback);

				SDL_HapticRumblePlay(haptics.at(player_id),
					strength,
					duration);
			}
		}
	}
	frame_time_clock->set_end();
}

void Omnific::HapticSystem::finalize()
{
	if (this->is_initialized)
		SDL_QuitSubSystem(SDL_INIT_HAPTIC);

	this->is_initialized = false;
}