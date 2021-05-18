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

#include <scripting_apis/scripting_apis.hpp>
#include <game/scene/components/ui_text_label.hpp>
#include <pybind11/embed.h>

PYBIND11_EMBEDDED_MODULE(lilliputian, m) 
{
    pybind11::class_<Lilliputian::ScriptingAPIs>(m, "ScriptingAPIs")
        .def(pybind11::init<>())
        .def("get_test_string", &Lilliputian::ScriptingAPIs::getTestString)
        .def("scene", &Lilliputian::ScriptingAPIs::scene);

    pybind11::class_<Lilliputian::SceneAPI>(m, "SceneAPI")
        .def(pybind11::init<>())
        .def("get_ui_text_label", &Lilliputian::SceneAPI::getUITextLabel);

    pybind11::class_<Lilliputian::WindowAPI>(m, "WindowAPI")
        .def(pybind11::init<>())
        .def("toggle_windowed_fullscreen", &Lilliputian::WindowAPI::toggleWindowedFullscreen);

    pybind11::class_<Lilliputian::UITextLabel>(m, "UITextLabel")
        .def(pybind11::init<>())
        .def("get_text", &Lilliputian::UITextLabel::getText)
        .def("set_text", &Lilliputian::UITextLabel::setText);
}