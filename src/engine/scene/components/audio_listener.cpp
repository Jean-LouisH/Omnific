#include "audio_listener.hpp"

void Omnific::AudioListener::deserialize(YAML::Node yaml_node)
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

void Omnific::AudioListener::set_volume(float value)
{
	if (value > 1.0)
		value = 1.0;
	else if (value < 0.0)
		value = 0.0;
	this->volume = value;
}

float Omnific::AudioListener::get_volume()
{
	return this->volume;
}

void Omnific::AudioListener::mute()
{
	this->previous_volume = this->volume;
	this->volume = 0.0;
}

void Omnific::AudioListener::unmute()
{
	this->volume = previous_volume;
}

std::vector<uint16_t>& Omnific::AudioListener::get_current_waveform()
{
	return this->current_waveform;
}