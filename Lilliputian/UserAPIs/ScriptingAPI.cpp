#include "ScriptingAPI.hpp"

Lilliputian::ScriptingAPI::ScriptingAPI(
	FileAPI* fileAPI,
	InputAPI* inputAPI,
	LoggingAPI* loggingAPI,
	SceneAPI* sceneAPI,
	TimeAPI* timeAPI,
	WindowAPI* windowAPI
)
{
	this->fileAPI = fileAPI;
	this->inputAPI = inputAPI;
	this->loggingAPI = loggingAPI;
	this->sceneAPI = sceneAPI;
	this->timeAPI = timeAPI;
	this->windowAPI = windowAPI;
}

Lilliputian::FileAPI& Lilliputian::ScriptingAPI::file() const
{
	return *this->fileAPI;
}

Lilliputian::InputAPI& Lilliputian::ScriptingAPI::input() const
{
	return *this->inputAPI;
}

Lilliputian::LoggingAPI& Lilliputian::ScriptingAPI::logging() const
{
	return *this->loggingAPI;
}

Lilliputian::SceneAPI& Lilliputian::ScriptingAPI::scene() const
{
	return *this->sceneAPI;
}

Lilliputian::TimeAPI& Lilliputian::ScriptingAPI::time() const
{
	return *this->timeAPI;
}

Lilliputian::WindowAPI& Lilliputian::ScriptingAPI::window() const
{
	return *this->windowAPI;
}