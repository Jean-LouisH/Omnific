// MIT License
// 
// Copyright (c) 2020 Jean-Louis Haywood
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

#include "command_line_api.hpp"
#include "file_api.hpp"
#include "input_api.hpp"
#include "log_api.hpp"
#include "scene_api.hpp"
#include "time_api.hpp"
#include "window_api.hpp"

#include "game/scene/scene.hpp"
#include "utilities/aliases.hpp"
#include "os/os.hpp"

namespace Lilliputian
{
	class ScriptingAPIs
	{
	public:
		static void initialize();
		static void bindScene(Scene* scene);
		static void bindEntity(SceneTreeID sceneTreeID, EntityID entityID);

		static CommandLineAPI& getCommandLineAPI();
		static FileAPI& getFileAPI();
		static InputAPI& getInputAPI();
		static LogAPI& getLogAPI();
		static SceneAPI& getSceneAPI();
		static TimeAPI& getTimeAPI();
		static WindowAPI& getWindowAPI();
	private:
		static ScriptingAPIs* instance;

		CommandLineAPI* commandLineAPI = nullptr;
		FileAPI* fileAPI = nullptr;
		InputAPI* inputAPI = nullptr;
		LogAPI* logAPI = nullptr;
		SceneAPI* sceneAPI = nullptr;
		TimeAPI* timeAPI = nullptr;
		WindowAPI* windowAPI = nullptr;

		static ScriptingAPIs* getInstance();
	};
}

