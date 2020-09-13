#pragma once

#include "FileAPI.hpp"
#include "InputAPI.hpp"
#include "LoggingAPI.hpp"
#include "SceneAPI.hpp"
#include "TimeAPI.hpp"
#include "WindowAPI.hpp"

namespace Lilliputian
{
	class ScriptingAPI
	{
	private:
		FileAPI* fileAPI;
		InputAPI* inputAPI;
		LoggingAPI* loggingAPI;
		SceneAPI* sceneAPI;
		TimeAPI* timeAPI;
		WindowAPI* windowAPI;
	public:
		ScriptingAPI(
			FileAPI* fileAPI,
			InputAPI* inputAPI,
			LoggingAPI* loggingAPI,
			SceneAPI* sceneAPI,
			TimeAPI* timeAPI,
			WindowAPI* windowAPI
		);

		FileAPI& file() const;
		InputAPI& input() const;
		LoggingAPI& logging() const;
		SceneAPI& scene() const;
		TimeAPI& time() const;
		WindowAPI& window() const;
	};
}

