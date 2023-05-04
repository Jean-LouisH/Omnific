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

#include "core/utilities/aliases.hpp"
#include <set>
#include <queue>
#include "core/utilities/constants.hpp"
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include "core/assets/audio.hpp"
#include "core/component.hpp"
#include "core/assets/audio_synthesis.hpp"


namespace Omnia
{
	class OMNIA_ENGINE_API AudioSource : public Component
	{
	public:
		AudioSource()
		{
			this->type = TYPE_STRING;
		};
		static constexpr const char* TYPE_STRING = "AudioSource";

		virtual Registerable* instance() override
		{
			AudioSource* clone = new AudioSource(*this);
			clone->id = UIDGenerator::getNewUID();
			return clone;
		}

		virtual void deserialize(YAML::Node yamlNode);
		void addAudio(std::shared_ptr<Audio> audio);
		void clearAudio();
		void removeAudio(std::string audioName);
		void playAudio(std::string audioName);
		void play();
		void pause();
		void stop();
		void reset();
		uint16_t getRepeatCount();
		void jump(float timePoint);
		void setVolume(float value);
		void setPanning(float value);
		float getVolume();
		float getCurrentPlaybackTime();
		std::vector<std::string> getAudioNames();
		std::shared_ptr<Audio> getActiveAudio();
		std::shared_ptr<Audio> getAudioByName(std::string audioName);
	private:
		std::unordered_map<std::string, std::shared_ptr<Audio>> audioCollection;
		lb_Libretti* libretti;
		std::string activeAudioName;

		bool isPlayingSynthesized = false;

		float normalize(float value);
	};
}