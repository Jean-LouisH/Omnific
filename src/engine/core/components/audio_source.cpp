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

void Omnia::AudioSource::addAudioStream(std::shared_ptr<AudioStream> audioStream)
{
	this->audioStreams.emplace(audioStream->getName(), audioStream);
}

void Omnia::AudioSource::queueAudioToPlayAndRepeat(std::string audioStreamName, uint8_t count)
{
	if (this->audioStreams.count(audioStreamName) > 0)
		for (int i = 0; i < count; i++)
			this->audioPlayQueue.emplace(this->audioStreams.at(audioStreamName));
}

void Omnia::AudioSource::queueAudioToPlay(std::string audioStreamName)
{
	this->queueAudioToPlayAndRepeat(audioStreamName, 1);
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

void Omnia::AudioSource::clearAudioStreams()
{
	this->audioStreams.clear();
}

std::queue<std::shared_ptr<Omnia::AudioStream>> Omnia::AudioSource::popEntireAudioPlayQueue()
{
	std::queue<std::shared_ptr<AudioStream>> outputQueue = this->audioPlayQueue;
	this->clearAudioPlayQueue();
	return outputQueue;
}

void Omnia::AudioSource::clearAudioPlayQueue()
{
	for (int i = 0; i < this->audioPlayQueue.size(); i++)
		this->audioPlayQueue.pop();
}

std::vector<std::string> Omnia::AudioSource::getAudioStreamNames()
{
	std::vector<std::string> audioStreamNames;

	for (std::unordered_map<std::string, std::shared_ptr<AudioStream>>::iterator it = this->audioStreams.begin();
		it != this->audioStreams.end();
		++it)
	{
		audioStreamNames.push_back(it->first);
	}

	return audioStreamNames;
}

std::shared_ptr<Omnia::AudioStream> Omnia::AudioSource::getAudioStreamByName(std::string audioStreamName)
{
	std::shared_ptr<AudioStream> audioStream;

	if (this->audioStreams.count(audioStreamName))
		audioStream = this->audioStreams.at(audioStreamName);

	return audioStream;
}