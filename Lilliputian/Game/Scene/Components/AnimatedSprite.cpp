#include "AnimatedSprite.hpp"

void Lilliputian::AnimatedSprite::addEmptyFrameSequence(String frameSequenceName)
{
	FrameSequence frameSequence;
	this->frameSequences.emplace(frameSequenceName, frameSequence);
}

void Lilliputian::AnimatedSprite::addFrameSequence(String frameSequenceName, FrameSequence frameSequence)
{
	this->frameSequences.emplace(frameSequenceName, frameSequence);
}

void Lilliputian::AnimatedSprite::addFrameToFrameSequence(String frameSequenceName, Frame frame)
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
	if (value >= 0 && value <= 255)
		this->alpha = value;
}

uint8_t Lilliputian::AnimatedSprite::getAlpha()
{
	return this->alpha;
}

void Lilliputian::AnimatedSprite::hide()
{
	this->alpha = 0;
}

void Lilliputian::AnimatedSprite::show()
{
	this->alpha = 255;
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

void Lilliputian::AnimatedSprite::play(String frameSequenceName)
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

Lilliputian::AnimatedSprite::Frame Lilliputian::AnimatedSprite::getCurrentFrame()
{
	return this->getCurrentFrameSequence().at(this->currentFrameIndex);
}

Lilliputian::Vector<Lilliputian::String> Lilliputian::AnimatedSprite::getFrameSequenceNames()
{
	Vector<String> frameSequenceNames;

	for (Map<String, FrameSequence>::iterator it = this->frameSequences.begin();
		it != this->frameSequences.end();
		++it)
	{
		frameSequenceNames.push_back(it->first);
	}

	return frameSequenceNames;
}

Lilliputian::AnimatedSprite::FrameSequence Lilliputian::AnimatedSprite::getFrameSequenceByName(String frameSequenceName)
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