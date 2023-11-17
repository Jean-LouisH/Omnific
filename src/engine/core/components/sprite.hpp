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

#pragma once

#include "core/utilities/aliases.hpp"
#include <vector>
#include <unordered_map>
#include "core/utilities/constants.hpp"
#include <string>
#include "core/assets/image.hpp"
#include "renderable_component.hpp"
#include <glm/glm.hpp>
#include <memory>

namespace Omnia
{
	class OMNIA_ENGINE_API Sprite : public RenderableComponent
	{
	public:
		Sprite()
		{
			this->type = TYPE_STRING;
		};
		static constexpr const char* TYPE_STRING = "Sprite";
		std::string current_frame_sequence_name = "";

		bool is_xbillboarded = false;
		bool is_ybillboarded = false;

		virtual Registerable* instance() override
		{
			Sprite* clone = new Sprite(*this);
			clone->id = UIDGenerator::get_new_uid();
			return clone;
		}
		virtual void deserialize(YAML::Node yaml_node);
		void add_image(std::shared_ptr<Image> image);
		void add_empty_frame_sequence(std::string frame_sequence_name);
		void add_frame_sequence(std::string frame_sequence_name, std::vector<std::shared_ptr<Image>> frame_sequence);
		void add_image_to_frame_sequence(std::string frame_sequence_name, std::shared_ptr<Image> frame);
		void clear_frame_sequences();

		void set_animation_speed(float value_fps);
		float get_animation_speed();

		void update(float delta_s);

		void play(std::string frame_sequence_name);
		void play();
		void pause();
		void stop();

		void set_backwards();
		void set_forwards();

		void flip_vertically();
		void flip_horizontally();

		std::shared_ptr<Image> get_current_frame();
		std::vector<std::string> get_frame_sequence_names();
		std::vector<std::shared_ptr<Image>> get_frame_sequence_by_name(std::string frame_sequence_name);
		std::vector<std::shared_ptr<Image>> get_current_frame_sequence();
	private:
		std::unordered_map<std::string, std::vector<std::shared_ptr<Image>>> frame_sequences;
		float animation_speed_in_fps = 12.0;
		float frame_time = 0.0;
		uint16_t current_frame_index = 0;
		glm::vec3 billboard_offset_rotation = {0.0, 0.0, 0.0};
		bool is_playing = false;
		bool is_backwards = false;
		bool is_flipped_vertically = false;
		bool is_flipped_horizontally = false;
	};
}