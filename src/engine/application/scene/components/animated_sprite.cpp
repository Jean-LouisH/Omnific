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

void Esi::AnimatedSprite::addEmptyFrameSequence(std::string frameSequenceName)
{
	FrameSequence frameSequence;
	this->frameSequences.emplace(frameSequenceName, frameSequence);
}

void Esi::AnimatedSprite::addFrameSequence(std::string frameSequenceName, FrameSequence frameSequence)
{
	this->frameSequences.emplace(frameSequenceName, frameSequence);
}

void Esi::AnimatedSprite::addFrameToFrameSequence(std::string frameSequenceName, Image frame)
{
	if (this->frameSequences.count(frameSequenceName))
		this->frameSequences.at(frameSequenceName).push_back(frame);
}

void Esi::AnimatedSprite::clearFrameSequences()
{
	this->frameSequences.clear();
}

void Esi::AnimatedSprite::setAlpha(uint8_t value)
{
	this->alpha = value;
	//apply to all frames...
}

uint8_t Esi::AnimatedSprite::getAlpha()
{
	return this->alpha;
}

void Esi::AnimatedSprite::hide()
{
	this->alpha = 0;
	//apply to all frames...
}

void Esi::AnimatedSprite::show()
{
	this->alpha = 255;
	//apply to all frames...
}

void Esi::AnimatedSprite::setAnimationSpeed(float value_fps)
{
	if (value_fps > 0.0)
		this->animationSpeed_fps = value_fps;
}

float Esi::AnimatedSprite::getAnimationSpeed()
{
	return this->animationSpeed_fps;
}

void Esi::AnimatedSprite::update(float delta_s)
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

void Esi::AnimatedSprite::play(std::string frameSequenceName)
{
	if (this->frameSequences.count(frameSequenceName))
	{
		this->isPlaying = true;
		this->currentFrameSequenceName = frameSequenceName;
	}
}

void Esi::AnimatedSprite::play()
{
	this->isPlaying = true;
}

void Esi::AnimatedSprite::pause()
{
	this->isPlaying = false;
}

void Esi::AnimatedSprite::stop()
{
	this->pause();
	this->currentFrameIndex = 0;
}

void Esi::AnimatedSprite::setBackwards()
{
	this->isBackwards = true;
}
void Esi::AnimatedSprite::setForwards()
{
	this->isBackwards = false;
}

void Esi::AnimatedSprite::flipVertically()
{
	this->isFlippedVertically != this->isFlippedVertically;
}

void Esi::AnimatedSprite::flipHorizontally()
{
	this->isFlippedHorizontally != this->isFlippedHorizontally;
}

Esi::Image Esi::AnimatedSprite::getCurrentFrame()
{
	return this->getCurrentFrameSequence().at(this->currentFrameIndex);
}

std::vector<std::string> Esi::AnimatedSprite::getFrameSequenceNames()
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

Esi::AnimatedSprite::FrameSequence Esi::AnimatedSprite::getFrameSequenceByName(std::string frameSequenceName)
{
	FrameSequence frameSequence;

	if (this->frameSequences.count(frameSequenceName))
		frameSequence = this->frameSequences.at(frameSequenceName);

	return frameSequence;
}

Esi::AnimatedSprite::FrameSequence Esi::AnimatedSprite::getCurrentFrameSequence()
{
	return this->frameSequences.at(this->currentFrameSequenceName);
}

std::string Esi::AnimatedSprite::getType() const
{
	return TYPE_STRING;
}

Esi::Image& Esi::AnimatedSprite::getImage()
{
	return this->getCurrentFrame();
}