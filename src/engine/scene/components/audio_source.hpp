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

#include "foundations/aliases.hpp"
#include <set>
#include <queue>
#include "foundations/constants.hpp"
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include "foundations/resources/audio_clip.hpp"
#include "scene/components/component.hpp"


namespace Omnific
{
	class OMNIFIC_ENGINE_API AudioSource : public Component
	{
		friend class AudioSystem;
	public:
		enum class PlaybackState
		{
			PLAYING,
			PAUSED,
			STOPPED
		};

		enum class AudioType
		{
			AUDIO_CLIP,
			MUSIC_SYNTHESIS,
			SOUND_FX_SYNTHESIS,
			AMBIANCE_SYNTHESIS,
			SPEECH_SYNTHESIS
		};

		AudioSource()
		{
			this->type = TYPE_STRING;
			this->playback_state = PlaybackState::STOPPED;
		};
		static constexpr const char* TYPE_STRING = "AudioSource";

		virtual Registerable* instance() override
		{
			AudioSource* clone = new AudioSource(*this);
			clone->id = UIDGenerator::get_new_uid();
			return clone;
		}

		bool is_capturing_waveform = false;
		bool is_looping = false;

		virtual void deserialize(YAML::Node yaml_node);
		void add_audio_clip(std::shared_ptr<AudioClip> audio_clip);
		void clear_audio_clip();
		void remove_audio_clip(std::string audio_clip_name);
		void play_audio_clip(std::string audio_clip_name);
		void play_audio_clip_infinitely(std::string audio_clip_name);
		void play();
		void play_infinitely();
		void pause();
		void resume();
		void stop();
		void reset();
		void jump(float time_point);
		void set_volume(float value);
		float get_volume();
		float get_current_playback_time();
		PlaybackState get_playback_state();
		std::vector<std::string> get_audio_clip_names();
		std::shared_ptr<AudioClip> get_active_audio_clip();
		std::shared_ptr<AudioClip> get_audio_clip_by_name(std::string audio_clip_name);
		std::vector<int16_t>& get_current_waveform();

	private:
		std::unordered_map<std::string, std::shared_ptr<AudioClip>> audio_clip_collection;
		std::vector<int16_t> current_waveform;
		std::string active_audio_clip_name;
		PlaybackState playback_state;
		float volume = 1.0;
		float playback_time = 0.0;
		AudioType audio_type = AudioType::AUDIO_CLIP;

		float clamp(float value);
	};
}