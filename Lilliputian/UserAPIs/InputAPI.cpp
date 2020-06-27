#include "InputAPI.hpp"

Lilliputian::InputAPI::InputAPI()
{
	this->analogueStickSensitivity = 4096;
}

bool Lilliputian::InputAPI::isKeyPressed(unsigned int keyCode)
{
	return false;
}