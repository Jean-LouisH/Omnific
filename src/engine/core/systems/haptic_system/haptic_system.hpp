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

#include "core/scene/scene.hpp"
#include <SDL.h>
#include <vector>
#include <unordered_map>
#include "core/system.hpp"
#include "core/singletons/os/input.hpp"
#include <core/scene/haptic_signal_buffer.hpp>
#include <core/singletons/os/profiler.hpp>
#include <memory>

namespace Omnia
{
	/* Processes HapticSignals from the HapticSignalBuffer in the Scene.*/
	class HapticSystem : public System
	{
	public:
		HapticSystem()
		{
			this->type = TYPE_STRING;
		};

		~HapticSystem();

		static constexpr const char* TYPE_STRING = "HapticSystem";

		virtual Registerable* instance() override
		{
			return new HapticSystem(*this);
		}

		virtual void initializeOutput() override;
		virtual void onOutput(std::shared_ptr<Scene> scene) override;
		virtual void finalizeOutput() override;
	private:
		typedef struct HapticPlayback
		{
			HiResTimer timer;
			uint16_t duration_ms;
			bool isPlaying;
		};
		std::unordered_map<PlayerID, HapticPlayback> hapticPlaybacks;

		void rumble(HapticSignal& hapticSignal, std::vector<SDL_Haptic*> haptics);
		void stopRumble(PlayerID playerID, std::vector<SDL_Haptic*> haptics);
	};
}

