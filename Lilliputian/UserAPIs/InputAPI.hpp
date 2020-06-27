#pragma once

#include <stdint.h>

namespace Lilliputian
{
	class InputAPI
	{
	private:
	public:

		enum keyCodes
		{
			KEY_CODES_ESCAPE,
		};

		InputAPI();
		uint16_t analogueStickSensitivity;
		bool isKeyPressed(unsigned int keyCode);
	};
}