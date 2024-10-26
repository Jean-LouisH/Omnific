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
#include <scene/scene.hpp>
#include <foundations/singletons/platform/platform.hpp>

void Omnific::Sprite::deserialize(YAML::Node yaml_node)
{
	Model::deserialize(yaml_node);

	for (YAML::const_iterator it3 = yaml_node.begin(); it3 != yaml_node.end(); ++it3)
	{
		if (it3->first.as<std::string>() == "image")
		{
			if (it3->second.as<std::string>() == "Image::default")
			{
				this->add_image(std::shared_ptr<Image>(new Image(it3->second.as<std::string>())));
			}
			else
			{
				this->add_image(Platform::get_file_access().load_resource_by_type<Image>(it3->second.as<std::string>()));
			}

			this->set_dimensions(this->get_image()->get_width(), this->get_image()->get_height(), 0.0);
		}
	}
}

void Omnific::Sprite::add_image(std::shared_ptr<Image> image)
{
	this->add_empty_frame_sequence("");
	this->add_image_to_frame_sequence("", image);
}

void Omnific::Sprite::add_empty_frame_sequence(std::string frame_sequence_name)
{
	std::vector<std::shared_ptr<Image>> frame_sequence;
	this->frame_sequences.emplace(frame_sequence_name, frame_sequence);
}

void Omnific::Sprite::add_frame_sequence(std::string frame_sequence_name, std::vector<std::shared_ptr<Image>> frame_sequence)
{
	this->frame_sequences.emplace(frame_sequence_name, frame_sequence);
	this->set_to_image(this->get_current_frame());
}

void Omnific::Sprite::add_image_to_frame_sequence(std::string frame_sequence_name, std::shared_ptr<Image> frame)
{
	if (this->frame_sequences.count(frame_sequence_name))
	{
		std::vector<std::shared_ptr<Image>> frame_sequence = this->frame_sequences.at(frame_sequence_name);
		this->frame_sequences.at(frame_sequence_name).push_back(frame);
		if (frame_sequence.size() == 1)
			this->set_dimensions(frame->get_width(), frame->get_height());
	}
	this->set_to_image(this->get_current_frame());
}

void Omnific::Sprite::clear_frame_sequences()
{
	this->frame_sequences.clear();
	this->current_frame_index = 0;
	this->set_to_image(this->get_current_frame());
}

void Omnific::Sprite::set_animation_speed(float value_fps)
{
	if (value_fps > 0.0)
		this->animation_speed_in_fps = value_fps;
}

float Omnific::Sprite::get_animation_speed()
{
	return this->animation_speed_in_fps;
}

void Omnific::Sprite::update(float delta_s)
{
	if (this->is_playing)
	{
		this->frame_time += delta_s;

		if (this->frame_time > (1.0 / this->animation_speed_in_fps))
		{
			uint32_t current_frame_sequence_size = this->get_current_frame_sequence().size();

			if (!this->is_backwards)
				this->current_frame_index = 
				this->current_frame_index + 1 % current_frame_sequence_size;
			else
				this->current_frame_index = 
				this->current_frame_index - 1 < 0 ? current_frame_sequence_size - 1 : this->current_frame_index - 1;

			this->frame_time = 0.0;
		}
	}

	this->set_to_image(this->get_current_frame());
}

void Omnific::Sprite::play(std::string frame_sequence_name)
{
	if (this->frame_sequences.count(frame_sequence_name))
	{
		this->is_playing = true;
		this->current_frame_sequence_name = frame_sequence_name;
	}
}

void Omnific::Sprite::play()
{
	this->is_playing = true;
}

void Omnific::Sprite::pause()
{
	this->is_playing = false;
}

void Omnific::Sprite::stop()
{
	this->pause();
	this->current_frame_index = 0;
}

void Omnific::Sprite::set_backwards()
{
	this->is_backwards = true;
}
void Omnific::Sprite::set_forwards()
{
	this->is_backwards = false;
}

void Omnific::Sprite::flip_vertically()
{
	this->is_flipped_vertically != this->is_flipped_vertically;
}

void Omnific::Sprite::flip_horizontally()
{
	this->is_flipped_horizontally != this->is_flipped_horizontally;
}

std::shared_ptr<Omnific::Image> Omnific::Sprite::get_current_frame()
{
	std::shared_ptr<Image> image = std::shared_ptr<Image>(new Image());
	if (this->frame_sequences.size() > 0)
		image = this->get_current_frame_sequence().at(this->current_frame_index);
	return image;
}

std::vector<std::string> Omnific::Sprite::get_frame_sequence_names()
{
	std::vector<std::string> frame_sequence_names;

	for (std::unordered_map<std::string, std::vector<std::shared_ptr<Image>>>::iterator it = this->frame_sequences.begin();
		it != this->frame_sequences.end();
		++it)
	{
		frame_sequence_names.push_back(it->first);
	}

	return frame_sequence_names;
}

std::vector<std::shared_ptr<Omnific::Image>> Omnific::Sprite::get_frame_sequence_by_name(std::string frame_sequence_name)
{
	std::vector<std::shared_ptr<Image>> frame_sequence;

	if (this->frame_sequences.count(frame_sequence_name))
		frame_sequence = this->frame_sequences.at(frame_sequence_name);

	return frame_sequence;
}

std::vector<std::shared_ptr<Omnific::Image>> Omnific::Sprite::get_current_frame_sequence()
{
	return this->frame_sequences.at(this->current_frame_sequence_name);
}