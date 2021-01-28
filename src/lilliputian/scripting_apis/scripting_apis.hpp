#pragma once

#include "file_api.hpp"
#include "input_api.hpp"
#include "log_api.hpp"
#include "render_api.hpp"
#include "scene_api.hpp"
#include "thread_api.hpp"
#include "time_api.hpp"
#include "window_api.hpp"

namespace Lilliputian
{
	class ScriptingAPIs
	{
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
	private:
		FileAPI* fileAPI = NULL;
		InputAPI* inputAPI = NULL;
		LogAPI* logAPI = NULL;
		RenderAPI* renderAPI = NULL;
		SceneAPI* sceneAPI = NULL;
		ThreadAPI* threadAPI = NULL;
		TimeAPI* timeAPI = NULL;
		WindowAPI* windowAPI = NULL;
	};
}

