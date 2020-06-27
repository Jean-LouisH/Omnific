#include "InputAPI.hpp"

Lilliputian::InputAPI::InputAPI()
{
	this->analogueStickSensitivity = 4096;
}

void Lilliputian::InputAPI::insertActionInput(const char* actionName, uint8_t inputCode)
{
	String actionString = actionName;

	if (this->actionInputs.count(actionName))
	{
		this->actionInputs.at(actionName).push_back(inputCode);
	}
	else
	{
		Vector<uint8_t> inputCodes;
		inputCodes.push_back(inputCode);
		this->actionInputs.emplace(actionString, inputCodes);
	}
}

void Lilliputian::InputAPI::replaceActionInput(const char* actionName, uint8_t inputCode)
{
	String actionString = actionName;

	if (this->actionInputs.count(actionString))
	{
		this->actionInputs.at(actionString).clear();
		this->actionInputs.at(actionString).push_back(inputCode);
	}
	else
	{
		Vector<uint8_t> inputCodes;
		inputCodes.push_back(inputCode);
		this->actionInputs.emplace(actionString, inputCodes);
	}

}

void Lilliputian::InputAPI::removeActionInput(const char* actionName, uint8_t inputCode)
{

}

void Lilliputian::InputAPI::removeAllActionInputs(const char* actionName)
{
	String actionString = actionName;
	if (this->actionInputs.count(actionString))
		this->actionInputs.at(actionString).clear();
}

Lilliputian::Map<Lilliputian::String, Lilliputian::Vector<uint8_t>> Lilliputian::InputAPI::getActionInputs()
{
	return this->actionInputs;
}

bool Lilliputian::InputAPI::isKeyPressed(unsigned int keyCode)
{
	return false;
}