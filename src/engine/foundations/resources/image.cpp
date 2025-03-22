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

#include "image.hpp"
#include <vector>
#include <cstring>
#include <algorithm>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

Omnific::Image::Image(std::string text, std::shared_ptr<Font> font, std::shared_ptr<Colour> colour, uint16_t wrap_length)
{
	this->type = TYPE_STRING;
	this->alignment = Alignment::TOP_LEFT;

	if (text == "")
		text = " ";

	SDL_Color sdl_color = { colour->get_red(), colour->get_green(), colour->get_blue(), colour->get_alpha() };
	std::shared_ptr<SDL_Surface> sdl_surface(SDL_ConvertSurfaceFormat(TTF_RenderUTF8_Blended_Wrapped(font->get_sdl_ttf_font(), text.c_str(), sdl_color, wrap_length), SDL_PIXELFORMAT_RGBA32, 0), SDL_FreeSurface);

	/*	SDL vertical pixel flip solution by 
		vvanpelt on StackOverflow: https://stackoverflow.com/a/65817254 
	*/
	SDL_Surface* flipped_sdlsurface = sdl_surface.get();
	SDL_LockSurface(flipped_sdlsurface);

	int pitch = flipped_sdlsurface->pitch; // row size
	char* temp = new char[pitch]; // intermediate buffer
	char* pixels = (char*)flipped_sdlsurface->pixels;

	for (int i = 0; i < flipped_sdlsurface->h / 2; ++i) {
		// get pointers to the two rows to swap
		char* row1 = pixels + i * pitch;
		char* row2 = pixels + (flipped_sdlsurface->h - i - 1) * pitch;

		// swap rows
		memcpy(temp, row1, pitch);
		memcpy(row1, row2, pitch);
		memcpy(row2, temp, pitch);
	}

	delete[] temp;

	SDL_UnlockSurface(flipped_sdlsurface);

	/////////

	if (sdl_surface != nullptr)
		this->set_to_parameters(sdl_surface->format->BytesPerPixel, sdl_surface->w, sdl_surface->h, (uint8_t*)sdl_surface->pixels);
}

Omnific::Image::Image(std::shared_ptr<Colour> colour)
{
	int width = 256;
	int height = 256;
	this->alignment = Alignment::CENTRE;
	this->set_to_colour(colour, width, height);
}

Omnific::Image::Image(std::shared_ptr<Colour> colour, int width, int height)
{
	this->alignment = Alignment::CENTRE;
	this->set_to_colour(colour, width, height);
}

Omnific::Image::Image(uint8_t* data, int width, int height, int colour_channels)
{
	this->type = TYPE_STRING;
	this->alignment = Alignment::CENTRE;
	this->set_to_parameters(colour_channels, width, height, data);
}

Omnific::Image::Image(std::string filepath)
{
	stbi_set_flip_vertically_on_load(0);
	this->type = TYPE_STRING;
	this->alignment = Alignment::CENTRE;
	this->set_name(filepath);

	std::string delimitter = "Image::";
	size_t delimitter_position = filepath.find(delimitter);
	size_t position = delimitter_position + delimitter.length();

	if (delimitter_position < filepath.length())
	{
		std::string token = filepath.substr(position, filepath.length());

		if (token == "default")
			this->set_to_default();
		if (token[0] == '#')
			this->set_to_colour(std::shared_ptr<Colour>(new Colour(token.substr(1, token.length()))), 256, 256);
	}
	else
	{
		this->data = std::shared_ptr<uint8_t>(stbi_load(filepath.c_str(), &this->width, &this->height, &this->colour_channel_count, 0), stbi_image_free);
	}
}

uint8_t* Omnific::Image::get_data()
{
	if (this->data != nullptr)
		return this->data.get();
	else
		return nullptr;
}

uint32_t Omnific::Image::get_width()
{
	if (this->data != nullptr)
		return this->width;
	else
		return 0;
}

uint32_t Omnific::Image::get_height()
{
	if (this->data != nullptr)
		return this->height;
	else
		return 0;
}

uint32_t Omnific::Image::get_depth()
{
	return this->get_bytes_per_pixel() * 8;
}

uint32_t Omnific::Image::get_pitch()
{
	return this->get_bytes_per_pixel() * this->get_width();
}

float Omnific::Image::get_aspect_ratio()
{
	return (float)this->get_width() / (float)this->get_height();
}

glm::vec2 Omnific::Image::get_dimensions()
{
	return glm::vec2(this->get_width(), this->get_height());
}

uint8_t Omnific::Image::get_bytes_per_pixel()
{
	return this->colour_channel_count;
}

Omnific::Image::Alignment Omnific::Image::get_alignment()
{
	return this->alignment;
}

size_t Omnific::Image::get_size()
{
	return this->width * this->height * this->colour_channel_count;
}

