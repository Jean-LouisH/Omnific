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

#include <game/scripting/scripting_apis.hpp>
#include <game/scene/components/ui_text_label.hpp>
#include <pybind11/embed.h>

PYBIND11_EMBEDDED_MODULE(lilliputian, m) 
{
	/*API classes*/
	pybind11::class_<Lilliputian::CommandLineAPI>(m, "CommandLineAPI");

	pybind11::class_<Lilliputian::FileAPI>(m, "FileAPI");

	pybind11::class_<Lilliputian::InputAPI>(m, "InputAPI")
		.def("is_on_press", &Lilliputian::InputAPI::isOnPress)
		.def("is_on_release", &Lilliputian::InputAPI::isOnRelease);

	pybind11::class_<Lilliputian::LogAPI>(m, "LogAPI");

	pybind11::class_<Lilliputian::RenderAPI>(m, "RenderAPI");

	pybind11::class_<Lilliputian::SceneAPI>(m, "SceneAPI")
		.def("has_component", &Lilliputian::SceneAPI::hasComponent)
		.def("change_to_scene", &Lilliputian::SceneAPI::changeToScene)
        .def("get_ui_text_label", &Lilliputian::SceneAPI::getUITextLabel, pybind11::return_value_policy::reference);

	pybind11::class_<Lilliputian::TimeAPI>(m, "TimeAPI");

    pybind11::class_<Lilliputian::WindowAPI>(m, "WindowAPI")
        .def("toggle_windowed_fullscreen", &Lilliputian::WindowAPI::toggleWindowedFullscreen);

	/*Component classes*/
    pybind11::class_<Lilliputian::UITextLabel>(m, "UITextLabel")
        .def("set_text", &Lilliputian::UITextLabel::setText);

	/*API getters*/
	m.def("get_command_line_api", &Lilliputian::ScriptingAPIs::getCommandLineAPI);
	m.def("get_file_api", &Lilliputian::ScriptingAPIs::getFileAPI);
	m.def("get_input_api", &Lilliputian::ScriptingAPIs::getInputAPI);
	m.def("get_log_api", &Lilliputian::ScriptingAPIs::getLogAPI);
	m.def("get_render_api", &Lilliputian::ScriptingAPIs::getRenderAPI);
    m.def("get_scene_api", &Lilliputian::ScriptingAPIs::getSceneAPI);
	m.def("get_time_api", &Lilliputian::ScriptingAPIs::getTimeAPI);
	m.def("get_window_api", &Lilliputian::ScriptingAPIs::getWindowAPI);
}