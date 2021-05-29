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

	pybind11::class_<Lilliputian::SceneAPI>(m, "SceneAPI")
		.def("has_component", &Lilliputian::SceneAPI::hasComponent)
		.def("change_to_scene", &Lilliputian::SceneAPI::changeToScene)
		.def("get_this_entity_2d", &Lilliputian::SceneAPI::getThisEntity2D, pybind11::return_value_policy::reference)
		.def("get_this_scene_tree_2d", &Lilliputian::SceneAPI::getThisSceneTree2D, pybind11::return_value_policy::reference)
		.def("get_scene", &Lilliputian::SceneAPI::getScene, pybind11::return_value_policy::reference)
        .def("get_ui_text_label", &Lilliputian::SceneAPI::getUITextLabel, pybind11::return_value_policy::reference);

	pybind11::class_<Lilliputian::TimeAPI>(m, "TimeAPI");

    pybind11::class_<Lilliputian::WindowAPI>(m, "WindowAPI")
        .def("toggle_windowed_fullscreen", &Lilliputian::WindowAPI::toggleWindowedFullscreen);

	/*Scene classes*/

	pybind11::class_<Lilliputian::Entity2D>(m, "Entity2D")
		.def_readonly("ID", &Lilliputian::Entity2D::ID)
		.def_readwrite("name", &Lilliputian::Entity2D::name)
		.def_readwrite("parentID", &Lilliputian::Entity2D::parentID)
		.def_readwrite("childIDs", &Lilliputian::Entity2D::childIDs)
		.def_readwrite("tags", &Lilliputian::Entity2D::tags);

	pybind11::class_<Lilliputian::SceneTree2D>(m, "SceneTree2D");

	pybind11::class_<Lilliputian::Scene>(m, "Scene");

	/*Component classes*/
    pybind11::class_<Lilliputian::UITextLabel>(m, "UITextLabel")
        .def("set_text", &Lilliputian::UITextLabel::setText);

	pybind11::enum_<Lilliputian::ComponentVariant::Type>(m, "Type")
		.value("NONE", Lilliputian::ComponentVariant::Type::NONE)
		.value("AI_BEHAVIOUR_TREE", Lilliputian::ComponentVariant::Type::AI_BEHAVIOUR_TREE)
		.value("AI_SIGHT_PERCEPTION", Lilliputian::ComponentVariant::Type::AI_SIGHT_PERCEPTION)
		.value("AI_SOUND_PERCEPTION", Lilliputian::ComponentVariant::Type::AI_SOUND_PERCEPTION)
		.value("ANIMATED_SPRITE", Lilliputian::ComponentVariant::Type::ANIMATED_SPRITE)
		.value("AUDIO_LISTENER_2D", Lilliputian::ComponentVariant::Type::AUDIO_LISTENER_2D)
		.value("AUDIO_STREAM_SOURCE_2D", Lilliputian::ComponentVariant::Type::AUDIO_STREAM_SOURCE_2D)
		.value("CAMERA_2D", Lilliputian::ComponentVariant::Type::CAMERA_2D)
		.value("CIRCLE_COLLIDER_2D", Lilliputian::ComponentVariant::Type::CIRCLE_COLLIDER_2D)
		.value("CONSTANT_DIRECTIONAL_FORCE_2D", Lilliputian::ComponentVariant::Type::CONSTANT_DIRECTIONAL_FORCE_2D)
		.value("CONSTANT_POINT_FORCE_2D", Lilliputian::ComponentVariant::Type::CONSTANT_POINT_FORCE_2D)
		.value("COUNTDOWN_TIMER", Lilliputian::ComponentVariant::Type::COUNTDOWN_TIMER)
		.value("FIXED_TRANSFORM_2D", Lilliputian::ComponentVariant::Type::FIXED_TRANSFORM_2D)
		.value("NAVIGATION_MESH_2D", Lilliputian::ComponentVariant::Type::NAVIGATION_MESH_2D)
		.value("NAVIGATION_MESH_AGENT_2D", Lilliputian::ComponentVariant::Type::NAVIGATION_MESH_AGENT_2D)
		.value("NAVIGATION_MESH_BOX_OBSTACLE_2D", Lilliputian::ComponentVariant::Type::NAVIGATION_MESH_BOX_OBSTACLE_2D)
		.value("NAVIGATION_PATH_2D", Lilliputian::ComponentVariant::Type::NAVIGATION_PATH_2D)
		.value("PHYSICS_CONSTRAINT_2D", Lilliputian::ComponentVariant::Type::PHYSICS_CONSTRAINT_2D)
		.value("PHYSICS_THRUSTER_2D", Lilliputian::ComponentVariant::Type::PHYSICS_THRUSTER_2D)
		.value("PROPERTY_ANIMATION", Lilliputian::ComponentVariant::Type::PROPERTY_ANIMATION)
		.value("RECTANGULAR_COLIIDER_2D", Lilliputian::ComponentVariant::Type::RECTANGULAR_COLIIDER_2D)
		.value("RECTANGULAR_MESH_2D", Lilliputian::ComponentVariant::Type::RECTANGULAR_MESH_2D)
		.value("RECTANGULAR_TRIGGER_AREA_2D", Lilliputian::ComponentVariant::Type::RECTANGULAR_TRIGGER_AREA_2D)
		.value("REGULAR_POLYGONAL_MESH_2D", Lilliputian::ComponentVariant::Type::REGULAR_POLYGONAL_MESH_2D)
		.value("RIGID_BODY_2D", Lilliputian::ComponentVariant::Type::RIGID_BODY_2D)
		.value("SPRITE", Lilliputian::ComponentVariant::Type::SPRITE)
		.value("STATIC_FLUID_2D", Lilliputian::ComponentVariant::Type::STATIC_FLUID_2D)
		.value("TRANSFORM_2D", Lilliputian::ComponentVariant::Type::TRANSFORM_2D)
		.value("UI_BUTTON", Lilliputian::ComponentVariant::Type::UI_BUTTON)
		.value("UI_RECTANGLE", Lilliputian::ComponentVariant::Type::UI_RECTANGLE)
		.value("UI_GRAPH_EDIT", Lilliputian::ComponentVariant::Type::UI_GRAPH_EDIT)
		.value("UI_GRAPH_NODE", Lilliputian::ComponentVariant::Type::UI_GRAPH_NODE)
		.value("UI_SCROLLBAR", Lilliputian::ComponentVariant::Type::UI_SCROLLBAR)
		.value("UI_SEPARATOR", Lilliputian::ComponentVariant::Type::UI_SEPARATOR)
		.value("UI_SLIDER", Lilliputian::ComponentVariant::Type::UI_SLIDER)
		.value("UI_HOVER_CARD", Lilliputian::ComponentVariant::Type::UI_HOVER_CARD)
		.value("UI_ITEM_LIST", Lilliputian::ComponentVariant::Type::UI_ITEM_LIST)
		.value("UI_PANEL", Lilliputian::ComponentVariant::Type::UI_PANEL)
		.value("UI_PROGRESS_BAR", Lilliputian::ComponentVariant::Type::UI_PROGRESS_BAR)
		.value("UI_SPIN_BOX", Lilliputian::ComponentVariant::Type::UI_SPIN_BOX)
		.value("UI_TAB", Lilliputian::ComponentVariant::Type::UI_TAB)
		.value("UI_TEXT_EDIT", Lilliputian::ComponentVariant::Type::UI_TEXT_EDIT)
		.value("UI_TEXT_LABEL", Lilliputian::ComponentVariant::Type::UI_TEXT_LABEL)
		.value("UI_TREE", Lilliputian::ComponentVariant::Type::UI_TREE);

	/*API getters*/
	m.def("get_command_line_api", &Lilliputian::ScriptingAPIs::getCommandLineAPI);
	m.def("get_file_api", &Lilliputian::ScriptingAPIs::getFileAPI);
	m.def("get_input_api", &Lilliputian::ScriptingAPIs::getInputAPI);
	m.def("get_log_api", &Lilliputian::ScriptingAPIs::getLogAPI);
    m.def("get_scene_api", &Lilliputian::ScriptingAPIs::getSceneAPI);
	m.def("get_time_api", &Lilliputian::ScriptingAPIs::getTimeAPI);
	m.def("get_window_api", &Lilliputian::ScriptingAPIs::getWindowAPI);
}