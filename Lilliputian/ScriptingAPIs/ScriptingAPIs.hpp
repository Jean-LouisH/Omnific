#pragma once

#include "FileAPI.hpp"
#include "InputAPI.hpp"
#include "LogAPI.hpp"
#include "SceneAPI.hpp"
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
		SceneAPI* sceneAPI = NULL;
		TimeAPI* timeAPI = NULL;
		WindowAPI* windowAPI = NULL;
	public:
		ScriptingAPIs();

		FileAPI& file() const;
		InputAPI& input() const;
		LogAPI& log() const;
		SceneAPI& scene() const;
		TimeAPI& time() const;
		WindowAPI& window() const;
	};
}

