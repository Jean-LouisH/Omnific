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
#include <application/scene/component_variant.hpp>
#include <pybind11/embed.h>
#include <pybind11/stl.h>
#include <glm/glm.hpp>
#include <application/scene/assets/animation.hpp>
#include <application/scene/assets/audio_stream.hpp>
#include <application/scene/assets/font.hpp>
#include <application/scene/assets/image.hpp>
#include <application/scene/assets/material.hpp>
#include <application/scene/assets/mesh.hpp>
#include <application/scene/assets/rig.hpp>
#include <application/scene/assets/shader.hpp>
#include <application/scene/assets/text.hpp>
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
		.def("get_this_entity_2d", &Esi::SceneAPI::getThisEntity, pybind11::return_value_policy::reference)
		.def("get_this_component_variant", &Esi::SceneAPI::getThisComponentVariant, pybind11::return_value_policy::reference)
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
		.def("get_component_variants", &Esi::Scene::getComponentVariants, pybind11::return_value_policy::reference)
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
	pybind11::class_<Esi::BehaviourTree>(m, "BehaviourTree");
	pybind11::class_<Esi::SightPerception>(m, "SightPerception");
	pybind11::class_<Esi::SoundPerception>(m, "SoundPerception");
	pybind11::class_<Esi::AnimatedSprite>(m, "AnimatedSprite");
	pybind11::class_<Esi::AudioListener>(m, "AudioListener");
	pybind11::class_<Esi::AudioStreamSource>(m, "AudioStreamSource");
	pybind11::class_<Esi::Camera>(m, "Camera");
	pybind11::class_<Esi::BallCollider>(m, "BallCollider");
	pybind11::class_<Esi::ConstantDirectionalForce>(m, "ConstantDirectionalForce");
	pybind11::class_<Esi::ConstantPointForce>(m, "ConstantPointForce");
	pybind11::class_<Esi::CountdownTimer>(m, "CountdownTimer")
		.def("start", &Esi::CountdownTimer::start)
		.def("stop", &Esi::CountdownTimer::stop)
		.def("is_finished", &Esi::CountdownTimer::isFinished);
	pybind11::class_<Esi::KinematicBody>(m, "KinematicBody");
	pybind11::class_<Esi::Light>(m, "Light");
	pybind11::class_<Esi::NavigationMeshAgent>(m, "NavigationMeshAgent");
	pybind11::class_<Esi::NavigationMeshBoxObstacle>(m, "NavigationMeshBoxObstacle");
	pybind11::class_<Esi::NavigationPath>(m, "NavigationPath");
	pybind11::class_<Esi::PhysicsConstraint>(m, "PhysicsConstraint");
	pybind11::class_<Esi::PhysicsThruster>(m, "PhysicsThruster");
	pybind11::class_<Esi::PropertyAnimation>(m, "PropertyAnimation");
	pybind11::class_<Esi::BoxCollider>(m, "BoxCollider");
	pybind11::class_<Esi::BoxMesh>(m, "BoxMesh");
	pybind11::class_<Esi::BoxTriggerSpace>(m, "BoxTriggerSpace");
	pybind11::class_<Esi::RegularPolytopalMesh>(m, "RegularPolytopalMesh");
	pybind11::class_<Esi::RigidBody>(m, "RigidBody");
	pybind11::class_<Esi::Sprite>(m, "Sprite");
	pybind11::class_<Esi::StaticFluid>(m, "StaticFluid");
	pybind11::class_<Esi::Transform>(m, "Transform");
	pybind11::class_<Esi::UIButton>(m, "UIButton");
	pybind11::class_<Esi::UIGraphEdit>(m, "UIGraphEdit");
	pybind11::class_<Esi::UIGraphNode>(m, "UIGraphNode");
	pybind11::class_<Esi::UIHoverCard>(m, "UIHoverCard");
	pybind11::class_<Esi::UIItemList>(m, "UIItemList");
	pybind11::class_<Esi::UIPanel>(m, "UIPanel");
	pybind11::class_<Esi::UIProgressBar>(m, "UIProgressBar");
	pybind11::class_<Esi::UIRectangle>(m, "UIRectangle");
	pybind11::class_<Esi::UIScrollbar>(m, "UIScrollbar");
	pybind11::class_<Esi::UISeparator>(m, "UISeparator");
	pybind11::class_<Esi::UISlider>(m, "UISlider");
	pybind11::class_<Esi::UISpinBox>(m, "UISpinBox");
	pybind11::class_<Esi::UITab>(m, "UITab");
	pybind11::class_<Esi::UITextEdit>(m, "UITextEdit");
	pybind11::class_<Esi::UITextLabel>(m, "UITextLabel")
		.def("set_text", &Esi::UITextLabel::setText);
	pybind11::class_<Esi::UITree>(m, "UITree");
	pybind11::class_<Esi::UIViewport>(m, "UIViewport");

	/*ComponentVariant types*/
	pybind11::class_<Esi::ComponentVariant> componentVariant(m, "CV");
	componentVariant.def("set_to", pybind11::overload_cast<Esi::BehaviourTree*>(&Esi::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Esi::SightPerception*>(&Esi::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Esi::SoundPerception*>(&Esi::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Esi::AnimatedSprite*>(&Esi::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Esi::AudioListener*>(&Esi::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Esi::AudioStreamSource*>(&Esi::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Esi::Camera*>(&Esi::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Esi::BallCollider*>(&Esi::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Esi::ConstantDirectionalForce*>(&Esi::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Esi::ConstantPointForce*>(&Esi::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Esi::CountdownTimer*>(&Esi::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Esi::KinematicBody*>(&Esi::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Esi::Light*>(&Esi::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Esi::NavigationMeshAgent*>(&Esi::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Esi::NavigationMeshBoxObstacle*>(&Esi::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Esi::NavigationPath*>(&Esi::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Esi::PhysicsConstraint*>(&Esi::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Esi::PhysicsThruster*>(&Esi::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Esi::PropertyAnimation*>(&Esi::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Esi::BoxCollider*>(&Esi::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Esi::BoxTriggerSpace*>(&Esi::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Esi::RegularPolytopalMesh*>(&Esi::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Esi::RigidBody*>(&Esi::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Esi::Sprite*>(&Esi::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Esi::StaticFluid*>(&Esi::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Esi::Transform*>(&Esi::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Esi::UIButton*>(&Esi::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Esi::UIGraphEdit*>(&Esi::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Esi::UIGraphNode*>(&Esi::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Esi::UIHoverCard*>(&Esi::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Esi::UIItemList*>(&Esi::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Esi::UIPanel*>(&Esi::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Esi::UIProgressBar*>(&Esi::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Esi::UIRectangle*>(&Esi::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Esi::UIScrollbar*>(&Esi::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Esi::UISeparator*>(&Esi::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Esi::UISlider*>(&Esi::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Esi::UISpinBox*>(&Esi::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Esi::UITab*>(&Esi::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Esi::UITextEdit*>(&Esi::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Esi::UITextLabel*>(&Esi::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Esi::UITree*>(&Esi::ComponentVariant::setTo));

	componentVariant.def("get_behaviour_tree", &Esi::ComponentVariant::getBehaviourTree, pybind11::return_value_policy::reference);
	componentVariant.def("get_sight_perception", &Esi::ComponentVariant::getSightPerception, pybind11::return_value_policy::reference);
	componentVariant.def("get_sound_perception", &Esi::ComponentVariant::getSoundPerception, pybind11::return_value_policy::reference);
	componentVariant.def("get_animated_sprite", &Esi::ComponentVariant::getAnimatedSprite, pybind11::return_value_policy::reference);
	componentVariant.def("get_audio_listener", &Esi::ComponentVariant::getAudioListener, pybind11::return_value_policy::reference);
	componentVariant.def("get_audio_stream_source", &Esi::ComponentVariant::getAudioStreamSource, pybind11::return_value_policy::reference);
	componentVariant.def("get_camera", &Esi::ComponentVariant::getCamera, pybind11::return_value_policy::reference);
	componentVariant.def("get_ball_collider", &Esi::ComponentVariant::getBallCollider, pybind11::return_value_policy::reference);
	componentVariant.def("get_constant_directional_force", &Esi::ComponentVariant::getConstantDirectionalForce, pybind11::return_value_policy::reference);
	componentVariant.def("get_constant_point_force", &Esi::ComponentVariant::getConstantPointForce, pybind11::return_value_policy::reference);
	componentVariant.def("get_countdown_timer", &Esi::ComponentVariant::getCountdownTimer, pybind11::return_value_policy::reference);
	componentVariant.def("get_kinematic_body", &Esi::ComponentVariant::getKinematicBody, pybind11::return_value_policy::reference);
	componentVariant.def("get_light", &Esi::ComponentVariant::getLight, pybind11::return_value_policy::reference);
	componentVariant.def("get_navigation_mesh_agent", &Esi::ComponentVariant::getNavigationMeshAgent, pybind11::return_value_policy::reference);
	componentVariant.def("get_navigation_mesh_box_obstacle", &Esi::ComponentVariant::getNavigationMeshBoxObstacle, pybind11::return_value_policy::reference);
	componentVariant.def("get_naviation_path", &Esi::ComponentVariant::getNavigationPath, pybind11::return_value_policy::reference);
	componentVariant.def("get_physics_constraint", &Esi::ComponentVariant::getPhysicsConstraint, pybind11::return_value_policy::reference);
	componentVariant.def("get_physics_thruster", &Esi::ComponentVariant::getPhysicsThruster, pybind11::return_value_policy::reference);
	componentVariant.def("get_property_animation", &Esi::ComponentVariant::getPropertyAnimation, pybind11::return_value_policy::reference);
	componentVariant.def("get_box_collider", &Esi::ComponentVariant::getBoxCollider, pybind11::return_value_policy::reference);
	componentVariant.def("get_box_trigger_space", &Esi::ComponentVariant::getBoxTriggerSpace, pybind11::return_value_policy::reference);
	componentVariant.def("get_regular_polytopal_mesh", &Esi::ComponentVariant::getRegularPolytopalMesh, pybind11::return_value_policy::reference);
	componentVariant.def("get_rigid_body", &Esi::ComponentVariant::getRigidBody, pybind11::return_value_policy::reference);
	componentVariant.def("get_sprite", &Esi::ComponentVariant::getSprite, pybind11::return_value_policy::reference);
	componentVariant.def("get_static_fluid", &Esi::ComponentVariant::getStaticFluid, pybind11::return_value_policy::reference);
	componentVariant.def("get_transform", &Esi::ComponentVariant::getTransform, pybind11::return_value_policy::reference);
	componentVariant.def("get_ui_button", &Esi::ComponentVariant::getUIButton, pybind11::return_value_policy::reference);
	componentVariant.def("get_ui_graph_edit", &Esi::ComponentVariant::getUIGraphEdit, pybind11::return_value_policy::reference);
	componentVariant.def("get_ui_graph_node", &Esi::ComponentVariant::getUIGraphNode, pybind11::return_value_policy::reference);
	componentVariant.def("get_ui_hover_card", &Esi::ComponentVariant::getUIHoverCard, pybind11::return_value_policy::reference);
	componentVariant.def("get_ui_item_list", &Esi::ComponentVariant::getUIItemList, pybind11::return_value_policy::reference);
	componentVariant.def("get_ui_panel", &Esi::ComponentVariant::getUIPanel, pybind11::return_value_policy::reference);;
	componentVariant.def("get_ui_progress_bar", &Esi::ComponentVariant::getUIProgressBar, pybind11::return_value_policy::reference);
	componentVariant.def("get_ui_rectangle", &Esi::ComponentVariant::getUIRectangle, pybind11::return_value_policy::reference);
	componentVariant.def("get_ui_scrollbar", &Esi::ComponentVariant::getUIScrollbar, pybind11::return_value_policy::reference);
	componentVariant.def("get_ui_separator", &Esi::ComponentVariant::getUISeparator, pybind11::return_value_policy::reference);
	componentVariant.def("get_ui_slider", &Esi::ComponentVariant::getUISlider, pybind11::return_value_policy::reference);
	componentVariant.def("get_ui_spin_box", &Esi::ComponentVariant::getUISpinBox, pybind11::return_value_policy::reference);
	componentVariant.def("get_ui_tab", &Esi::ComponentVariant::getUITab, pybind11::return_value_policy::reference);
	componentVariant.def("get_ui_text_edit", &Esi::ComponentVariant::getUITextEdit, pybind11::return_value_policy::reference);
	componentVariant.def("get_ui_text_label", &Esi::ComponentVariant::getUITextLabel, pybind11::return_value_policy::reference);
	componentVariant.def("get_ui_tree", &Esi::ComponentVariant::getUITree, pybind11::return_value_policy::reference);
	componentVariant.def("get_ui_viewport", &Esi::ComponentVariant::getUIViewport, pybind11::return_value_policy::reference);

	pybind11::enum_<Esi::ComponentVariant::Type>(componentVariant, "Type")
		.value("NONE", Esi::ComponentVariant::Type::NONE)
		.value("BEHAVIOUR_TREE", Esi::ComponentVariant::Type::BEHAVIOUR_TREE)
		.value("SIGHT_PERCEPTION", Esi::ComponentVariant::Type::SIGHT_PERCEPTION)
		.value("SOUND_PERCEPTION", Esi::ComponentVariant::Type::SOUND_PERCEPTION)
		.value("ANIMATED_SPRITE", Esi::ComponentVariant::Type::ANIMATED_SPRITE)
		.value("AUDIO_LISTENER", Esi::ComponentVariant::Type::AUDIO_LISTENER)
		.value("AUDIO_STREAM_SOURCE", Esi::ComponentVariant::Type::AUDIO_STREAM_SOURCE)
		.value("CAMERA", Esi::ComponentVariant::Type::CAMERA)
		.value("BALL_COLLIDER", Esi::ComponentVariant::Type::BALL_COLLIDER)
		.value("CONSTANT_DIRECTIONAL_FORCE", Esi::ComponentVariant::Type::CONSTANT_DIRECTIONAL_FORCE)
		.value("CONSTANT_POINT_FORCE", Esi::ComponentVariant::Type::CONSTANT_POINT_FORCE)
		.value("COUNTDOWN_TIMER", Esi::ComponentVariant::Type::COUNTDOWN_TIMER)
		.value("KINEMATIC_BODY", Esi::ComponentVariant::Type::KINEMATIC_BODY)
		.value("LIGHT", Esi::ComponentVariant::Type::LIGHT)
		.value("NAVIGATION_MESH", Esi::ComponentVariant::Type::NAVIGATION_MESH)
		.value("NAVIGATION_MESH_AGENT", Esi::ComponentVariant::Type::NAVIGATION_MESH_AGENT)
		.value("NAVIGATION_MESH_BOX_OBSTACLE", Esi::ComponentVariant::Type::NAVIGATION_MESH_BOX_OBSTACLE)
		.value("NAVIGATION_PATH", Esi::ComponentVariant::Type::NAVIGATION_PATH)
		.value("PHYSICS_CONSTRAINT", Esi::ComponentVariant::Type::PHYSICS_CONSTRAINT)
		.value("PHYSICS_THRUSTER", Esi::ComponentVariant::Type::PHYSICS_THRUSTER)
		.value("PROPERTY_ANIMATION", Esi::ComponentVariant::Type::PROPERTY_ANIMATION)
		.value("BOX_COLIIDER", Esi::ComponentVariant::Type::BOX_COLLIDER)
		.value("BOX_MESH", Esi::ComponentVariant::Type::BOX_MESH)
		.value("BOX_TRIGGER_SPACE", Esi::ComponentVariant::Type::BOX_TRIGGER_SPACE)
		.value("REGULAR_POLYTOPAL_MESH", Esi::ComponentVariant::Type::REGULAR_POLYTOPAL_MESH)
		.value("RIGID_BODY", Esi::ComponentVariant::Type::RIGID_BODY)
		.value("SPRITE", Esi::ComponentVariant::Type::SPRITE)
		.value("STATIC_FLUID", Esi::ComponentVariant::Type::STATIC_FLUID)
		.value("TRANSFORM", Esi::ComponentVariant::Type::TRANSFORM)
		.value("UI_BUTTON", Esi::ComponentVariant::Type::UI_BUTTON)
		.value("UI_RECTANGLE", Esi::ComponentVariant::Type::UI_RECTANGLE)
		.value("UI_GRAPH_EDIT", Esi::ComponentVariant::Type::UI_GRAPH_EDIT)
		.value("UI_GRAPH_NODE", Esi::ComponentVariant::Type::UI_GRAPH_NODE)
		.value("UI_SCROLLBAR", Esi::ComponentVariant::Type::UI_SCROLLBAR)
		.value("UI_SEPARATOR", Esi::ComponentVariant::Type::UI_SEPARATOR)
		.value("UI_SLIDER", Esi::ComponentVariant::Type::UI_SLIDER)
		.value("UI_HOVER_CARD", Esi::ComponentVariant::Type::UI_HOVER_CARD)
		.value("UI_ITEM_LIST", Esi::ComponentVariant::Type::UI_ITEM_LIST)
		.value("UI_PANEL", Esi::ComponentVariant::Type::UI_PANEL)
		.value("UI_PROGRESS_BAR", Esi::ComponentVariant::Type::UI_PROGRESS_BAR)
		.value("UI_SPIN_BOX", Esi::ComponentVariant::Type::UI_SPIN_BOX)
		.value("UI_TAB", Esi::ComponentVariant::Type::UI_TAB)
		.value("UI_TEXT_EDIT", Esi::ComponentVariant::Type::UI_TEXT_EDIT)
		.value("UI_TEXT_LABEL", Esi::ComponentVariant::Type::UI_TEXT_LABEL)
		.value("UI_TREE", Esi::ComponentVariant::Type::UI_TREE)
		.value("UI_VIEWPORT", Esi::ComponentVariant::Type::UI_VIEWPORT)
		.export_values();

	/*Asset classes*/

	pybind11::class_<Esi::AudioStream>(m, "AudioStream");
	pybind11::class_<Esi::Font>(m, "Font");
	pybind11::class_<Esi::Image>(m, "Image");
	pybind11::class_<Esi::Text>(m, "Text");

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