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

#include "sprite.hpp"
#include <core/scene/scene.hpp>
#include <core/singletons/os/os.hpp>

void Omnia::Sprite::deserialize(YAML::Node yamlNode)
{
	for (YAML::const_iterator it3 = yamlNode.begin(); it3 != yamlNode.end(); ++it3)
	{
		if (it3->first.as<std::string>() == "image")
		{
			if (it3->second.as<std::string>() == "Image::default")
			{
				this->addImage(std::shared_ptr<Image>(new Image(it3->second.as<std::string>())));
			}
			else
			{
				this->addImage(OS::getFileAccess().loadAssetByType<Image>(it3->second.as<std::string>()));
			}

			this->setDimensions(this->image->getWidth(), this->image->getHeight(), 0.0);
		}
		else if (it3->first.as<std::string>() == "dimensions")
		{
			this->setDimensions(
				it3->second[0].as<double>(),
				it3->second[1].as<double>(),
				it3->second[2].as<double>());
		}
		else if (it3->first.as<std::string>() == "alpha")
		{
			this->setAlpha((uint8_t)(it3->second.as<double>() * 255.0));
		}
	}
}

void Omnia::Sprite::addImage(std::shared_ptr<Image> image)
{
	this->addEmptyFrameSequence("");
	this->addImageToFrameSequence("", image);
}

void Omnia::Sprite::addEmptyFrameSequence(std::string frameSequenceName)
{
	std::vector<std::shared_ptr<Image>> frameSequence;
	this->frameSequences.emplace(frameSequenceName, frameSequence);
}

void Omnia::Sprite::addFrameSequence(std::string frameSequenceName, std::vector<std::shared_ptr<Image>> frameSequence)
{
	this->frameSequences.emplace(frameSequenceName, frameSequence);
	this->image = this->getCurrentFrame();
}

void Omnia::Sprite::addImageToFrameSequence(std::string frameSequenceName, std::shared_ptr<Image> frame)
{
	if (this->frameSequences.count(frameSequenceName))
	{
		std::vector<std::shared_ptr<Image>> frameSequence = this->frameSequences.at(frameSequenceName);
		this->frameSequences.at(frameSequenceName).push_back(frame);
		if (frameSequence.size() == 1)
			this->setDimensions(frame->getWidth(), frame->getHeight());
	}
	this->image = this->getCurrentFrame();
}

void Omnia::Sprite::clearFrameSequences()
{
	this->frameSequences.clear();
	this->currentFrameIndex = 0;
	this->image = this->getCurrentFrame();
}

void Omnia::Sprite::setAnimationSpeed(float value_fps)
{
	if (value_fps > 0.0)
		this->animationSpeedInFPS = value_fps;
}

float Omnia::Sprite::getAnimationSpeed()
{
	return this->animationSpeedInFPS;
}

void Omnia::Sprite::update(float delta_s)
{
	if (this->isPlaying)
	{
		this->frameTime += delta_s;

		if (this->frameTime > (1.0 / this->animationSpeedInFPS))
		{
			uint32_t currentFrameSequenceSize = this->getCurrentFrameSequence().size();

			if (!this->isBackwards)
				this->currentFrameIndex = 
				this->currentFrameIndex + 1 % currentFrameSequenceSize;
			else
				this->currentFrameIndex = 
				this->currentFrameIndex - 1 < 0 ? currentFrameSequenceSize - 1 : this->currentFrameIndex - 1;

			this->frameTime = 0.0;
		}
	}

	this->image = this->getCurrentFrame();
}

void Omnia::Sprite::play(std::string frameSequenceName)
{
	if (this->frameSequences.count(frameSequenceName))
	{
		this->isPlaying = true;
		this->currentFrameSequenceName = frameSequenceName;
	}
}

void Omnia::Sprite::play()
{
	this->isPlaying = true;
}

void Omnia::Sprite::pause()
{
	this->isPlaying = false;
}

void Omnia::Sprite::stop()
{
	this->pause();
	this->currentFrameIndex = 0;
}

void Omnia::Sprite::setBackwards()
{
	this->isBackwards = true;
}
void Omnia::Sprite::setForwards()
{
	this->isBackwards = false;
}

void Omnia::Sprite::flipVertically()
{
	this->isFlippedVertically != this->isFlippedVertically;
}

void Omnia::Sprite::flipHorizontally()
{
	this->isFlippedHorizontally != this->isFlippedHorizontally;
}

std::shared_ptr<Omnia::Image> Omnia::Sprite::getCurrentFrame()
{
	std::shared_ptr<Image> image = std::shared_ptr<Image>(new Image());
	if (this->frameSequences.size() > 0)
		image = this->getCurrentFrameSequence().at(this->currentFrameIndex);
	return image;
}

std::vector<std::string> Omnia::Sprite::getFrameSequenceNames()
{
	std::vector<std::string> frameSequenceNames;

	for (std::unordered_map<std::string, std::vector<std::shared_ptr<Image>>>::iterator it = this->frameSequences.begin();
		it != this->frameSequences.end();
		++it)
	{
		frameSequenceNames.push_back(it->first);
	}

	return frameSequenceNames;
}

std::vector<std::shared_ptr<Omnia::Image>> Omnia::Sprite::getFrameSequenceByName(std::string frameSequenceName)
{
	std::vector<std::shared_ptr<Image>> frameSequence;

	if (this->frameSequences.count(frameSequenceName))
		frameSequence = this->frameSequences.at(frameSequenceName);

	return frameSequence;
}

std::vector<std::shared_ptr<Omnia::Image>> Omnia::Sprite::getCurrentFrameSequence()
{
	return this->frameSequences.at(this->currentFrameSequenceName);
}