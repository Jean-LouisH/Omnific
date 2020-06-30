#include "ScriptingAPI.hpp"

Lilliputian::ScriptingAPI::ScriptingAPI(GameAPI* gameAPI, InputAPI* inputAPI)
{
	this->game = gameAPI;
	this->input = inputAPI;
}