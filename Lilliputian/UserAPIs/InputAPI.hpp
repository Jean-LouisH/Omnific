#pragma once

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
		
		bool isKeyPressed(unsigned int keyCode);
	};
}