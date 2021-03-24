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
#include "render_api.hpp"
#include "scene_api.hpp"
#include "time_api.hpp"
#include "window_api.hpp"

namespace Lilliputian
{
	class ScriptingAPIs
	{
	public:
		ScriptingAPIs();

		CommandLineAPI& commandLine() const;
		FileAPI& file() const;
		InputAPI& input() const;
		LogAPI& log() const;
		RenderAPI& render() const;
		SceneAPI& scene() const;
		TimeAPI& time() const;
		WindowAPI& window() const;
	private:
		CommandLineAPI* commandLineAPI = nullptr;
		FileAPI* fileAPI = nullptr;
		InputAPI* inputAPI = nullptr;
		LogAPI* logAPI = nullptr;
		RenderAPI* renderAPI = nullptr;
		SceneAPI* sceneAPI = nullptr;
		TimeAPI* timeAPI = nullptr;
		WindowAPI* windowAPI = nullptr;
	};
}

