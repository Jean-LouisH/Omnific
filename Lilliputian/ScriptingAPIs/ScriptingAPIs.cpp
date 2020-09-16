#include "ScriptingAPIs.hpp"

Lilliputian::ScriptingAPIs::ScriptingAPIs()
{
	this->fileAPI = new FileAPI();
	this->inputAPI = new InputAPI();
	this->logAPI = new LogAPI();
	this->renderAPI = new RenderAPI();
	this->sceneAPI = new SceneAPI();
	this->threadAPI = new ThreadAPI();
	this->timeAPI = new TimeAPI();
	this->windowAPI = new WindowAPI();
}

Lilliputian::FileAPI& Lilliputian::ScriptingAPIs::file() const
{
	return *this->fileAPI;
}

Lilliputian::InputAPI& Lilliputian::ScriptingAPIs::input() const
{
	return *this->inputAPI;
}

Lilliputian::LogAPI& Lilliputian::ScriptingAPIs::log() const
{
	return *this->logAPI;
}

Lilliputian::RenderAPI& Lilliputian::ScriptingAPIs::render() const
{
	return *this->renderAPI;
}

Lilliputian::SceneAPI& Lilliputian::ScriptingAPIs::scene() const
{
	return *this->sceneAPI;
}

Lilliputian::ThreadAPI& Lilliputian::ScriptingAPIs::thread() const
{
	return *this->threadAPI;
}

Lilliputian::TimeAPI& Lilliputian::ScriptingAPIs::time() const
{
	return *this->timeAPI;
}

Lilliputian::WindowAPI& Lilliputian::ScriptingAPIs::window() const
{
	return *this->windowAPI;
}