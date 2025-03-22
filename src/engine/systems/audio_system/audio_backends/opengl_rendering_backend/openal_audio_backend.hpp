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
#include <foundations/aliases.hpp>
#include "openal_buffer.hpp"
#include "openal_listener.hpp"
#include "openal_source.hpp"
#include <memory>
#include <foundations/resources/audio.hpp>
#include <scene/components/audio_source.hpp>
#include <scene/components/audio_listener.hpp>
#include <al.h>


namespace Omnific
{
	/* The object that calls the rendering API functions. */
	class OpenALAudioBackend
	{
	public:
		std::unordered_map<AssetID, std::shared_ptr<OpenALBuffer>> buffers;
		std::unordered_map<ComponentID, std::shared_ptr<OpenALListener>> listeners;
		std::unordered_map<ComponentID, std::shared_ptr<OpenALSource>> sources;
		uint8_t allowable_missed_frames = 0;
		std::unordered_map<UID, uint8_t> missed_frame_counts;

		void initialize();
		std::shared_ptr<OpenALBuffer> get_buffer(std::shared_ptr<Audio> audio);
		std::shared_ptr<OpenALListener> get_listener(std::shared_ptr<AudioListener> listener);
		std::shared_ptr<OpenALSource> get_source(std::shared_ptr<AudioSource> source);
		void collect_garbage();
	private:
	};
}

