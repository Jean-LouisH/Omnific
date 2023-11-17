#include "audio_listener.hpp"

void Omnia::AudioListener::deserialize(YAML::Node yaml_node)
{
	for (YAML::const_iterator it3 = yaml_node.begin(); it3 != yaml_node.end(); ++it3)
	{
		if (it3->first.as<std::string>() == "default")
		{

		}
		else if (it3->first.as<std::string>() == "")
		{

		}
	}
}

void Omnia::AudioListener::set_volume(float value)
{
	if (value > 1.0)
		value = 1.0;
	else if (value < 0.0)
		value = 0.0;
	this->volume = value;
}

float Omnia::AudioListener::get_volume()
{
	return this->volume;
}

void Omnia::AudioListener::mute()
{
	this->previous_volume = this->volume;
	this->volume = 0.0;
}

void Omnia::AudioListener::unmute()
{
	this->volume = previous_volume;
}