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

#include "audio_source.hpp"

void Omnia::AudioSource::deserialize(YAML::Node yamlNode)
{
	for (YAML::const_iterator it3 = yamlNode.begin(); it3 != yamlNode.end(); ++it3)
	{
		if (it3->first.as<std::string>() == "default")
		{

		}
		else if (it3->first.as<std::string>() == "")
		{

		}
	}
}

void Omnia::AudioSource::addAudio(std::shared_ptr<Audio> audio)
{
	this->audio.emplace(audio->getName(), audio);
}

void Omnia::AudioSource::queueAudioToPlayAndRepeat(std::string audioName, uint8_t count)
{
	if (this->audio.count(audioName) > 0)
		for (int i = 0; i < count; i++)
			this->audioPlayQueue.emplace(this->audio.at(audioName));
}

void Omnia::AudioSource::queueAudioToPlay(std::string audioName)
{
	this->queueAudioToPlayAndRepeat(audioName, 1);
}

void Omnia::AudioSource::setVolume(float value)
{
	if (value > 1.0)
		value = 1.0;
	else if (value < 0.0)
		value = 0.0;
	this->volume = value;
}

float Omnia::AudioSource::getVolume()
{
	return this->volume;
}

void Omnia::AudioSource::clearAudio()
{
	this->audio.clear();
}

std::queue<std::shared_ptr<Omnia::Audio>> Omnia::AudioSource::popEntireAudioPlayQueue()
{
	std::queue<std::shared_ptr<Audio>> outputQueue = this->audioPlayQueue;
	this->clearAudioPlayQueue();
	return outputQueue;
}

void Omnia::AudioSource::clearAudioPlayQueue()
{
	for (int i = 0; i < this->audioPlayQueue.size(); i++)
		this->audioPlayQueue.pop();
}

std::vector<std::string> Omnia::AudioSource::getAudioNames()
{
	std::vector<std::string> audioStreamNames;

	for (std::unordered_map<std::string, std::shared_ptr<Audio>>::iterator it = this->audio.begin();
		it != this->audio.end();
		++it)
	{
		audioStreamNames.push_back(it->first);
	}

	return audioStreamNames;
}

std::shared_ptr<Omnia::Audio> Omnia::AudioSource::getAudioByName(std::string audioName)
{
	std::shared_ptr<Audio> audioStream;

	if (this->audio.count(audioName))
		audioStream = this->audio.at(audioName);

	return audioStream;
}