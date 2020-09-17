#include "AnimatedSprite2D.hpp"

void Lilliputian::AnimatedSprite2D::addEmptyFrameSequence(String frameSequenceName)
{
	FrameSequence frameSequence;
	this->frameSequences.emplace(frameSequenceName, frameSequence);
}

void Lilliputian::AnimatedSprite2D::addFrameSequence(String frameSequenceName, FrameSequence frameSequence)
{
	this->frameSequences.emplace(frameSequenceName, frameSequence);
}

void Lilliputian::AnimatedSprite2D::addFrameToFrameSequence(String frameSequenceName, Frame frame)
{
	if (this->frameSequences.count(frameSequenceName))
		this->frameSequences.at(frameSequenceName).push_back(frame);
}

void Lilliputian::AnimatedSprite2D::clearFrameSequences()
{
	this->frameSequences.clear();
}

void Lilliputian::AnimatedSprite2D::setAlpha(uint8_t value)
{
	if (value >= 0 && value <= 255)
		this->alpha = value;
}

uint8_t Lilliputian::AnimatedSprite2D::getAlpha()
{
	return this->alpha;
}

void Lilliputian::AnimatedSprite2D::hide()
{
	this->alpha = 0;
}

void Lilliputian::AnimatedSprite2D::show()
{
	this->alpha = 255;
}

void Lilliputian::AnimatedSprite2D::setAnimationSpeed(float value_fps)
{
	if (value_fps > 0.0)
		this->animationSpeed_fps = value_fps;
}

float Lilliputian::AnimatedSprite2D::getAnimationSpeed()
{
	return this->animationSpeed_fps;
}

void Lilliputian::AnimatedSprite2D::update(float delta_s)
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

void Lilliputian::AnimatedSprite2D::play(String frameSequenceName)
{
	if (this->frameSequences.count(frameSequenceName))
	{
		this->isPlaying = true;
		this->currentFrameSequenceName = frameSequenceName;
	}
}

void Lilliputian::AnimatedSprite2D::play()
{
	this->isPlaying = true;
}

void Lilliputian::AnimatedSprite2D::pause()
{
	this->isPlaying = false;
}

void Lilliputian::AnimatedSprite2D::stop()
{
	this->pause();
	this->currentFrameIndex = 0;
}

void Lilliputian::AnimatedSprite2D::setBackwards()
{
	this->isBackwards = true;
}
void Lilliputian::AnimatedSprite2D::setForwards()
{
	this->isBackwards = false;
}

void Lilliputian::AnimatedSprite2D::flipVertically()
{
	this->isFlippedVertically != this->isFlippedVertically;
}

void Lilliputian::AnimatedSprite2D::flipHorizontally()
{
	this->isFlippedHorizontally != this->isFlippedHorizontally;
}

Lilliputian::AnimatedSprite2D::Frame Lilliputian::AnimatedSprite2D::getCurrentFrame()
{
	return this->getCurrentFrameSequence().at(this->currentFrameIndex);
}

Lilliputian::Vector<Lilliputian::String> Lilliputian::AnimatedSprite2D::getFrameSequenceNames()
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

Lilliputian::AnimatedSprite2D::FrameSequence Lilliputian::AnimatedSprite2D::getFrameSequenceByName(String frameSequenceName)
{
	FrameSequence frameSequence;

	if (this->frameSequences.count(frameSequenceName))
		frameSequence = this->frameSequences.at(frameSequenceName);

	return frameSequence;
}

Lilliputian::AnimatedSprite2D::FrameSequence Lilliputian::AnimatedSprite2D::getCurrentFrameSequence()
{
	return this->frameSequences.at(this->currentFrameSequenceName);
}