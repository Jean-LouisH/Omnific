#include "ScriptingAPI.hpp"

Lilliputian::ScriptingAPI::ScriptingAPI(GameAPI* gameAPI, InputAPI* inputAPI)
{
	this->gameAPI = gameAPI;
	this->inputAPI = inputAPI;
}

Lilliputian::GameAPI& Lilliputian::ScriptingAPI::game() const
{
	return *this->gameAPI;
}

Lilliputian::InputAPI& Lilliputian::ScriptingAPI::input() const
{
	return *this->inputAPI;
}