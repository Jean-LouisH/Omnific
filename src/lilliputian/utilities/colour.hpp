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
	private:
		float red;
		float blue;
		float green;
		float alpha;
	};
}