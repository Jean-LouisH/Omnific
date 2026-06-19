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

#include "label.hpp"
#include <foundations/singletons/platform/platform.hpp>
#include <foundations/singletons/resource_manager.hpp>

void Omnific::Label::deserialize(YAML::Node yaml_node)
{
	GUIElement::deserialize(yaml_node);

	for (YAML::const_iterator it3 = yaml_node.begin(); it3 != yaml_node.end(); ++it3)
	{
		if (it3->first.as<std::string>() == "text")
		{
			this->text = it3->second.as<std::string>();
		}
		else if (it3->first.as<std::string>() == "font")
		{
			std::shared_ptr<Omnific::Font> font = ResourceManager::load_resource<Font>(it3->second[0].as<std::string>());
			*font = Font(font->get_name(), it3->second[1].as<int>());
			this->font = font;
		}
		else if (it3->first.as<std::string>() == "wrap_length")
		{
			this->wrap_length = it3->second.as<int>();
		}
		else if (it3->first.as<std::string>() == "text_colour")
		{
			this->text_colour = std::shared_ptr<Colour>(new Colour(it3->second.as<std::string>()));
		}
	}

	this->update_image();
}

void Omnific::Label::set_text(std::string text)
{
	if (text != this->text)
	{
		this->text = text;
		this->update_image();
	}
}

void Omnific::Label::set_font(std::shared_ptr<Omnific::Font> font)
{
	this->font = font;
	this->update_image();
}

void Omnific::Label::set_text_colour(std::shared_ptr<Colour> colour)
{
	this->text_colour = colour;
	this->update_image();
}

void Omnific::Label::set_horizontal_alignment(Font::HorizontalAlignment alignment)
{
	this->horizontal_alignment = alignment;
	if (this->font != nullptr)
	{
		this->font = std::shared_ptr<Font>(new Font(this->horizontal_alignment));
	}
	else
	{
		uint16_t font_size = this->font->get_font_size();
		this->font = std::shared_ptr<Font>(new Font(this->horizontal_alignment, font_size));
	}
	this->update_image();
}

void Omnific::Label::update_image()
{
	if (!this->is_hidden() && this->font != nullptr && this->text_colour != nullptr)
	{
		if (this->font->get_sdl_ttf_font() != nullptr)
		{
			this->set_to_image(std::shared_ptr<Image>(new Image(this->text, this->font, this->text_colour, this->wrap_length)));
		}
	}
}