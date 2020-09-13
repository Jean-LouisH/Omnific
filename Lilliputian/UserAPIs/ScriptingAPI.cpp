#include "ScriptingAPI.hpp"

Lilliputian::ScriptingAPI::ScriptingAPI(
	EngineAPI* engineAPI,
	GameAPI* gameAPI,
	InputAPI* inputAPI,
	SceneAPI* sceneAPI
)
{
	this->engineAPI = engineAPI;
	this->gameAPI = gameAPI;
	this->inputAPI = inputAPI;
	this->sceneAPI = sceneAPI;
}

Lilliputian::EngineAPI& Lilliputian::ScriptingAPI::engine() const
{
	return *this->engineAPI;
}

Lilliputian::GameAPI& Lilliputian::ScriptingAPI::game() const
{
	return *this->gameAPI;
}

Lilliputian::InputAPI& Lilliputian::ScriptingAPI::input() const
{
	return *this->inputAPI;
}

Lilliputian::SceneAPI& Lilliputian::ScriptingAPI::scene() const
{
	return *this->sceneAPI;
}