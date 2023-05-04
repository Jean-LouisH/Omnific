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

#include "libretti_audio_system.hpp"
#include "core/scene/scene.hpp"
#include <Libretti.h>
#include "core/components/audio_source.hpp"
#include "core/components/audio_listener.hpp"
#include <core/singletons/configuration.hpp>
#include <core/singletons/os/os.hpp>

Omnia::LibrettiAudioSystem::~LibrettiAudioSystem()
{
	this->finalize();
}

void Omnia::LibrettiAudioSystem::initialize()
{
	lb_initialize();
	this->isInitialized = true;
	OS::getLogger().write("Initialized Libretti Audio System");
}

void Omnia::LibrettiAudioSystem::onLogic(std::shared_ptr<Scene> scene)
{
	lb_incrementAllPlayTimes(OS::getProfiler().getTimer(UPDATE_THREAD_TIMER_NAME)->getDeltaInSeconds());
}

void Omnia::LibrettiAudioSystem::onOutput(std::shared_ptr<Scene> scene)
{
	std::unordered_map<SceneLayerID, std::shared_ptr<SceneLayer>>& sceneLayers = scene->getSceneLayers();

	for (auto it = sceneLayers.begin(); it != sceneLayers.end(); it++)
	{
		std::vector<std::shared_ptr<AudioListener>> audioListeners = it->second->getComponentsByType<AudioListener>();

		if (audioListeners.size() > 0)
		{
			std::shared_ptr<AudioListener> audioListener = audioListeners.at(audioListeners.size() - 1); //Get the last AudioListener.
		}
	}
}

void Omnia::LibrettiAudioSystem::finalize()
{
	lb_finalize();
	this->isInitialized = false;
}