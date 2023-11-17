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

#include "audio_system.hpp"
#include <core/singletons/os/os.hpp>
#include <core/assets/audio_stream.hpp>
#include <Libretti.h>
#include "core/components/audio_listener.hpp"
#include <core/components/audio_source.hpp>

Omnia::AudioSystem::~AudioSystem()
{
	this->finalize();
}

void Omnia::AudioSystem::initialize()
{
	SDL_InitSubSystem(SDL_INIT_AUDIO);
	Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG);
	Mix_OpenAudio(44100, AUDIO_S16SYS, 2, pow(2, 11));
	lb_initialize();
	this->is_initialized = true;
	OS::get_logger().write("Initialized Audio System.");
}

void Omnia::AudioSystem::on_logic(std::shared_ptr<Scene> scene)
{
	lb_incrementAllPlayTimes(OS::get_profiler().get_timer(LOOP_THREAD_TIMER_NAME)->get_delta_in_seconds());
}

void Omnia::AudioSystem::on_late(std::shared_ptr<Scene> scene)
{
	std::unordered_map<SceneLayerID, std::shared_ptr<SceneLayer>>& scene_layers = scene->get_scene_layers();

	for (auto it = scene_layers.begin(); it != scene_layers.end(); it++)
	{
		std::vector<std::shared_ptr<AudioListener>> audio_listeners = it->second->get_components_by_type<AudioListener>();

		if (audio_listeners.size() > 0)
		{
			std::shared_ptr<AudioListener> audio_listener = audio_listeners.at(audio_listeners.size() - 1); //Get the last AudioListener.
		}
	}
}

void Omnia::AudioSystem::finalize()
{
	if (this->is_initialized)
	{
		Mix_CloseAudio();
		Mix_Quit();
		SDL_QuitSubSystem(SDL_INIT_AUDIO);
	}

	this->is_initialized = false;
}