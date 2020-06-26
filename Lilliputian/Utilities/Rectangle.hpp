#pragma once

#include <stdint.h>

namespace Lilliputian
{
	class Rectangle
	{
	private:
	public:
		unsigned int width;
		unsigned int height;

		Rectangle():
			width(0),
			height(0)
		{}
	};
}
