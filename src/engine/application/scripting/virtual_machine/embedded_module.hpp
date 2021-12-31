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

PYBIND11_EMBEDDED_MODULE(esi, m) 
{
	/*API classes*/
	pybind11::class_<Esi::CommandLineAPI>(m, "CommandLineAPI")
		.def("open_window", &Esi::CommandLineAPI::openWindow)
		.def("close_window", &Esi::CommandLineAPI::closeWindow);

	pybind11::class_<Esi::InputAPI>(m, "InputAPI")
		.def("is_on_press", pybind11::overload_cast<std::string>(&Esi::InputAPI::isOnPress))
		.def("is_on_press", pybind11::overload_cast<std::vector<std::string>>(&Esi::InputAPI::isOnPress))
		.def("is_on_press", pybind11::overload_cast<std::vector<std::string>, Esi::PlayerID>(&Esi::InputAPI::isOnPress))
		.def("is_on_double_press", pybind11::overload_cast<std::string, unsigned int>(&Esi::InputAPI::isOnDoublePress))
		.def("is_on_double_press", pybind11::overload_cast<std::vector<std::string>, unsigned int>(&Esi::InputAPI::isOnDoublePress))
		.def("is_on_double_press", pybind11::overload_cast<std::vector<std::string>, unsigned int>(&Esi::InputAPI::isOnDoublePress))
		.def("is_pressed", pybind11::overload_cast<std::string>(&Esi::InputAPI::isPressed))
		.def("is_pressed", pybind11::overload_cast<std::vector<std::string>>(&Esi::InputAPI::isPressed))
		.def("is_pressed", pybind11::overload_cast<std::vector<std::string>, Esi::PlayerID>(&Esi::InputAPI::isPressed))
		.def("is_on_release", pybind11::overload_cast<std::string>(&Esi::InputAPI::isOnRelease))
		.def("is_on_release", pybind11::overload_cast<std::vector<std::string>>(&Esi::InputAPI::isOnRelease))
		.def("is_on_release", pybind11::overload_cast<std::vector<std::string>, Esi::PlayerID>(&Esi::InputAPI::isOnRelease))
		.def("is_released", pybind11::overload_cast<std::string>(&Esi::InputAPI::isReleased))
		.def("is_released", pybind11::overload_cast<std::string, Esi::PlayerID>(&Esi::InputAPI::isReleased))
		.def("is_left_mouse_button_on_press", &Esi::InputAPI::isLeftMouseButtonOnPress)
		.def("is_left_mouse_button_on_release", &Esi::InputAPI::isLeftMouseButtonOnRelease)
		.def("is_left_mouse_button_double_clicked", &Esi::InputAPI::isLeftMouseButtonDoubleClicked)
		.def("is_middle_mouse_button_on_press", &Esi::InputAPI::isMiddleMouseButtonOnPress)
		.def("is_middle_mouse_button_on_release", &Esi::InputAPI::isMiddleMouseButtonOnRelease)
		.def("is_middle_mouse_button_double_clicked", &Esi::InputAPI::isMiddleMouseButtonDoubleClicked)
		.def("is_right_mouse_button_on_press", &Esi::InputAPI::isRightMouseButtonOnPress)
		.def("is_right_mouse_button_on_release", &Esi::InputAPI::isRightMouseButtonOnRelease)
		.def("is_right_mouse_button_double_clicked", &Esi::InputAPI::isRightMouseButtonDoubleClicked)
		.def("get_mouse_position", &Esi::InputAPI::getMousePosition)
		.def("get_mouse_wheel_velocity", &Esi::InputAPI::getMouseWheelVelocity)
		.def("get_mouse_motion_velocity", &Esi::InputAPI::getMouseMotionVelocity)
		.def("force_shutdown", &Esi::InputAPI::forceShutdown)
		.def("force_restart", &Esi::InputAPI::forceRestart);

	pybind11::class_<Esi::LogAPI>(m, "LogAPI")
		.def("write", &Esi::LogAPI::write)
		.def("write_to_file", &Esi::LogAPI::writeToFile)
		.def("get_last_message", &Esi::LogAPI::getLastMessage)
		.def("get_logs", &Esi::LogAPI::getLogs);

	pybind11::class_<Esi::SceneAPI>(m, "SceneAPI")
		.def("this_has_component", &Esi::SceneAPI::thisHasComponent)
		.def("preload_scene", &Esi::SceneAPI::preloadScene)
		.def("load_scene", &Esi::SceneAPI::loadScene)
		.def("unload_scene", &Esi::SceneAPI::unloadScene)
		.def("change_to_scene", &Esi::SceneAPI::changeToScene)
		.def("get_this_entity", &Esi::SceneAPI::getThisEntity, pybind11::return_value_policy::reference)
		.def("get_this_component", &Esi::SceneAPI::getThisComponent, pybind11::return_value_policy::reference)
		.def("get_this_scene", &Esi::SceneAPI::getThisScene, pybind11::return_value_policy::reference);

	pybind11::class_<Esi::TimeAPI>(m, "TimeAPI")
		.def("set_ms_per_compute_update", &Esi::TimeAPI::setMsPerComputeUpdate)
		.def("set_target_fps", &Esi::TimeAPI::setTargetFPS)
		.def("get_ms_per_compute_update", &Esi::TimeAPI::setMsPerComputeUpdate)
		.def("get_target_fps", &Esi::TimeAPI::setTargetFPS)
		.def("get_frame_time_delta", &Esi::TimeAPI::getFrameTimeDelta);

    pybind11::class_<Esi::WindowAPI>(m, "WindowAPI")
		.def("set_to_windowed", &Esi::WindowAPI::setToWindowed)
		.def("set_to_fullscreen", &Esi::WindowAPI::setToFullscreen)
        .def("toggle_windowed_fullscreen", &Esi::WindowAPI::toggleWindowedFullscreen)
		.def("resize", &Esi::WindowAPI::resize)
		.def("change_title", &Esi::WindowAPI::changeTitle)
		.def("change_icon", &Esi::WindowAPI::changeIcon)
		.def("maximize", &Esi::WindowAPI::maximize)
		.def("minimize", &Esi::WindowAPI::minimize)
		.def("raise", &Esi::WindowAPI::raise)
		.def("restore", &Esi::WindowAPI::restore)
		.def("hide", &Esi::WindowAPI::hide)
		.def("show", &Esi::WindowAPI::show);

	/*Scene classes*/

	pybind11::class_<Esi::Entity>(m, "Entity")
		.def_readonly("ID", &Esi::Entity::ID)
		.def_readwrite("name", &Esi::Entity::name)
		.def_readwrite("parentID", &Esi::Entity::parentID)
		.def_readwrite("childIDs", &Esi::Entity::childIDs)
		.def_readwrite("tags", &Esi::Entity::tags);

	pybind11::class_<Esi::Scene>(m, "Scene")
		.def("get_asset_cache", &Esi::Scene::getAssetCache, pybind11::return_value_policy::reference)	
		.def("add_entity", &Esi::Scene::addEntity)
		.def("add_empty_entity", &Esi::Scene::addEmptyEntity)
		.def("add_component", &Esi::Scene::addComponent)
		.def("add_component_to_last_entity", &Esi::Scene::addComponentToLastEntity)
		.def("remove_entity", &Esi::Scene::removeEntity)
		.def("remove_component", &Esi::Scene::removeComponent)
		.def("get_component_variants", &Esi::Scene::getComponents, pybind11::return_value_policy::reference)
		.def("get_entity_transform", &Esi::Scene::getEntityTransform, pybind11::return_value_policy::reference)
		.def("get_entity", &Esi::Scene::getEntity, pybind11::return_value_policy::reference)
		.def("get_entity_by_name", &Esi::Scene::getEntityByName, pybind11::return_value_policy::reference)
		.def("get_last_entity", &Esi::Scene::getLastEntity, pybind11::return_value_policy::reference)
		.def("get_entities", &Esi::Scene::getEntities, pybind11::return_value_policy::reference)
		.def("get_event_bus", &Esi::Scene::getEventBus, pybind11::return_value_policy::reference)
		.def("get_haptic_signal_buffer", &Esi::Scene::getHapticSignalBuffer, pybind11::return_value_policy::reference)
		.def("get_id", &Esi::Scene::getID);

	pybind11::class_<Esi::Event::Parameters>(m, "EventParameters")
		.def_readwrite("floats", &Esi::Event::Parameters::floats)
		.def_readwrite("strings", &Esi::Event::Parameters::strings);

	pybind11::class_<Esi::Event>(m, "Event")
		.def(pybind11::init<std::string, uint64_t, Esi::Event::Parameters>())
		.def(pybind11::init<std::string, uint64_t>())
		.def("get_name", &Esi::Event::getName)
		.def("get_parameters", &Esi::Event::getParameters)
		.def("get_timestamp", &Esi::Event::getTimestamp);

	pybind11::class_<Esi::EventBus>(m, "EventBus")
		.def("clear", &Esi::EventBus::clear)
		.def("query", &Esi::EventBus::query)
		.def("publish", pybind11::overload_cast<std::string>(&Esi::EventBus::publish))
		.def("publish", pybind11::overload_cast<std::string, std::vector<float>>(&Esi::EventBus::publish))
		.def("publish", pybind11::overload_cast<std::string, std::vector<std::string>>(&Esi::EventBus::publish))
		.def("publish", pybind11::overload_cast<std::string, std::vector<float>, std::vector<std::string>>(&Esi::EventBus::publish));

	pybind11::class_<Esi::HapticSignal>(m, "HapticSignal")
		.def(pybind11::init<Esi::PlayerID, float, uint16_t>())
		.def("get_duration_ms", &Esi::HapticSignal::getDuration_ms)
		.def("get_player_id", &Esi::HapticSignal::getPlayerID)
		.def("get_strength_pct", &Esi::HapticSignal::getStrength_pct);

	pybind11::class_<Esi::HapticSignalBuffer>(m, "HapticSignalBuffer")
		.def("publish", &Esi::HapticSignalBuffer::publish)
		.def("query", &Esi::HapticSignalBuffer::query)
		.def("get_haptic_signals", &Esi::HapticSignalBuffer::getHapticSignals)
		.def("clear", &Esi::HapticSignalBuffer::clear);

	/*Component classes*/
	pybind11::class_<Esi::Component>(m, "Component")
		.def("set_entity_id", &Esi::Component::setEntityID)
		.def("get_id", &Esi::Component::getID)
		.def("is_attached_to_entity", &Esi::Component::isAttachedToEntity)
		.def("get_type", &Esi::Component::getType)
		.def("is_renderable", &Esi::Component::isRenderable);

	pybind11::class_<Esi::BehaviourTree, Esi::Component>(m, "BehaviourTree");
	pybind11::class_<Esi::SightPerception, Esi::Component>(m, "SightPerception");
	pybind11::class_<Esi::SoundPerception, Esi::Component>(m, "SoundPerception");
	pybind11::class_<Esi::AnimatedSprite, Esi::Component>(m, "AnimatedSprite");
	pybind11::class_<Esi::AudioListener, Esi::Component>(m, "AudioListener");
	pybind11::class_<Esi::AudioStreamSource, Esi::Component>(m, "AudioStreamSource");
	pybind11::class_<Esi::Camera, Esi::Component>(m, "Camera");
	pybind11::class_<Esi::BallCollider, Esi::Component>(m, "BallCollider");
	pybind11::class_<Esi::ConstantDirectionalForce, Esi::Component>(m, "ConstantDirectionalForce");
	pybind11::class_<Esi::ConstantPointForce, Esi::Component>(m, "ConstantPointForce");
	pybind11::class_<Esi::CountdownTimer, Esi::Component>(m, "CountdownTimer")
		.def("start", &Esi::CountdownTimer::start)
		.def("stop", &Esi::CountdownTimer::stop)
		.def("is_finished", &Esi::CountdownTimer::isFinished);
	pybind11::class_<Esi::KinematicBody, Esi::Component>(m, "KinematicBody");
	pybind11::class_<Esi::Light, Esi::Component>(m, "Light");
	pybind11::class_<Esi::NavigationMeshAgent, Esi::Component>(m, "NavigationMeshAgent");
	pybind11::class_<Esi::NavigationMeshBoxObstacle, Esi::Component>(m, "NavigationMeshBoxObstacle");
	pybind11::class_<Esi::NavigationPath, Esi::Component>(m, "NavigationPath");
	pybind11::class_<Esi::PhysicsConstraint, Esi::Component>(m, "PhysicsConstraint");
	pybind11::class_<Esi::PhysicsThruster, Esi::Component>(m, "PhysicsThruster");
	pybind11::class_<Esi::PropertyAnimation, Esi::Component>(m, "PropertyAnimation");
	pybind11::class_<Esi::BoxCollider, Esi::Component>(m, "BoxCollider");
	pybind11::class_<Esi::BoxMesh, Esi::Component>(m, "BoxMesh");
	pybind11::class_<Esi::BoxTriggerSpace, Esi::Component>(m, "BoxTriggerSpace");
	pybind11::class_<Esi::RegularPolytopalMesh, Esi::Component>(m, "RegularPolytopalMesh");
	pybind11::class_<Esi::RigidBody, Esi::Component>(m, "RigidBody");
	pybind11::class_<Esi::Sprite, Esi::Component>(m, "Sprite");
	pybind11::class_<Esi::StaticFluid, Esi::Component>(m, "StaticFluid");
	pybind11::class_<Esi::Transform, Esi::Component>(m, "Transform");
	pybind11::class_<Esi::UIButton, Esi::Component>(m, "UIButton");
	pybind11::class_<Esi::UIGraphEdit, Esi::Component>(m, "UIGraphEdit");
	pybind11::class_<Esi::UIGraphNode, Esi::Component>(m, "UIGraphNode");
	pybind11::class_<Esi::UIHoverCard, Esi::Component>(m, "UIHoverCard");
	pybind11::class_<Esi::UIItemList, Esi::Component>(m, "UIItemList");
	pybind11::class_<Esi::UIPanel, Esi::Component>(m, "UIPanel");
	pybind11::class_<Esi::UIProgressBar, Esi::Component>(m, "UIProgressBar");
	pybind11::class_<Esi::UIRectangle, Esi::Component>(m, "UIRectangle");
	pybind11::class_<Esi::UIScrollbar, Esi::Component>(m, "UIScrollbar");
	pybind11::class_<Esi::UISeparator, Esi::Component>(m, "UISeparator");
	pybind11::class_<Esi::UISlider, Esi::Component>(m, "UISlider");
	pybind11::class_<Esi::UISpinBox, Esi::Component>(m, "UISpinBox");
	pybind11::class_<Esi::UITab, Esi::Component>(m, "UITab");
	pybind11::class_<Esi::UITextEdit, Esi::Component>(m, "UITextEdit");
	pybind11::class_<Esi::UITextLabel, Esi::Component>(m, "UITextLabel")
		.def("set_text", &Esi::UITextLabel::setText);
	pybind11::class_<Esi::UITree, Esi::Component>(m, "UITree");
	pybind11::class_<Esi::UIViewport, Esi::Component>(m, "UIViewport");

	/*Asset classes*/

	pybind11::class_<Esi::Asset>(m, "Asset")
		.def("get_id", &Esi::Asset::getID)
		.def("set_name", &Esi::Asset::setName)
		.def("get_name", &Esi::Asset::getName)
		.def("get_type", &Esi::Asset::getType)
		.def("unload", &Esi::Asset::unload);

	pybind11::class_<Esi::AudioStream, Esi::Asset>(m, "AudioStream");
	pybind11::class_<Esi::Font, Esi::Asset>(m, "Font");
	pybind11::class_<Esi::Image, Esi::Asset>(m, "Image");
	pybind11::class_<Esi::Text, Esi::Asset>(m, "Text");

	pybind11::class_<Esi::AssetCache>(m, "AssetCache")
		.def("store", &Esi::AssetCache::store)
		.def("delete_asset", &Esi::AssetCache::deleteAsset)
		.def("delete_all_assets", &Esi::AssetCache::deleteAllAssets)
		.def("get_assets", &Esi::AssetCache::getAssets);

	/*Utility classes*/
	pybind11::class_<glm::vec2>(m, "Vector2")
		.def_readwrite("x", &glm::vec2::x)
		.def_readwrite("y", &glm::vec2::y);

	pybind11::class_<Esi::Rectangle>(m, "Rectangle")
		.def_readwrite("height", &Esi::Rectangle::height)
		.def_readwrite("width", &Esi::Rectangle::width);
	pybind11::class_<Esi::HiResTimer>(m, "HiResTimer")
		.def("set_start", &Esi::HiResTimer::setStart)
		.def("set_end", &Esi::HiResTimer::setEnd)
		.def("get_delta_ns", &Esi::HiResTimer::getDelta_ns);
	pybind11::class_<Esi::Colour>(m, "Colour")
		.def(pybind11::init<std::string>())
		.def(pybind11::init<uint8_t, uint8_t, uint8_t, uint8_t>())
		.def("get_red", &Esi::Colour::getRed)
		.def("get_green", &Esi::Colour::getGreen)
		.def("get_blue", &Esi::Colour::getBlue)
		.def("get_alpha", &Esi::Colour::getAlpha);
	pybind11::class_<Esi::AABB2D>(m, "AABB2D")
		.def_readwrite("max_px", &Esi::AABB2D::max_px)
		.def_readwrite("min_px", &Esi::AABB2D::min_px);

	/*API getters*/
	m.def("get_command_line_api", &Esi::ScriptingAPIs::getCommandLineAPI);
	m.def("get_input_api", &Esi::ScriptingAPIs::getInputAPI);
	m.def("get_log_api", &Esi::ScriptingAPIs::getLogAPI);
    m.def("get_scene_api", &Esi::ScriptingAPIs::getSceneAPI);
	m.def("get_time_api", &Esi::ScriptingAPIs::getTimeAPI);
	m.def("get_window_api", &Esi::ScriptingAPIs::getWindowAPI);
	m.def("get_data_directory_path", &Esi::ScriptingAPIs::getDataDirectoryPath);
	m.def("get_executable_directory_path", &Esi::ScriptingAPIs::getExecutableDirectoryPath);
}