#include "audio_listener.hpp"

void Omnia::AudioListener::deserialize(YAML::Node yamlNode)
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

void Omnia::AudioListener::setVolume(float value)
{
	if (value > 1.0)
		value = 1.0;
	else if (value < 0.0)
		value = 0.0;
	this->volume = value;
}

float Omnia::AudioListener::getVolume()
{
	return this->volume;
}

void Omnia::AudioListener::mute()
{
	this->previousVolume = this->volume;
	this->volume = 0.0;
}

void Omnia::AudioListener::unmute()
{
	this->volume = previousVolume;
}