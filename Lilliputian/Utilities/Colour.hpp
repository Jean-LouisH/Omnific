#pragma once

#include <stdint.h>
#include <string>

namespace Lilliputian
{
	class Colour
	{
	private:
		double red;
		double blue;
		double green;
		double alpha;
	public:
		Colour();
		Colour(std::string hex);
	};
}