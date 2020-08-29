#pragma once

#include <stdint.h>
#include <string>

namespace Lilliputian
{
	class Colour
	{
	private:
		float red;
		float blue;
		float green;
		float alpha;
	public:
		Colour();
		Colour(std::string hex);
	};
}