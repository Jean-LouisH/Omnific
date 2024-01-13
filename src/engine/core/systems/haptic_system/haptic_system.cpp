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
#include "core/singletons/os/os.hpp"
#include <SDL.h>

Omnia::HapticSystem::~HapticSystem()
{
	this->finalize();
}

void Omnia::HapticSystem::rumble(HapticSignal& haptic_signal, std::vector<SDL_Haptic*> haptics)
{
	if (haptic_signal.get_player_id() < haptics.size())
	{
		SDL_HapticRumblePlay(haptics.at(haptic_signal.get_player_id()),
			haptic_signal.get_strength(),
			haptic_signal.get_duration());
	}
}

void Omnia::HapticSystem::stop_rumble(PlayerID player_id, std::vector<SDL_Haptic*> haptics)
{
	if (player_id < haptics.size())
	{
		SDL_HapticRumbleStop(haptics.at(player_id));
	}
}

void Omnia::HapticSystem::initialize()
{
	SDL_InitSubSystem(SDL_INIT_HAPTIC);
	this->is_initialized = true;
	OS::get_logger().write("Initialized Haptic System");
}

void Omnia::HapticSystem::on_late(std::shared_ptr<Scene> scene)
{
	Input& hid = OS::get_input();

	for (const auto scene_layer_it : scene->get_scene_layers())
	{
		std::shared_ptr<HapticSignalBuffer> haptic_signal_buffer = scene_layer_it.second->get_haptic_signal_buffer();
		std::unordered_map<PlayerID, std::queue<HapticSignal>>& haptic_signals = haptic_signal_buffer->get_haptic_signals();

		for (auto it = haptic_signals.begin(); it != haptic_signals.end(); it++)
		{
			PlayerID player_id = it->first;

			if (this->haptic_playbacks.count(player_id))
			{
				HapticPlayback& haptic_playback = this->haptic_playbacks.at(player_id);
				std::queue<HapticSignal>& haptic_signal_queue = it->second;

				if (haptic_playback.is_playing)
				{
					haptic_playback.timer.set_end();

					if (haptic_playback.timer.get_delta() > haptic_playback.duration_ms)
					{
						haptic_playback.is_playing = false;
						this->stop_rumble(player_id, hid.get_haptics());
						haptic_signal_queue.pop();
					}
				}
				else
				{
					if (!haptic_signal_queue.empty())
					{
						HapticSignal& haptic_signal = haptic_signals.at(player_id).front();
						haptic_playback.duration_ms = haptic_signal.get_duration();
						haptic_playback.timer.set_start();
						haptic_playback.is_playing = true;
						this->rumble(haptic_signal, hid.get_haptics());
					}
				}
			}
			else
			{
				HapticPlayback haptic_playback;
				haptic_playback.is_playing = false;
				this->haptic_playbacks.emplace(player_id, haptic_playback);
			}
		}
	}
}

void Omnia::HapticSystem::finalize()
{
	if (this->is_initialized)
		SDL_QuitSubSystem(SDL_INIT_HAPTIC);

	this->is_initialized = false;
}