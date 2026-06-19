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

#include "animator.hpp"

void Omnific::Animator::deserialize(YAML::Node yaml_node)
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

void Omnific::Animator::play(std::string animation_name)
{
	this->play_repeatedly(animation_name, 0);
}

void Omnific::Animator::play_repeatedly(std::string animation_name, uint64_t repeat_count)
{
	std::shared_ptr<Animation> animation = this->get_animation(animation_name);
	if (!animation) return;
	
	animation->is_playing = true;
	animation->progress = 0.0;
	animation->repeat_count = 0;
	animation->allowable_repeats = repeat_count;
}

void Omnific::Animator::play_infinitely(std::string animation_name)
{
	this->play_repeatedly(animation_name, -1);
}

void Omnific::Animator::pause(std::string animation_name)
{
	std::shared_ptr<Animation> animation = this->get_animation(animation_name);
	if (!animation) return;

	animation->is_playing = false;

}

void Omnific::Animator::reset(std::string animation_name)
{
	this->play(animation_name);
}

void Omnific::Animator::stop(std::string animation_name)
{
	std::shared_ptr<Animation> animation = this->get_animation(animation_name);
	if (!animation) return;

	animation->is_playing = false;
	animation->progress = 0.0;
}

void Omnific::Animator::set_playback_speed(std::string animation_name)
{
	std::shared_ptr<Animation> animation = this->get_animation(animation_name);
	if (!animation) return;

	animation->is_playing = false;
	animation->progress = 0.0;
}

std::vector<std::string> Omnific::Animator::get_animation_names()
{
	std::vector<std::string> animation_names;

	for (auto& [animation_name, animation] : this->skeletal_animations)
	{
		animation_names.push_back(animation_name);
	}

	for (auto& [animation_name, animation] : this->property_animations)
	{
		animation_names.push_back(animation_name);
	}

	return animation_names;
}

float Omnific::Animator::get_animation_duration(std::string animation_name)
{
	std::shared_ptr<Animation> animation = this->get_animation(animation_name);
	if (!animation) return 0.0;

	return animation->duration;
}

std::shared_ptr<Omnific::Animation> Omnific::Animator::get_animation(std::string animation_name)
{
	std::shared_ptr<Animation> animation;

	if (this->skeletal_animations.count(animation_name))
	{
		animation = this->skeletal_animations[animation_name];
	}
	else if (this->property_animations.count(animation_name))
	{
		animation = this->property_animations[animation_name];
	}

	return animation;
}