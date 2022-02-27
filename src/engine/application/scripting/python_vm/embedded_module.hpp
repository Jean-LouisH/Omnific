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

#include <application/scripting/scripting_apis/scripting_apis.hpp>
#include <application/scene/component.hpp>
#include <pybind11/embed.h>
#include <pybind11/stl.h>
#include <glm/glm.hpp>
#include <application/scene/scene.hpp>
#include <utilities/rectangle.hpp>
#include <utilities/hi_res_timer.hpp>
#include <utilities/colour.hpp>
#include <utilities/aabb_2d.hpp>
#include <utilities/aliases.hpp>

PYBIND11_EMBEDDED_MODULE(omnific_engine, m) 
{
	/*API classes*/
	pybind11::class_<Omnific::CommandLineAPI>(m, "CommandLineAPI")
		.def("open_window", &Omnific::CommandLineAPI::openWindow)
		.def("close_window", &Omnific::CommandLineAPI::closeWindow);

	pybind11::class_<Omnific::InputAPI>(m, "InputAPI")
		.def("is_on_press", pybind11::overload_cast<std::string>(&Omnific::InputAPI::isOnPress))
		.def("is_on_press", pybind11::overload_cast<std::vector<std::string>>(&Omnific::InputAPI::isOnPress))
		.def("is_on_press", pybind11::overload_cast<std::vector<std::string>, Omnific::PlayerID>(&Omnific::InputAPI::isOnPress))
		.def("is_on_double_press", pybind11::overload_cast<std::string, unsigned int>(&Omnific::InputAPI::isOnDoublePress))
		.def("is_on_double_press", pybind11::overload_cast<std::vector<std::string>, unsigned int>(&Omnific::InputAPI::isOnDoublePress))
		.def("is_on_double_press", pybind11::overload_cast<std::vector<std::string>, unsigned int>(&Omnific::InputAPI::isOnDoublePress))
		.def("is_pressed", pybind11::overload_cast<std::string>(&Omnific::InputAPI::isPressed))
		.def("is_pressed", pybind11::overload_cast<std::vector<std::string>>(&Omnific::InputAPI::isPressed))
		.def("is_pressed", pybind11::overload_cast<std::vector<std::string>, Omnific::PlayerID>(&Omnific::InputAPI::isPressed))
		.def("is_on_release", pybind11::overload_cast<std::string>(&Omnific::InputAPI::isOnRelease))
		.def("is_on_release", pybind11::overload_cast<std::vector<std::string>>(&Omnific::InputAPI::isOnRelease))
		.def("is_on_release", pybind11::overload_cast<std::vector<std::string>, Omnific::PlayerID>(&Omnific::InputAPI::isOnRelease))
		.def("is_released", pybind11::overload_cast<std::string>(&Omnific::InputAPI::isReleased))
		.def("is_released", pybind11::overload_cast<std::string, Omnific::PlayerID>(&Omnific::InputAPI::isReleased))
		.def("is_left_mouse_button_on_press", &Omnific::InputAPI::isLeftMouseButtonOnPress)
		.def("is_left_mouse_button_on_release", &Omnific::InputAPI::isLeftMouseButtonOnRelease)
		.def("is_left_mouse_button_double_clicked", &Omnific::InputAPI::isLeftMouseButtonDoubleClicked)
		.def("is_middle_mouse_button_on_press", &Omnific::InputAPI::isMiddleMouseButtonOnPress)
		.def("is_middle_mouse_button_on_release", &Omnific::InputAPI::isMiddleMouseButtonOnRelease)
		.def("is_middle_mouse_button_double_clicked", &Omnific::InputAPI::isMiddleMouseButtonDoubleClicked)
		.def("is_right_mouse_button_on_press", &Omnific::InputAPI::isRightMouseButtonOnPress)
		.def("is_right_mouse_button_on_release", &Omnific::InputAPI::isRightMouseButtonOnRelease)
		.def("is_right_mouse_button_double_clicked", &Omnific::InputAPI::isRightMouseButtonDoubleClicked)
		.def("get_mouse_position", &Omnific::InputAPI::getMousePosition)
		.def("get_mouse_wheel_velocity", &Omnific::InputAPI::getMouseWheelVelocity)
		.def("get_mouse_motion_velocity", &Omnific::InputAPI::getMouseMotionVelocity)
		.def("force_shutdown", &Omnific::InputAPI::forceShutdown)
		.def("force_restart", &Omnific::InputAPI::forceRestart);

	pybind11::class_<Omnific::LogAPI>(m, "LogAPI")
		.def("write", &Omnific::LogAPI::write)
		.def("write_to_file", &Omnific::LogAPI::writeToFile)
		.def("get_last_message", &Omnific::LogAPI::getLastMessage)
		.def("get_logs", &Omnific::LogAPI::getLogs);

	pybind11::class_<Omnific::SceneAPI>(m, "SceneAPI")
		.def("has_component", &Omnific::SceneAPI::hasComponent)
		.def("preload_scene", &Omnific::SceneAPI::preloadScene)
		.def("load_scene", &Omnific::SceneAPI::loadScene)
		.def("unload_scene", &Omnific::SceneAPI::unloadScene)
		.def("change_to_scene", &Omnific::SceneAPI::changeToScene)
		.def("get_entity", &Omnific::SceneAPI::getEntity, pybind11::return_value_policy::reference)
		.def("get_component", &Omnific::SceneAPI::getComponent, pybind11::return_value_policy::reference)
		.def("get_scene", &Omnific::SceneAPI::getScene, pybind11::return_value_policy::reference);

	pybind11::class_<Omnific::TimeAPI>(m, "TimeAPI")
		.def("set_ms_per_compute_update", &Omnific::TimeAPI::setMsPerComputeUpdate)
		.def("set_target_fps", &Omnific::TimeAPI::setTargetFPS)
		.def("get_ms_per_compute_update", &Omnific::TimeAPI::setMsPerComputeUpdate)
		.def("get_target_fps", &Omnific::TimeAPI::setTargetFPS)
		.def("get_frame_time_delta", &Omnific::TimeAPI::getFrameTimeDelta);

    pybind11::class_<Omnific::WindowAPI>(m, "WindowAPI")
		.def("set_to_windowed", &Omnific::WindowAPI::setToWindowed)
		.def("set_to_fullscreen", &Omnific::WindowAPI::setToFullscreen)
        .def("toggle_windowed_fullscreen", &Omnific::WindowAPI::toggleWindowedFullscreen)
		.def("resize", &Omnific::WindowAPI::resize)
		.def("change_title", &Omnific::WindowAPI::changeTitle)
		.def("change_icon", &Omnific::WindowAPI::changeIcon)
		.def("maximize", &Omnific::WindowAPI::maximize)
		.def("minimize", &Omnific::WindowAPI::minimize)
		.def("raise", &Omnific::WindowAPI::raise)
		.def("restore", &Omnific::WindowAPI::restore)
		.def("hide", &Omnific::WindowAPI::hide)
		.def("show", &Omnific::WindowAPI::show);

	/*Scene classes*/

	pybind11::class_<Omnific::Entity>(m, "Entity")
		.def_readonly("ID", &Omnific::Entity::id)
		.def_readwrite("name", &Omnific::Entity::name)
		.def_readwrite("parentID", &Omnific::Entity::parentID)
		.def_readwrite("childIDs", &Omnific::Entity::childIDs)
		.def_readwrite("tags", &Omnific::Entity::tags);

	pybind11::class_<Omnific::SceneTree>(m, "SceneTree")	
		.def("add_entity", &Omnific::SceneTree::addEntity)
		.def("add_empty_entity", &Omnific::SceneTree::addEmptyEntity)
		.def("add_component", &Omnific::SceneTree::addComponent)
		.def("add_component_to_last_entity", &Omnific::SceneTree::addComponentToLastEntity)
		.def("remove_entity", &Omnific::SceneTree::removeEntity)
		.def("remove_component", &Omnific::SceneTree::removeComponent)
		.def("get_component_variants", &Omnific::SceneTree::getComponents, pybind11::return_value_policy::reference)
		.def("get_entity_transform", &Omnific::SceneTree::getEntityTransform, pybind11::return_value_policy::reference)
		.def("get_entity", &Omnific::SceneTree::getEntity, pybind11::return_value_policy::reference)
		.def("get_entity_by_name", &Omnific::SceneTree::getEntityByName, pybind11::return_value_policy::reference)
		.def("get_last_entity", &Omnific::SceneTree::getLastEntity, pybind11::return_value_policy::reference)
		.def("get_entities", &Omnific::SceneTree::getEntities, pybind11::return_value_policy::reference)
		.def("get_event_bus", &Omnific::SceneTree::getEventBus, pybind11::return_value_policy::reference)
		.def("get_haptic_signal_buffer", &Omnific::SceneTree::getHapticSignalBuffer, pybind11::return_value_policy::reference)
		.def("get_id", &Omnific::SceneTree::getID);

	pybind11::class_<Omnific::Event::Parameters>(m, "EventParameters")
		.def_readwrite("floats", &Omnific::Event::Parameters::floats)
		.def_readwrite("strings", &Omnific::Event::Parameters::strings);

	pybind11::class_<Omnific::Event>(m, "Event")
		.def(pybind11::init<std::string, uint64_t, Omnific::Event::Parameters>())
		.def(pybind11::init<std::string, uint64_t>())
		.def("get_name", &Omnific::Event::getName)
		.def("get_parameters", &Omnific::Event::getParameters)
		.def("get_timestamp", &Omnific::Event::getTimestamp);

	pybind11::class_<Omnific::EventBus>(m, "EventBus")
		.def("clear", &Omnific::EventBus::clear)
		.def("query", &Omnific::EventBus::query)
		.def("publish", pybind11::overload_cast<std::string>(&Omnific::EventBus::publish))
		.def("publish", pybind11::overload_cast<std::string, std::unordered_map<std::string, float>, std::unordered_map<std::string, std::string>>(&Omnific::EventBus::publish))
		.def("publish", pybind11::overload_cast<std::string, std::unordered_map<std::string, std::string>>(&Omnific::EventBus::publish))
		.def("publish", pybind11::overload_cast<std::string, std::unordered_map<std::string, float>>(&Omnific::EventBus::publish));

	pybind11::class_<Omnific::HapticSignal>(m, "HapticSignal")
		.def(pybind11::init<Omnific::PlayerID, float, uint16_t>())
		.def("get_duration", &Omnific::HapticSignal::getDuration)
		.def("get_player_id", &Omnific::HapticSignal::getPlayerID)
		.def("get_strength", &Omnific::HapticSignal::getStrength);

	pybind11::class_<Omnific::HapticSignalBuffer>(m, "HapticSignalBuffer")
		.def("publish", &Omnific::HapticSignalBuffer::publish)
		.def("query", &Omnific::HapticSignalBuffer::query)
		.def("get_haptic_signals", &Omnific::HapticSignalBuffer::getHapticSignals)
		.def("clear", &Omnific::HapticSignalBuffer::clear);

	/*Component classes*/
	pybind11::class_<Omnific::Component>(m, "Component")
		.def("set_entity_id", &Omnific::Component::setEntityID)
		.def("get_id", &Omnific::Component::getID)
		.def("is_attached_to_entity", &Omnific::Component::isAttachedToEntity)
		.def("get_type", &Omnific::Component::getType)
		.def("is_renderable", &Omnific::Component::isRenderable);

	pybind11::class_<Omnific::BehaviourTree, Omnific::Component>(m, Omnific::BehaviourTree::TYPE_STRING);
	pybind11::class_<Omnific::SightPerception, Omnific::Component>(m, Omnific::SightPerception::TYPE_STRING);
	pybind11::class_<Omnific::SoundPerception, Omnific::Component>(m, Omnific::SoundPerception::TYPE_STRING);
	pybind11::class_<Omnific::AudioListener, Omnific::Component>(m, Omnific::AudioListener::TYPE_STRING);
	pybind11::class_<Omnific::AudioSource, Omnific::Component>(m, Omnific::AudioSource::TYPE_STRING);
	pybind11::class_<Omnific::Camera, Omnific::Component>(m, Omnific::Camera::TYPE_STRING);
	pybind11::class_<Omnific::ConstantForce, Omnific::Component>(m, Omnific::ConstantForce::TYPE_STRING);
	pybind11::class_<Omnific::CountdownTimer, Omnific::Component>(m, Omnific::CountdownTimer::TYPE_STRING)
		.def("start", &Omnific::CountdownTimer::start)
		.def("stop", &Omnific::CountdownTimer::stop)
		.def("is_finished", &Omnific::CountdownTimer::isFinished);
	pybind11::class_<Omnific::CharacterBody, Omnific::Component>(m, Omnific::CharacterBody::TYPE_STRING);
	pybind11::class_<Omnific::Light, Omnific::Component>(m, Omnific::Light::TYPE_STRING);
	pybind11::class_<Omnific::NavigationMeshAgent, Omnific::Component>(m, Omnific::NavigationMeshAgent::TYPE_STRING);
	pybind11::class_<Omnific::NavigationMeshBoxObstacle, Omnific::Component>(m, Omnific::NavigationMeshBoxObstacle::TYPE_STRING);
	pybind11::class_<Omnific::NavigationPath, Omnific::Component>(m, Omnific::NavigationPath::TYPE_STRING);
	pybind11::class_<Omnific::PropertyAnimation, Omnific::Component>(m, Omnific::PropertyAnimation::TYPE_STRING);
	pybind11::class_<Omnific::Collider, Omnific::Component>(m, Omnific::Collider::TYPE_STRING);
	pybind11::class_<Omnific::ModelContainer, Omnific::Component>(m, Omnific::ModelContainer::TYPE_STRING);
	pybind11::class_<Omnific::RigidBody, Omnific::Component>(m, Omnific::RigidBody::TYPE_STRING);
	pybind11::class_<Omnific::SpriteContainer, Omnific::Component>(m, Omnific::SpriteContainer::TYPE_STRING);
	pybind11::class_<Omnific::StaticFluid, Omnific::Component>(m, Omnific::StaticFluid::TYPE_STRING);
	pybind11::class_<Omnific::Transform, Omnific::Component>(m, Omnific::Transform::TYPE_STRING);
	pybind11::class_<Omnific::UIButton, Omnific::Component>(m, Omnific::UIButton::TYPE_STRING);
	pybind11::class_<Omnific::UIGraphEdit, Omnific::Component>(m, Omnific::UIGraphEdit::TYPE_STRING);
	pybind11::class_<Omnific::UIGraphNode, Omnific::Component>(m, Omnific::UIGraphNode::TYPE_STRING);
	pybind11::class_<Omnific::UIHoverCard, Omnific::Component>(m, Omnific::UIHoverCard::TYPE_STRING);
	pybind11::class_<Omnific::UIItemList, Omnific::Component>(m, Omnific::UIItemList::TYPE_STRING);
	pybind11::class_<Omnific::UIPanel, Omnific::Component>(m, Omnific::UIPanel::TYPE_STRING);
	pybind11::class_<Omnific::UIProgressBar, Omnific::Component>(m, Omnific::UIProgressBar::TYPE_STRING);
	pybind11::class_<Omnific::UIRectangle, Omnific::Component>(m, Omnific::UIRectangle::TYPE_STRING);
	pybind11::class_<Omnific::UIScrollbar, Omnific::Component>(m, Omnific::UIScrollbar::TYPE_STRING);
	pybind11::class_<Omnific::UISeparator, Omnific::Component>(m, Omnific::UISeparator::TYPE_STRING);
	pybind11::class_<Omnific::UISlider, Omnific::Component>(m, Omnific::UISlider::TYPE_STRING);
	pybind11::class_<Omnific::UISpinBox, Omnific::Component>(m, Omnific::UISpinBox::TYPE_STRING);
	pybind11::class_<Omnific::UITab, Omnific::Component>(m, Omnific::UITab::TYPE_STRING);
	pybind11::class_<Omnific::UITextEdit, Omnific::Component>(m, Omnific::UITextEdit::TYPE_STRING);
	pybind11::class_<Omnific::UITextLabel, Omnific::Component>(m, Omnific::UITextLabel::TYPE_STRING)
		.def("set_text", &Omnific::UITextLabel::setText);
	pybind11::class_<Omnific::UITree, Omnific::Component>(m, Omnific::UITree::TYPE_STRING);
	pybind11::class_<Omnific::UIViewport, Omnific::Component>(m, Omnific::UIViewport::TYPE_STRING);

	/*Asset classes*/

	pybind11::class_<Omnific::Asset>(m, "Asset")
		.def("get_id", &Omnific::Asset::getID)
		.def("set_name", &Omnific::Asset::setName)
		.def("get_name", &Omnific::Asset::getName)
		.def("get_type", &Omnific::Asset::getType);

	pybind11::class_<Omnific::AudioStream, Omnific::Asset>(m, Omnific::AudioStream::TYPE_STRING);
	pybind11::class_<Omnific::Font, Omnific::Asset>(m, Omnific::Font::TYPE_STRING);
	pybind11::class_<Omnific::Image, Omnific::Asset>(m, Omnific::Image::TYPE_STRING);
	pybind11::class_<Omnific::Text, Omnific::Asset>(m, Omnific::Text::TYPE_STRING);

	/*Utility classes*/
	pybind11::class_<glm::vec2>(m, "Vector2")
		.def_readwrite("x", &glm::vec2::x)
		.def_readwrite("y", &glm::vec2::y);

	pybind11::class_<Omnific::Rectangle>(m, "Rectangle")
		.def_readwrite("height", &Omnific::Rectangle::height)
		.def_readwrite("width", &Omnific::Rectangle::width);
	pybind11::class_<Omnific::HiResTimer>(m, "HiResTimer")
		.def("set_start", &Omnific::HiResTimer::setStart)
		.def("set_end", &Omnific::HiResTimer::setEnd)
		.def("get_delta_in_nanoseconds", &Omnific::HiResTimer::getDeltaInNanoseconds);
	pybind11::class_<Omnific::Colour>(m, "Colour")
		.def(pybind11::init<std::string>())
		.def(pybind11::init<uint8_t, uint8_t, uint8_t, uint8_t>())
		.def("get_red", &Omnific::Colour::getRed)
		.def("get_green", &Omnific::Colour::getGreen)
		.def("get_blue", &Omnific::Colour::getBlue)
		.def("get_alpha", &Omnific::Colour::getAlpha);
	pybind11::class_<Omnific::AABB2D>(m, "AABB2D")
		.def_readwrite("max", &Omnific::AABB2D::max)
		.def_readwrite("min", &Omnific::AABB2D::min);

	/*API getters*/
	m.def("get_command_line_api", &Omnific::ScriptingAPIs::getCommandLineAPI);
	m.def("get_input_api", &Omnific::ScriptingAPIs::getInputAPI);
	m.def("get_log_api", &Omnific::ScriptingAPIs::getLogAPI);
    m.def("get_scene_api", &Omnific::ScriptingAPIs::getSceneAPI);
	m.def("get_time_api", &Omnific::ScriptingAPIs::getTimeAPI);
	m.def("get_window_api", &Omnific::ScriptingAPIs::getWindowAPI);
	m.def("get_data_directory_path", &Omnific::ScriptingAPIs::getDataDirectoryPath);
	m.def("get_executable_directory_path", &Omnific::ScriptingAPIs::getExecutableDirectoryPath);
}