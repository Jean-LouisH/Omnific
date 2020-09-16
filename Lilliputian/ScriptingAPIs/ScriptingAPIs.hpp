#pragma once

#include "FileAPI.hpp"
#include "InputAPI.hpp"
#include "LogAPI.hpp"
#include "RenderAPI.hpp"
#include "SceneAPI.hpp"
#include "ThreadAPI.hpp"
#include "TimeAPI.hpp"
#include "WindowAPI.hpp"

namespace Lilliputian
{
	class ScriptingAPIs
	{
	private:
		FileAPI* fileAPI = NULL;
		InputAPI* inputAPI = NULL;
		LogAPI* logAPI = NULL;
		RenderAPI* renderAPI = NULL;
		SceneAPI* sceneAPI = NULL;
		ThreadAPI* threadAPI = NULL;
		TimeAPI* timeAPI = NULL;
		WindowAPI* windowAPI = NULL;
	public:
		ScriptingAPIs();

		FileAPI& file() const;
		InputAPI& input() const;
		LogAPI& log() const;
		RenderAPI& render() const;
		SceneAPI& scene() const;
		ThreadAPI& thread() const;
		TimeAPI& time() const;
		WindowAPI& window() const;
	};
}

