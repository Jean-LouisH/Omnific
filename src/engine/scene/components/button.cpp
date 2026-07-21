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

#include "button.hpp"
#include <foundations/singletons/platform/platform.hpp>
#include <foundations/singletons/resource_manager.hpp>

void Omnific::Button::deserialize(YAML::Node yaml_node)
{
	GUIElement::deserialize(yaml_node);

	this->label = std::shared_ptr<Label>(new Label());

	for (YAML::const_iterator it3 = yaml_node.begin(); it3 != yaml_node.end(); ++it3)
	{
		if (it3->first.as<std::string>() == "text")
		{
			this->label->text = it3->second.as<std::string>();
		}
		else if (it3->first.as<std::string>() == "font")
		{
			std::shared_ptr<Omnific::Font> font = ResourceManager::load_resource<Font>(it3->second[0].as<std::string>());
			*font = Font(font->get_name(), it3->second[1].as<int>());
			this->label->font = font;
		}
		else if (it3->first.as<std::string>() == "text_colour")
		{
			this->label->text_colour = std::shared_ptr<Colour>(new Colour(it3->second.as<std::string>()));
		}
	}

	this->label->update_image();
	this->update_image();
}

void Omnific::Button::update_image()
{
	if (!this->is_hidden())
	{
		this->highlight_on_input();
		
		/* The border of the button wraps around the text it contains by an offset. */
		this->dimensions = this->label->get_dimensions() + glm::vec3(this->button_space_from_text, 0.0);
		std::shared_ptr<Image> base_button_image = std::shared_ptr<Image>(new Image(this->background_colour, this->dimensions.x, this->dimensions.y));
		std::shared_ptr<Image> label_image = this->label->get_image();

		Image::normal_blend(
			base_button_image->get_data(), 
			glm::vec2(0.0), 
			this->dimensions, 
			label_image->get_data(), 
			this->button_space_from_text * 0.5f, 
			label->get_dimensions());

		this->set_to_image(std::shared_ptr<Image>(new Image(
			base_button_image->get_data(), 
			this->dimensions.x,
			this->dimensions.y,
			base_button_image->get_bytes_per_pixel())));

	}
}