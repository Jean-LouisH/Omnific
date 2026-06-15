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

#include "foundations/aliases.hpp"
#include <vector>
#include "foundations/constants.hpp"
#include <stdint.h>
#include "scene/components/component.hpp"
#include <unordered_map>

namespace Omnific
{
	class OMNIFIC_ENGINE_API Animation
	{
	public:
		enum class InterpolationType 
		{
			LINEAR,
			STEP,
			CUBIC_SPLINE
		};

		std::string name;
		float duration = 0.0f;  
		float playback_speed_percentage = 1.0;
		float progress = 0.0;
		uint8_t allowable_repeats = 0;
		uint8_t repeat_count = 0;
		bool is_playing = false; 
	private:
	};

	class OMNIFIC_ENGINE_API PropertyAnimation : public Animation
	{
	public:
		struct Sampler 
		{
			InterpolationType interpolation;
			std::vector<float> input_timestamps;
			std::vector<float> output_values; 
		};

		std::vector<PropertyAnimation::Sampler> samplers;
		float* target_property;
		size_t sampler_index;
		
	private:
	};

	class OMNIFIC_ENGINE_API SkeletalAnimation : public Animation
	{
	public:
		enum class Path 
		{
			TRANSLATION,
			ROTATION,
			SCALE
		};

		struct Sampler 
		{
			InterpolationType interpolation;
			std::vector<float> input_timestamps;
			std::vector<glm::vec4> output_values; 
		};

		struct Channel 
		{
			uint32_t target_entity_id; 
			Path path;
			size_t sampler_index;
		};
  
		std::vector<SkeletalAnimation::Sampler> samplers;
		std::vector<SkeletalAnimation::Channel> channels;
	};

	class OMNIFIC_ENGINE_API Animator : public Component
	{
	public:
		Animator()
		{
			this->type = TYPE_STRING;
		};
		static constexpr const char* TYPE_STRING = "Animator";

		virtual Registerable* instance() override
		{
			Animator* clone = new Animator(*this);
			clone->id = UIDGenerator::get_new_uid();
			return clone;
		}

		std::unordered_map<std::string, std::shared_ptr<SkeletalAnimation>> skeletal_animations;
		std::unordered_map<std::string, std::shared_ptr<PropertyAnimation>> property_animations;

		virtual void deserialize(YAML::Node yaml_node);
	private:
	};
}