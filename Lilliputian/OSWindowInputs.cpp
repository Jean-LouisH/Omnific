#include "OSWindowInputs.hpp"

void Lilliputian::OSWindowInputs::clear()
{
	this->controllerAxisEvents.clear();
	this->controllerButtonEvents.clear();
	this->keyboardEvents.clear();
	this->mouseButtonEvents.clear();
	this->mouseMotionEvents.clear();
	this->mouseWheelEvents.clear();
}