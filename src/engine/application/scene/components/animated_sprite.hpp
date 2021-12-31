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

#include "utilities/aliases.hpp"
#include <vector>
#include <unordered_map>
#include "utilities/constants.hpp"
#include <string>
#include "application/scene/assets/image.hpp"
#include "application/scene/renderable_component.hpp"


namespace Esi
{
	class AnimatedSprite : public RenderableComponent
	{
		using FrameIndex = uint16_t;
		using FrameSequence = std::vector<Image>;
	public:
		std::string currentFrameSequenceName = "";

		void addEmptyFrameSequence(std::string frameSequenceName);
		void addFrameSequence(std::string frameSequenceName, FrameSequence frameSequence);
		void addFrameToFrameSequence(std::string frameSequenceName, Image frame);
		void clearFrameSequences();

		void setAlpha(uint8_t value);
		uint8_t getAlpha();
		void hide();
		void show();

		void setAnimationSpeed(float value_fps);
		float getAnimationSpeed();

		void update(float delta_s);

		void play(std::string frameSequenceName);
		void play();
		void pause();
		void stop();

		void setBackwards();
		void setForwards();

		void flipVertically();
		void flipHorizontally();

		Image getCurrentFrame();
		std::vector<std::string> getFrameSequenceNames();
		FrameSequence getFrameSequenceByName(std::string frameSequenceName);
		FrameSequence getCurrentFrameSequence();

		std::string getType() const override;
		Image& getImage() override;
	private:
		std::unordered_map<std::string, FrameSequence> frameSequences;
		uint8_t alpha = 255;
		float animationSpeed_fps = 12.0;
		float frameTime_s = 0.0;
		FrameIndex currentFrameIndex = 0;
		bool isPlaying = false;
		bool isBackwards = false;
		bool isFlippedVertically = false;
		bool isFlippedHorizontally = false;
	};
}