#pragma once

#include <stdint.h>
#include <string>

namespace Lilliputian
{
	class Colour
	{
	public:
		Colour();
		Colour(std::string hex);
		Colour(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);
	private:
		float red;
		float green;
		float blue;
		float alpha;
	};
}