void Omnific::Image::normal_blend(
		uint8_t* lower_image_data,
		glm::vec2 lower_image_position,
		glm::vec2 lower_image_dimensions,
		uint8_t* upper_image_data,
		glm::vec2 upper_image_position,
		glm::vec2 upper_image_dimensions)
{
	const uint8_t colour_channels = 4;

    // Calculate starting points and limits for iteration to stay within bounds
    int starting_upper_y = std::max(0.0f, lower_image_position.y - upper_image_position.y);
    int starting_upper_x = std::max(0.0f, lower_image_position.x - upper_image_position.x);
    int upper_y_max = std::min(upper_image_dimensions.y, lower_image_dimensions.y - upper_image_position.y);
    int upper_x_max = std::min(upper_image_dimensions.x, lower_image_dimensions.x - upper_image_position.x);

    for (int upper_y = starting_upper_y; upper_y < upper_y_max; upper_y++) 
	{
        for (int upper_x = starting_upper_x; upper_x < upper_x_max; upper_x++) 
		{
            int upper_index = (upper_y * upper_image_dimensions.x + upper_x) * colour_channels;
            int lower_index = ((upper_y + upper_image_position.y - lower_image_position.y) * lower_image_dimensions.x +
                               (upper_x + upper_image_position.x - lower_image_position.x)) * colour_channels;

            // Get the alpha values of the upper and lower images
            float upper_alpha = upper_image_data[upper_index + 3] / 255.0f;
            float lower_alpha = lower_image_data[lower_index + 3] / 255.0f;

            // Blend each RGB channel
            for (int colour_channel = 0; colour_channel < colour_channels - 1; colour_channel++) 
			{
                lower_image_data[lower_index + colour_channel] = 
                    static_cast<uint8_t>(upper_image_data[upper_index + colour_channel] * upper_alpha +
                                         lower_image_data[lower_index + colour_channel] * lower_alpha * (1.0f - upper_alpha));
            }

            // Calculate the new alpha value for the blended pixel
            lower_image_data[lower_index + 3] = static_cast<uint8_t>((upper_alpha + lower_alpha * (1.0f - upper_alpha)) * 255);
        }
    }
}

void Omnific::Image::colourize_pixel(uint32_t fill_colour, int x, int y)
{
	for (int colour_channel = 0; colour_channel < this->colour_channel_count; colour_channel++)
	{
		const int colour_channel_byte_length = 8;
		int image_index = (y * this->width * this->colour_channel_count) + (x * this->colour_channel_count) + colour_channel;
		int colour_byte_length = this->colour_channel_count * colour_channel_byte_length;
		int colour_channel_byte_offset = colour_channel * colour_channel_byte_length;
		int mask_byte_shift = colour_byte_length - colour_channel_byte_length - colour_channel_byte_offset;
		uint8_t colour_channel_value = (uint8_t)((fill_colour & (0x000000FF << mask_byte_shift)) >> mask_byte_shift);
		this->data.get()[image_index] = colour_channel_value;
	}
}

void Omnific::Image::set_to_default()
{
	const uint32_t lighter_grey = 0x333333;
	const uint32_t darker_grey = 0x2d2d2d;
	const uint16_t size = 256;
	const uint8_t divisions = 4;
	this->height = size;
	this->width = size;
	this->colour_channel_count = 3;
	size_t data_size = this->width * this->height * this->colour_channel_count;

	this->data = std::shared_ptr<uint8_t>(new uint8_t[data_size]);
	uint32_t fill_colour = 0;
	bool darker = false;

	for (int y = 0; y < this->height; y++)
	{
		for (int x = 0; x < this->width; x++)
		{
			if (darker)
				fill_colour = darker_grey;
			else
				fill_colour = lighter_grey;

			this->colourize_pixel(fill_colour, x, y);

			if ((x % (size / divisions)) == 0)
				darker = !darker;
		}

		if ((y % (size / divisions)) == 0)
			darker = !darker;
	}
}

void Omnific::Image::set_to_colour(std::shared_ptr<Colour> colour, int width, int height)
{
	this->type = TYPE_STRING;
	this->width = width;
	this->height = height;
	this->colour_channel_count = 4;
	size_t data_size = this->width * this->height * this->colour_channel_count;
	const uint32_t fill_colour = colour->get_rgba();
	this->data = std::shared_ptr<uint8_t>(new uint8_t[data_size]);

	for (int y = 0; y < this->height; y++)
		for (int x = 0; x < this->width; x++)
			this->colourize_pixel(fill_colour, x, y);
}

void Omnific::Image::set_to_parameters(int colour_channels, int width, int height, uint8_t* data)
{
	this->height = height;
	this->width = width;
	this->colour_channel_count = colour_channels;
	size_t data_size = this->width * this->height * this->colour_channel_count;
	const int colour_channel_byte_length = 8;
	this->data = std::shared_ptr<uint8_t>(new uint8_t[data_size]);

	for (int y = 0; y < this->height; y++)
	{
		for (int x = 0; x < this->width; x++)
		{
			uint32_t fill_colour = 0;
			for (int colour_channel = 0; colour_channel < colour_channels; colour_channel++)
				fill_colour |= data[(y * this->width * this->colour_channel_count) + (x * this->colour_channel_count) + colour_channel] <<
								(colour_channel_byte_length * (colour_channels - 1 - colour_channel));

			this->colourize_pixel(fill_colour, x, y);
		}
	}
}