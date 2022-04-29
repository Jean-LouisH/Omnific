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

PYBIND11_EMBEDDED_MODULE(omnia, m) 
{
	/*API classes*/
	pybind11::class_<Omnia::CommandLineAPI>(m, "CommandLineAPI")
		.def("open_window", &Omnia::CommandLineAPI::openWindow)
		.def("close_window", &Omnia::CommandLineAPI::closeWindow);

	pybind11::class_<Omnia::InputAPI>(m, "InputAPI")
		.def("is_on_press", pybind11::overload_cast<std::string>(&Omnia::InputAPI::isOnPress))
		.def("is_on_press", pybind11::overload_cast<std::vector<std::string>>(&Omnia::InputAPI::isOnPress))
		.def("is_on_press", pybind11::overload_cast<std::vector<std::string>, Omnia::PlayerID>(&Omnia::InputAPI::isOnPress))
		.def("is_on_double_press", pybind11::overload_cast<std::string, unsigned int>(&Omnia::InputAPI::isOnDoublePress))
		.def("is_on_double_press", pybind11::overload_cast<std::vector<std::string>, unsigned int>(&Omnia::InputAPI::isOnDoublePress))
		.def("is_on_double_press", pybind11::overload_cast<std::vector<std::string>, unsigned int>(&Omnia::InputAPI::isOnDoublePress))
		.def("is_pressed", pybind11::overload_cast<std::string>(&Omnia::InputAPI::isPressed))
		.def("is_pressed", pybind11::overload_cast<std::vector<std::string>>(&Omnia::InputAPI::isPressed))
		.def("is_pressed", pybind11::overload_cast<std::vector<std::string>, Omnia::PlayerID>(&Omnia::InputAPI::isPressed))
		.def("is_on_release", pybind11::overload_cast<std::string>(&Omnia::InputAPI::isOnRelease))
		.def("is_on_release", pybind11::overload_cast<std::vector<std::string>>(&Omnia::InputAPI::isOnRelease))
		.def("is_on_release", pybind11::overload_cast<std::vector<std::string>, Omnia::PlayerID>(&Omnia::InputAPI::isOnRelease))
		.def("is_released", pybind11::overload_cast<std::string>(&Omnia::InputAPI::isReleased))
		.def("is_released", pybind11::overload_cast<std::string, Omnia::PlayerID>(&Omnia::InputAPI::isReleased))
		.def("is_left_mouse_button_on_press", &Omnia::InputAPI::isLeftMouseButtonOnPress)
		.def("is_left_mouse_button_on_release", &Omnia::InputAPI::isLeftMouseButtonOnRelease)
		.def("is_left_mouse_button_double_clicked", &Omnia::InputAPI::isLeftMouseButtonDoubleClicked)
		.def("is_middle_mouse_button_on_press", &Omnia::InputAPI::isMiddleMouseButtonOnPress)
		.def("is_middle_mouse_button_on_release", &Omnia::InputAPI::isMiddleMouseButtonOnRelease)
		.def("is_middle_mouse_button_double_clicked", &Omnia::InputAPI::isMiddleMouseButtonDoubleClicked)
		.def("is_right_mouse_button_on_press", &Omnia::InputAPI::isRightMouseButtonOnPress)
		.def("is_right_mouse_button_on_release", &Omnia::InputAPI::isRightMouseButtonOnRelease)
		.def("is_right_mouse_button_double_clicked", &Omnia::InputAPI::isRightMouseButtonDoubleClicked)
		.def("get_mouse_position", &Omnia::InputAPI::getMousePosition)
		.def("get_mouse_wheel_velocity", &Omnia::InputAPI::getMouseWheelVelocity)
		.def("get_mouse_motion_velocity", &Omnia::InputAPI::getMouseMotionVelocity)
		.def("force_shutdown", &Omnia::InputAPI::forceShutdown)
		.def("force_restart", &Omnia::InputAPI::forceRestart);

	pybind11::class_<Omnia::LogAPI>(m, "LogAPI")
		.def("write", &Omnia::LogAPI::write)
		.def("write_to_file", &Omnia::LogAPI::writeToFile)
		.def("get_last_message", &Omnia::LogAPI::getLastMessage)
		.def("get_logs", &Omnia::LogAPI::getLogs);

	pybind11::class_<Omnia::SceneAPI>(m, "SceneAPI")
		.def("has_component", &Omnia::SceneAPI::hasComponent)
		.def("preload_scene", &Omnia::SceneAPI::preloadScene)
		.def("load_scene", &Omnia::SceneAPI::loadScene)
		.def("unload_scene", &Omnia::SceneAPI::unloadScene)
		.def("change_to_scene", &Omnia::SceneAPI::changeToScene)
		.def("get_entity", &Omnia::SceneAPI::getEntity, pybind11::return_value_policy::reference)
		.def("get_component", &Omnia::SceneAPI::getComponent, pybind11::return_value_policy::reference)
		.def("get_scene", &Omnia::SceneAPI::getScene, pybind11::return_value_policy::reference)
		.def("get_scene_tree", &Omnia::SceneAPI::getSceneTree, pybind11::return_value_policy::reference)
		.def("load_image", &Omnia::SceneAPI::loadImage, pybind11::return_value_policy::reference);

	pybind11::class_<Omnia::TimeAPI>(m, "TimeAPI")
		.def("set_ms_per_compute_update", &Omnia::TimeAPI::setMsPerComputeUpdate)
		.def("set_target_fps", &Omnia::TimeAPI::setTargetFPS)
		.def("get_ms_per_compute_update", &Omnia::TimeAPI::setMsPerComputeUpdate)
		.def("get_target_fps", &Omnia::TimeAPI::setTargetFPS)
		.def("get_frame_time_delta", &Omnia::TimeAPI::getFrameTimeDelta);

    pybind11::class_<Omnia::WindowAPI>(m, "WindowAPI")
		.def("set_to_windowed", &Omnia::WindowAPI::setToWindowed)
		.def("set_to_fullscreen", &Omnia::WindowAPI::setToFullscreen)
        .def("toggle_windowed_fullscreen", &Omnia::WindowAPI::toggleWindowedFullscreen)
		.def("resize", &Omnia::WindowAPI::resize)
		.def("change_title", &Omnia::WindowAPI::changeTitle)
		.def("change_icon", &Omnia::WindowAPI::changeIcon)
		.def("maximize", &Omnia::WindowAPI::maximize)
		.def("minimize", &Omnia::WindowAPI::minimize)
		.def("raise", &Omnia::WindowAPI::raise)
		.def("restore", &Omnia::WindowAPI::restore)
		.def("hide", &Omnia::WindowAPI::hide)
		.def("show", &Omnia::WindowAPI::show);

	/*Scene classes*/

	pybind11::class_<Omnia::Entity>(m, "Entity")
		.def_readonly("ID", &Omnia::Entity::id)
		.def_readwrite("name", &Omnia::Entity::name)
		.def_readwrite("parentID", &Omnia::Entity::parentID)
		.def_readwrite("childIDs", &Omnia::Entity::childIDs)
		.def_readwrite("tags", &Omnia::Entity::tags);

	pybind11::class_<Omnia::SceneTree>(m, "SceneTree")	
		.def("add_entity", &Omnia::SceneTree::addEntity)
		.def("add_empty_entity", &Omnia::SceneTree::addEmptyEntity)
		.def("add_component", &Omnia::SceneTree::addComponent)
		.def("add_component_to_last_entity", &Omnia::SceneTree::addComponentToLastEntity)
		.def("remove_entity", &Omnia::SceneTree::removeEntity)
		.def("remove_component", &Omnia::SceneTree::removeComponent)
		.def("get_component_variants", &Omnia::SceneTree::getComponents, pybind11::return_value_policy::reference)
		.def("get_entity_transform", &Omnia::SceneTree::getEntityTransform, pybind11::return_value_policy::reference)
		.def("get_entity", &Omnia::SceneTree::getEntity, pybind11::return_value_policy::reference)
		.def("get_entity_by_name", &Omnia::SceneTree::getEntityByName, pybind11::return_value_policy::reference)
		.def("get_last_entity", &Omnia::SceneTree::getLastEntity, pybind11::return_value_policy::reference)
		.def("get_entities", &Omnia::SceneTree::getEntities, pybind11::return_value_policy::reference)
		.def("get_event_bus", &Omnia::SceneTree::getEventBus, pybind11::return_value_policy::reference)
		.def("get_haptic_signal_buffer", &Omnia::SceneTree::getHapticSignalBuffer, pybind11::return_value_policy::reference)
		.def("get_id", &Omnia::SceneTree::getID);

	pybind11::class_<Omnia::Event::Parameters>(m, "EventParameters")
		.def_readwrite("floats", &Omnia::Event::Parameters::floats)
		.def_readwrite("strings", &Omnia::Event::Parameters::strings);

	pybind11::class_<Omnia::Event>(m, "Event")
		.def(pybind11::init<std::string, uint64_t, Omnia::Event::Parameters>())
		.def(pybind11::init<std::string, uint64_t>())
		.def("get_name", &Omnia::Event::getName)
		.def("get_parameters", &Omnia::Event::getParameters)
		.def("get_timestamp", &Omnia::Event::getTimestamp);

	pybind11::class_<Omnia::EventBus>(m, "EventBus")
		.def("clear", &Omnia::EventBus::clear)
		.def("query", &Omnia::EventBus::query)
		.def("publish", pybind11::overload_cast<std::string>(&Omnia::EventBus::publish))
		.def("publish", pybind11::overload_cast<std::string, std::unordered_map<std::string, float>, std::unordered_map<std::string, std::string>>(&Omnia::EventBus::publish))
		.def("publish", pybind11::overload_cast<std::string, std::unordered_map<std::string, std::string>>(&Omnia::EventBus::publish))
		.def("publish", pybind11::overload_cast<std::string, std::unordered_map<std::string, float>>(&Omnia::EventBus::publish));

	pybind11::class_<Omnia::HapticSignal>(m, "HapticSignal")
		.def(pybind11::init<Omnia::PlayerID, float, uint16_t>())
		.def("get_duration", &Omnia::HapticSignal::getDuration)
		.def("get_player_id", &Omnia::HapticSignal::getPlayerID)
		.def("get_strength", &Omnia::HapticSignal::getStrength);

	pybind11::class_<Omnia::HapticSignalBuffer>(m, "HapticSignalBuffer")
		.def("publish", &Omnia::HapticSignalBuffer::publish)
		.def("query", &Omnia::HapticSignalBuffer::query)
		.def("get_haptic_signals", &Omnia::HapticSignalBuffer::getHapticSignals)
		.def("clear", &Omnia::HapticSignalBuffer::clear);

	/*Asset classes*/

	pybind11::class_<Omnia::Asset, std::shared_ptr<Omnia::Asset>>(m, "Asset")
		.def("get_id", &Omnia::Asset::getID)
		.def("set_name", &Omnia::Asset::setName)
		.def("get_name", &Omnia::Asset::getName)
		.def("get_type", &Omnia::Asset::getType);

	pybind11::class_<Omnia::AudioStream, Omnia::Asset, std::shared_ptr<Omnia::AudioStream>>(m, Omnia::AudioStream::TYPE_STRING);
	pybind11::class_<Omnia::Font, Omnia::Asset, std::shared_ptr<Omnia::Font>>(m, Omnia::Font::TYPE_STRING);
	pybind11::class_<Omnia::Image, Omnia::Asset, std::shared_ptr<Omnia::Image>>(m, Omnia::Image::TYPE_STRING);
	pybind11::class_<Omnia::Text, Omnia::Asset, std::shared_ptr<Omnia::Text>>(m, Omnia::Text::TYPE_STRING);

	/*Component classes*/
	pybind11::class_<Omnia::Component>(m, "Component")
		.def("set_entity_id", &Omnia::Component::setEntityID)
		.def("get_id", &Omnia::Component::getID)
		.def("is_attached_to_entity", &Omnia::Component::isAttachedToEntity)
		.def("get_type", &Omnia::Component::getType)
		.def("is_renderable", &Omnia::Component::isRenderable);

	pybind11::class_<Omnia::BehaviourTree, Omnia::Component>(m, Omnia::BehaviourTree::TYPE_STRING);
	pybind11::class_<Omnia::SightPerception, Omnia::Component>(m, Omnia::SightPerception::TYPE_STRING);
	pybind11::class_<Omnia::SoundPerception, Omnia::Component>(m, Omnia::SoundPerception::TYPE_STRING);
	pybind11::class_<Omnia::AudioListener, Omnia::Component>(m, Omnia::AudioListener::TYPE_STRING);
	pybind11::class_<Omnia::AudioSource, Omnia::Component>(m, Omnia::AudioSource::TYPE_STRING);
	pybind11::class_<Omnia::Camera, Omnia::Component>(m, Omnia::Camera::TYPE_STRING);
	pybind11::class_<Omnia::ConstantForce, Omnia::Component>(m, Omnia::ConstantForce::TYPE_STRING);
	pybind11::class_<Omnia::CountdownTimer, Omnia::Component>(m, Omnia::CountdownTimer::TYPE_STRING)
		.def("start", &Omnia::CountdownTimer::start)
		.def("stop", &Omnia::CountdownTimer::stop)
		.def("is_finished", &Omnia::CountdownTimer::isFinished);
	pybind11::class_<Omnia::CharacterBody, Omnia::Component>(m, Omnia::CharacterBody::TYPE_STRING);
	pybind11::class_<Omnia::Light, Omnia::Component>(m, Omnia::Light::TYPE_STRING);
	pybind11::class_<Omnia::ModelContainer, Omnia::Component>(m, Omnia::ModelContainer::TYPE_STRING)
		.def("add_model", &Omnia::ModelContainer::addModel)
		.def("add_cube", &Omnia::ModelContainer::addCube)
		.def("add_textured_cube", &Omnia::ModelContainer::addTexturedCube)
		.def("set_model_index", &Omnia::ModelContainer::setModelIndex)
		.def("get_current_model", &Omnia::ModelContainer::getCurrentModel, pybind11::return_value_policy::reference);
	pybind11::class_<Omnia::NavigationMeshAgent, Omnia::Component>(m, Omnia::NavigationMeshAgent::TYPE_STRING);
	pybind11::class_<Omnia::NavigationMeshBoxObstacle, Omnia::Component>(m, Omnia::NavigationMeshBoxObstacle::TYPE_STRING);
	pybind11::class_<Omnia::NavigationPath, Omnia::Component>(m, Omnia::NavigationPath::TYPE_STRING);
	pybind11::class_<Omnia::PropertyAnimation, Omnia::Component>(m, Omnia::PropertyAnimation::TYPE_STRING);
	pybind11::class_<Omnia::Collider, Omnia::Component>(m, Omnia::Collider::TYPE_STRING);
	pybind11::class_<Omnia::RigidBody, Omnia::Component>(m, Omnia::RigidBody::TYPE_STRING);
	pybind11::class_<Omnia::SpriteContainer, Omnia::Component>(m, Omnia::SpriteContainer::TYPE_STRING);
	pybind11::class_<Omnia::StaticFluid, Omnia::Component>(m, Omnia::StaticFluid::TYPE_STRING);
	pybind11::class_<Omnia::Transform, Omnia::Component>(m, Omnia::Transform::TYPE_STRING)
		.def("globally_translate_x", &Omnia::Transform::globallyTranslateX)
		.def("globally_translate_y", &Omnia::Transform::globallyTranslateY)
		.def("globally_translate_z", &Omnia::Transform::globallyTranslateZ)
		.def("rotate_x", &Omnia::Transform::rotateX)
		.def("rotate_y", &Omnia::Transform::rotateY)
		.def("rotate_z", &Omnia::Transform::rotateZ);
	pybind11::class_<Omnia::UIAccordion, Omnia::Component>(m, Omnia::UIAccordion::TYPE_STRING);
	pybind11::class_<Omnia::UIBreadcrumbs, Omnia::Component>(m, Omnia::UIBreadcrumbs::TYPE_STRING);
	pybind11::class_<Omnia::UIButton, Omnia::Component>(m, Omnia::UIButton::TYPE_STRING);
	pybind11::class_<Omnia::UIDropdown, Omnia::Component>(m, Omnia::UIDropdown::TYPE_STRING);
	pybind11::class_<Omnia::UIGraph, Omnia::Component>(m, Omnia::UIGraph::TYPE_STRING);
	pybind11::class_<Omnia::UIGridView, Omnia::Component>(m, Omnia::UIGridView::TYPE_STRING);
	pybind11::class_<Omnia::UIHoverCard, Omnia::Component>(m, Omnia::UIHoverCard::TYPE_STRING);
	pybind11::class_<Omnia::UIList, Omnia::Component>(m, Omnia::UIList::TYPE_STRING);
	pybind11::class_<Omnia::UILoader, Omnia::Component>(m, Omnia::UILoader::TYPE_STRING);
	pybind11::class_<Omnia::UIPanel, Omnia::Component>(m, Omnia::UIPanel::TYPE_STRING);
	pybind11::class_<Omnia::UIPlaceholder, Omnia::Component>(m, Omnia::UIPlaceholder::TYPE_STRING);
	pybind11::class_<Omnia::UIProgressBar, Omnia::Component>(m, Omnia::UIProgressBar::TYPE_STRING);
	pybind11::class_<Omnia::UIReveal, Omnia::Component>(m, Omnia::UIReveal::TYPE_STRING);
	pybind11::class_<Omnia::UISearchField, Omnia::Component>(m, Omnia::UISearchField::TYPE_STRING);
	pybind11::class_<Omnia::UISeparator, Omnia::Component>(m, Omnia::UISeparator::TYPE_STRING);
	pybind11::class_<Omnia::UISlider, Omnia::Component>(m, Omnia::UISlider::TYPE_STRING);
	pybind11::class_<Omnia::UISpinBox, Omnia::Component>(m, Omnia::UISpinBox::TYPE_STRING);
	pybind11::class_<Omnia::UIStep, Omnia::Component>(m, Omnia::UIStep::TYPE_STRING);
	pybind11::class_<Omnia::UITab, Omnia::Component>(m, Omnia::UITab::TYPE_STRING);
	pybind11::class_<Omnia::UITextField, Omnia::Component>(m, Omnia::UITextField::TYPE_STRING);
	pybind11::class_<Omnia::UITextLabel, Omnia::Component>(m, Omnia::UITextLabel::TYPE_STRING)
		.def("set_text", &Omnia::UITextLabel::setText);
	pybind11::class_<Omnia::UIToggle, Omnia::Component>(m, Omnia::UIToggle::TYPE_STRING);
	pybind11::class_<Omnia::UIToggleGroup, Omnia::Component>(m, Omnia::UIToggleGroup::TYPE_STRING);
	pybind11::class_<Omnia::UITreeView, Omnia::Component>(m, Omnia::UITreeView::TYPE_STRING);
	pybind11::class_<Omnia::UIViewport, Omnia::Component>(m, Omnia::UIViewport::TYPE_STRING);

	/*Utility classes*/
	pybind11::class_<glm::vec2>(m, "Vector2")
		.def_readwrite("x", &glm::vec2::x)
		.def_readwrite("y", &glm::vec2::y);

	pybind11::class_<Omnia::Rectangle>(m, "Rectangle")
		.def_readwrite("height", &Omnia::Rectangle::height)
		.def_readwrite("width", &Omnia::Rectangle::width);
	pybind11::class_<Omnia::HiResTimer>(m, "HiResTimer")
		.def("set_start", &Omnia::HiResTimer::setStart)
		.def("set_end", &Omnia::HiResTimer::setEnd)
		.def("get_delta_in_nanoseconds", &Omnia::HiResTimer::getDeltaInNanoseconds);
	pybind11::class_<Omnia::Colour>(m, "Colour")
		.def(pybind11::init<std::string>())
		.def(pybind11::init<uint8_t, uint8_t, uint8_t, uint8_t>())
		.def("get_red", &Omnia::Colour::getRed)
		.def("get_green", &Omnia::Colour::getGreen)
		.def("get_blue", &Omnia::Colour::getBlue)
		.def("get_alpha", &Omnia::Colour::getAlpha);
	pybind11::class_<Omnia::AABB2D>(m, "AABB2D")
		.def_readwrite("max", &Omnia::AABB2D::max)
		.def_readwrite("min", &Omnia::AABB2D::min);

	/*API getters*/
	m.def("get_command_line_api", &Omnia::ScriptingAPIs::getCommandLineAPI, pybind11::return_value_policy::reference);
	m.def("get_input_api", &Omnia::ScriptingAPIs::getInputAPI, pybind11::return_value_policy::reference);
	m.def("get_log_api", &Omnia::ScriptingAPIs::getLogAPI, pybind11::return_value_policy::reference);
    m.def("get_scene_api", &Omnia::ScriptingAPIs::getSceneAPI, pybind11::return_value_policy::reference);
	m.def("get_time_api", &Omnia::ScriptingAPIs::getTimeAPI, pybind11::return_value_policy::reference);
	m.def("get_window_api", &Omnia::ScriptingAPIs::getWindowAPI, pybind11::return_value_policy::reference);
	m.def("get_data_directory_path", &Omnia::ScriptingAPIs::getDataDirectoryPath);
	m.def("get_executable_directory_path", &Omnia::ScriptingAPIs::getExecutableDirectoryPath);
}