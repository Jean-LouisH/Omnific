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

#include "animated_sprite.hpp"

void Lilliputian::AnimatedSprite::addEmptyFrameSequence(std::string frameSequenceName)
{
	FrameSequence frameSequence;
	this->frameSequences.emplace(frameSequenceName, frameSequence);
}

void Lilliputian::AnimatedSprite::addFrameSequence(std::string frameSequenceName, FrameSequence frameSequence)
{
	this->frameSequences.emplace(frameSequenceName, frameSequence);
}

void Lilliputian::AnimatedSprite::addFrameToFrameSequence(std::string frameSequenceName, Image frame)
{
	if (this->frameSequences.count(frameSequenceName))
		this->frameSequences.at(frameSequenceName).push_back(frame);
}

void Lilliputian::AnimatedSprite::clearFrameSequences()
{
	this->frameSequences.clear();
}

void Lilliputian::AnimatedSprite::setAlpha(uint8_t value)
{
	this->alpha = value;
	//apply to all frames...
}

uint8_t Lilliputian::AnimatedSprite::getAlpha()
{
	return this->alpha;
}

void Lilliputian::AnimatedSprite::hide()
{
	this->alpha = 0;
	//apply to all frames...
}

void Lilliputian::AnimatedSprite::show()
{
	this->alpha = 255;
	//apply to all frames...
}

void Lilliputian::AnimatedSprite::setAnimationSpeed(float value_fps)
{
	if (value_fps > 0.0)
		this->animationSpeed_fps = value_fps;
}

float Lilliputian::AnimatedSprite::getAnimationSpeed()
{
	return this->animationSpeed_fps;
}

void Lilliputian::AnimatedSprite::update(float delta_s)
{
	if (this->isPlaying)
	{
		this->frameTime_s += delta_s;

		if (this->frameTime_s > (1.0 / this->animationSpeed_fps))
		{
			uint32_t currentFrameSequenceSize = this->getCurrentFrameSequence().size();

			if (!this->isBackwards)
				this->currentFrameIndex = 
				this->currentFrameIndex + 1 % currentFrameSequenceSize;
			else
				this->currentFrameIndex = 
				this->currentFrameIndex - 1 < 0 ? currentFrameSequenceSize - 1 : this->currentFrameIndex - 1;

			this->frameTime_s = 0.0;
		}
	}
}

void Lilliputian::AnimatedSprite::play(std::string frameSequenceName)
{
	if (this->frameSequences.count(frameSequenceName))
	{
		this->isPlaying = true;
		this->currentFrameSequenceName = frameSequenceName;
	}
}

void Lilliputian::AnimatedSprite::play()
{
	this->isPlaying = true;
}

void Lilliputian::AnimatedSprite::pause()
{
	this->isPlaying = false;
}

void Lilliputian::AnimatedSprite::stop()
{
	this->pause();
	this->currentFrameIndex = 0;
}

void Lilliputian::AnimatedSprite::setBackwards()
{
	this->isBackwards = true;
}
void Lilliputian::AnimatedSprite::setForwards()
{
	this->isBackwards = false;
}

void Lilliputian::AnimatedSprite::flipVertically()
{
	this->isFlippedVertically != this->isFlippedVertically;
}

void Lilliputian::AnimatedSprite::flipHorizontally()
{
	this->isFlippedHorizontally != this->isFlippedHorizontally;
}

Lilliputian::Image Lilliputian::AnimatedSprite::getCurrentFrame()
{
	return this->getCurrentFrameSequence().at(this->currentFrameIndex);
}

std::vector<std::string> Lilliputian::AnimatedSprite::getFrameSequenceNames()
{
	std::vector<std::string> frameSequenceNames;

	for (std::unordered_map<std::string, FrameSequence>::iterator it = this->frameSequences.begin();
		it != this->frameSequences.end();
		++it)
	{
		frameSequenceNames.push_back(it->first);
	}

	return frameSequenceNames;
}

Lilliputian::AnimatedSprite::FrameSequence Lilliputian::AnimatedSprite::getFrameSequenceByName(std::string frameSequenceName)
{
	FrameSequence frameSequence;

	if (this->frameSequences.count(frameSequenceName))
		frameSequence = this->frameSequences.at(frameSequenceName);

	return frameSequence;
}

Lilliputian::AnimatedSprite::FrameSequence Lilliputian::AnimatedSprite::getCurrentFrameSequence()
{
	return this->frameSequences.at(this->currentFrameSequenceName);
}