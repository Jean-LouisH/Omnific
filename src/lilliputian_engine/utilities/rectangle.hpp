#pragma once

#include <stdint.h>

namespace Lilliputian
{
	class Rectangle
	{
	public:
		uint32_t width;
		uint32_t height;

		Rectangle() :
			width(0),
			height(0)
		{}
	private:
	};
}
