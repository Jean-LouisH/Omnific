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

#include "core/component.hpp"
#include <core/assets/colour.hpp>

namespace Omnia
{
	enum class LightMode
	{
		DIRECTIONAL,
		OMNIDIRECTIONAL,
		SPOT
	};

	class OMNIA_ENGINE_API Light : public Component
	{
	public:
		Light()
		{
			this->type = TYPE_STRING;
			this->colour = std::shared_ptr<Colour>(new Colour(1.0, 1.0, 1.0, 1.0));
			this->shadowColour = std::shared_ptr<Colour>(new Colour(0.0, 0.0, 0.0, 1.0));
		};
		static constexpr const char* TYPE_STRING = "Light";

		LightMode mode = LightMode::DIRECTIONAL;
		std::shared_ptr<Colour> colour;
		std::shared_ptr<Colour> shadowColour;
		float intensity = 1.0;
		float attenuation = 0.0;
		float range = 0.0;
		float angle = 45.0;
		bool isShadowEnabled = false;

		virtual Registerable* instance() override
		{
			Light* clone = new Light(*this);
			clone->id = UIDGenerator::getNewUID();
			return clone;
		}
		virtual void deserialize(YAML::Node yamlNode);
	private:
	};
}