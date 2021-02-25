#pragma once

#include "boot_configuration.hpp"
#include "utilities/string.hpp"

namespace Lilliputian
{
	class BootLoader
	{
	public:
		BootConfiguration* loadFromFile(String filepath);
	};
}