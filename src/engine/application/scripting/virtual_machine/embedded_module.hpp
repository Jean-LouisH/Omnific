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
#include <utilities/rectangle.hpp>
#include <utilities/hi_res_timer.hpp>
#include <utilities/colour.hpp>
#include <utilities/aabb_2d.hpp>
#include <utilities/aliases.hpp>

PYBIND11_EMBEDDED_MODULE(lilliputian, m) 
{
	/*API classes*/
	pybind11::class_<Lilliputian::CommandLineAPI>(m, "CommandLineAPI")
		.def("open_window", &Lilliputian::CommandLineAPI::openWindow)
		.def("close_window", &Lilliputian::CommandLineAPI::closeWindow);

	pybind11::class_<Lilliputian::InputAPI>(m, "InputAPI")
		.def("is_on_press", pybind11::overload_cast<std::string>(&Lilliputian::InputAPI::isOnPress))
		.def("is_on_press", pybind11::overload_cast<std::vector<std::string>>(&Lilliputian::InputAPI::isOnPress))
		.def("is_on_press", pybind11::overload_cast<std::vector<std::string>, Lilliputian::PlayerID>(&Lilliputian::InputAPI::isOnPress))
		.def("is_on_double_press", pybind11::overload_cast<std::string, unsigned int>(&Lilliputian::InputAPI::isOnDoublePress))
		.def("is_on_double_press", pybind11::overload_cast<std::vector<std::string>, unsigned int>(&Lilliputian::InputAPI::isOnDoublePress))
		.def("is_on_double_press", pybind11::overload_cast<std::vector<std::string>, unsigned int>(&Lilliputian::InputAPI::isOnDoublePress))
		.def("is_pressed", pybind11::overload_cast<std::string>(&Lilliputian::InputAPI::isPressed))
		.def("is_pressed", pybind11::overload_cast<std::vector<std::string>>(&Lilliputian::InputAPI::isPressed))
		.def("is_pressed", pybind11::overload_cast<std::vector<std::string>, Lilliputian::PlayerID>(&Lilliputian::InputAPI::isPressed))
		.def("is_on_release", pybind11::overload_cast<std::string>(&Lilliputian::InputAPI::isOnRelease))
		.def("is_on_release", pybind11::overload_cast<std::vector<std::string>>(&Lilliputian::InputAPI::isOnRelease))
		.def("is_on_release", pybind11::overload_cast<std::vector<std::string>, Lilliputian::PlayerID>(&Lilliputian::InputAPI::isOnRelease))
		.def("is_released", pybind11::overload_cast<std::string>(&Lilliputian::InputAPI::isReleased))
		.def("is_released", pybind11::overload_cast<std::string, Lilliputian::PlayerID>(&Lilliputian::InputAPI::isReleased))
		.def("is_left_mouse_button_on_press", &Lilliputian::InputAPI::isLeftMouseButtonOnPress)
		.def("is_left_mouse_button_on_release", &Lilliputian::InputAPI::isLeftMouseButtonOnRelease)
		.def("is_left_mouse_button_double_clicked", &Lilliputian::InputAPI::isLeftMouseButtonDoubleClicked)
		.def("is_middle_mouse_button_on_press", &Lilliputian::InputAPI::isMiddleMouseButtonOnPress)
		.def("is_middle_mouse_button_on_release", &Lilliputian::InputAPI::isMiddleMouseButtonOnRelease)
		.def("is_middle_mouse_button_double_clicked", &Lilliputian::InputAPI::isMiddleMouseButtonDoubleClicked)
		.def("is_right_mouse_button_on_press", &Lilliputian::InputAPI::isRightMouseButtonOnPress)
		.def("is_right_mouse_button_on_release", &Lilliputian::InputAPI::isRightMouseButtonOnRelease)
		.def("is_right_mouse_button_double_clicked", &Lilliputian::InputAPI::isRightMouseButtonDoubleClicked)
		.def("get_mouse_position", &Lilliputian::InputAPI::getMousePosition)
		.def("get_mouse_wheel_velocity", &Lilliputian::InputAPI::getMouseWheelVelocity)
		.def("get_mouse_motion_velocity", &Lilliputian::InputAPI::getMouseMotionVelocity);

	pybind11::class_<Lilliputian::LogAPI>(m, "LogAPI")
		.def("write", &Lilliputian::LogAPI::write)
		.def("write_to_file", &Lilliputian::LogAPI::writeToFile)
		.def("get_last_message", &Lilliputian::LogAPI::getLastMessage)
		.def("get_logs", &Lilliputian::LogAPI::getLogs);

	pybind11::class_<Lilliputian::SceneAPI>(m, "SceneAPI")
		.def("this_has_component", &Lilliputian::SceneAPI::thisHasComponent)
		.def("preload_scene", &Lilliputian::SceneAPI::preloadScene)
		.def("load_scene", &Lilliputian::SceneAPI::loadScene)
		.def("unload_scene", &Lilliputian::SceneAPI::unloadScene)
		.def("change_to_scene", &Lilliputian::SceneAPI::changeToScene)
		.def("get_this_entity_2d", &Lilliputian::SceneAPI::getThisEntity, pybind11::return_value_policy::reference)
		.def("get_this_component_variant", &Lilliputian::SceneAPI::getThisComponentVariant, pybind11::return_value_policy::reference)
		.def("get_this_scene", &Lilliputian::SceneAPI::getThisScene, pybind11::return_value_policy::reference);

	pybind11::class_<Lilliputian::TimeAPI>(m, "TimeAPI")
		.def("set_ms_per_compute_update", &Lilliputian::TimeAPI::setMsPerComputeUpdate)
		.def("set_target_fps", &Lilliputian::TimeAPI::setTargetFPS)
		.def("get_ms_per_compute_update", &Lilliputian::TimeAPI::setMsPerComputeUpdate)
		.def("get_target_fps", &Lilliputian::TimeAPI::setTargetFPS)
		.def("get_frame_time_delta", &Lilliputian::TimeAPI::getFrameTimeDelta);

    pybind11::class_<Lilliputian::WindowAPI>(m, "WindowAPI")
		.def("set_to_windowed", &Lilliputian::WindowAPI::setToWindowed)
		.def("set_to_fullscreen", &Lilliputian::WindowAPI::setToFullscreen)
        .def("toggle_windowed_fullscreen", &Lilliputian::WindowAPI::toggleWindowedFullscreen)
		.def("resize", &Lilliputian::WindowAPI::resize)
		.def("change_title", &Lilliputian::WindowAPI::changeTitle)
		.def("change_icon", &Lilliputian::WindowAPI::changeIcon)
		.def("maximize", &Lilliputian::WindowAPI::maximize)
		.def("minimize", &Lilliputian::WindowAPI::minimize)
		.def("raise", &Lilliputian::WindowAPI::raise)
		.def("restore", &Lilliputian::WindowAPI::restore)
		.def("hide", &Lilliputian::WindowAPI::hide)
		.def("show", &Lilliputian::WindowAPI::show);

	/*Scene classes*/

	pybind11::class_<Lilliputian::Entity>(m, "Entity")
		.def_readonly("ID", &Lilliputian::Entity::ID)
		.def_readwrite("name", &Lilliputian::Entity::name)
		.def_readwrite("parentID", &Lilliputian::Entity::parentID)
		.def_readwrite("childIDs", &Lilliputian::Entity::childIDs)
		.def_readwrite("tags", &Lilliputian::Entity::tags);

	pybind11::class_<Lilliputian::Scene>(m, "Scene")
		.def("get_asset_cache", &Lilliputian::Scene::getAssetCache, pybind11::return_value_policy::reference)	
		.def("add_entity", &Lilliputian::Scene::addEntity)
		.def("add_empty_entity", &Lilliputian::Scene::addEmptyEntity)
		.def("add_component", &Lilliputian::Scene::addComponent)
		.def("add_component_to_last_entity", &Lilliputian::Scene::addComponentToLastEntity)
		.def("remove_entity", &Lilliputian::Scene::removeEntity)
		.def("remove_component", &Lilliputian::Scene::removeComponent)
		.def("get_component_variants", &Lilliputian::Scene::getComponentVariants, pybind11::return_value_policy::reference)
		.def("get_entity_transform", &Lilliputian::Scene::getEntityTransform, pybind11::return_value_policy::reference)
		.def("get_entity", &Lilliputian::Scene::getEntity, pybind11::return_value_policy::reference)
		.def("get_entity_by_name", &Lilliputian::Scene::getEntityByName, pybind11::return_value_policy::reference)
		.def("get_last_entity", &Lilliputian::Scene::getLastEntity, pybind11::return_value_policy::reference)
		.def("get_entities", &Lilliputian::Scene::getEntities, pybind11::return_value_policy::reference)
		.def("get_event_bus", &Lilliputian::Scene::getEventBus, pybind11::return_value_policy::reference)
		.def("get_haptic_signal_buffer", &Lilliputian::Scene::getHapticSignalBuffer, pybind11::return_value_policy::reference)
		.def("get_id", &Lilliputian::Scene::getID);

	pybind11::class_<Lilliputian::Event::Parameters>(m, "EventParameters")
		.def_readwrite("floats", &Lilliputian::Event::Parameters::floats)
		.def_readwrite("strings", &Lilliputian::Event::Parameters::strings);

	pybind11::class_<Lilliputian::Event>(m, "Event")
		.def(pybind11::init<std::string, uint64_t, Lilliputian::Event::Parameters>())
		.def(pybind11::init<std::string, uint64_t>())
		.def("get_name", &Lilliputian::Event::getName)
		.def("get_parameters", &Lilliputian::Event::getParameters)
		.def("get_timestamp", &Lilliputian::Event::getTimestamp);

	pybind11::class_<Lilliputian::EventBus>(m, "EventBus")
		.def("clear", &Lilliputian::EventBus::clear)
		.def("query", &Lilliputian::EventBus::query)
		.def("publish", pybind11::overload_cast<std::string>(&Lilliputian::EventBus::publish))
		.def("publish", pybind11::overload_cast<std::string, std::vector<float>>(&Lilliputian::EventBus::publish))
		.def("publish", pybind11::overload_cast<std::string, std::vector<std::string>>(&Lilliputian::EventBus::publish))
		.def("publish", pybind11::overload_cast<std::string, std::vector<float>, std::vector<std::string>>(&Lilliputian::EventBus::publish));

	pybind11::class_<Lilliputian::HapticSignal>(m, "HapticSignal")
		.def(pybind11::init<Lilliputian::PlayerID, float, uint16_t>())
		.def("get_duration_ms", &Lilliputian::HapticSignal::getDuration_ms)
		.def("get_player_id", &Lilliputian::HapticSignal::getPlayerID)
		.def("get_strength_pct", &Lilliputian::HapticSignal::getStrength_pct);

	pybind11::class_<Lilliputian::HapticSignalBuffer>(m, "HapticSignalBuffer")
		.def("publish", &Lilliputian::HapticSignalBuffer::publish)
		.def("query", &Lilliputian::HapticSignalBuffer::query)
		.def("get_haptic_signals", &Lilliputian::HapticSignalBuffer::getHapticSignals)
		.def("clear", &Lilliputian::HapticSignalBuffer::clear);

	/*Component classes*/
	pybind11::class_<Lilliputian::BehaviourTree>(m, "BehaviourTree");
	pybind11::class_<Lilliputian::SightPerception>(m, "SightPerception");
	pybind11::class_<Lilliputian::SoundPerception>(m, "SoundPerception");
	pybind11::class_<Lilliputian::AnimatedSprite>(m, "AnimatedSprite");
	pybind11::class_<Lilliputian::AudioListener>(m, "AudioListener");
	pybind11::class_<Lilliputian::AudioStreamSource>(m, "AudioStreamSource");
	pybind11::class_<Lilliputian::Camera>(m, "Camera");
	pybind11::class_<Lilliputian::BallCollider>(m, "BallCollider");
	pybind11::class_<Lilliputian::ConstantDirectionalForce>(m, "ConstantDirectionalForce");
	pybind11::class_<Lilliputian::ConstantPointForce>(m, "ConstantPointForce");
	pybind11::class_<Lilliputian::CountdownTimer>(m, "CountdownTimer")
		.def("start", &Lilliputian::CountdownTimer::start)
		.def("stop", &Lilliputian::CountdownTimer::stop)
		.def("is_finished", &Lilliputian::CountdownTimer::isFinished);
	pybind11::class_<Lilliputian::KinematicBody>(m, "KinematicBody");
	pybind11::class_<Lilliputian::Light>(m, "Light");
	pybind11::class_<Lilliputian::NavigationMeshAgent>(m, "NavigationMeshAgent");
	pybind11::class_<Lilliputian::NavigationMeshBoxObstacle>(m, "NavigationMeshBoxObstacle");
	pybind11::class_<Lilliputian::NavigationPath>(m, "NavigationPath");
	pybind11::class_<Lilliputian::PhysicsConstraint>(m, "PhysicsConstraint");
	pybind11::class_<Lilliputian::PhysicsThruster>(m, "PhysicsThruster");
	pybind11::class_<Lilliputian::PropertyAnimation>(m, "PropertyAnimation");
	pybind11::class_<Lilliputian::BoxCollider>(m, "BoxCollider");
	pybind11::class_<Lilliputian::BoxMesh>(m, "BoxMesh");
	pybind11::class_<Lilliputian::BoxTriggerSpace>(m, "BoxTriggerSpace");
	pybind11::class_<Lilliputian::RegularPolytopalMesh>(m, "RegularPolytopalMesh");
	pybind11::class_<Lilliputian::RigidBody>(m, "RigidBody");
	pybind11::class_<Lilliputian::Sprite>(m, "Sprite");
	pybind11::class_<Lilliputian::StaticFluid>(m, "StaticFluid");
	pybind11::class_<Lilliputian::Transform>(m, "Transform");
	pybind11::class_<Lilliputian::UIButton>(m, "UIButton");
	pybind11::class_<Lilliputian::UIGraphEdit>(m, "UIGraphEdit");
	pybind11::class_<Lilliputian::UIGraphNode>(m, "UIGraphNode");
	pybind11::class_<Lilliputian::UIHoverCard>(m, "UIHoverCard");
	pybind11::class_<Lilliputian::UIItemList>(m, "UIItemList");
	pybind11::class_<Lilliputian::UIPanel>(m, "UIPanel");
	pybind11::class_<Lilliputian::UIProgressBar>(m, "UIProgressBar");
	pybind11::class_<Lilliputian::UIRectangle>(m, "UIRectangle");
	pybind11::class_<Lilliputian::UIScrollbar>(m, "UIScrollbar");
	pybind11::class_<Lilliputian::UISeparator>(m, "UISeparator");
	pybind11::class_<Lilliputian::UISlider>(m, "UISlider");
	pybind11::class_<Lilliputian::UISpinBox>(m, "UISpinBox");
	pybind11::class_<Lilliputian::UITab>(m, "UITab");
	pybind11::class_<Lilliputian::UITextEdit>(m, "UITextEdit");
	pybind11::class_<Lilliputian::UITextLabel>(m, "UITextLabel")
		.def("set_text", &Lilliputian::UITextLabel::setText);
	pybind11::class_<Lilliputian::UITree>(m, "UITree");
	pybind11::class_<Lilliputian::UIViewport>(m, "UIViewport");

	/*ComponentVariant types*/
	pybind11::class_<Lilliputian::ComponentVariant> componentVariant(m, "CV");
	componentVariant.def("set_to", pybind11::overload_cast<Lilliputian::BehaviourTree*>(&Lilliputian::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Lilliputian::SightPerception*>(&Lilliputian::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Lilliputian::SoundPerception*>(&Lilliputian::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Lilliputian::AnimatedSprite*>(&Lilliputian::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Lilliputian::AudioListener*>(&Lilliputian::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Lilliputian::AudioStreamSource*>(&Lilliputian::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Lilliputian::Camera*>(&Lilliputian::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Lilliputian::BallCollider*>(&Lilliputian::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Lilliputian::ConstantDirectionalForce*>(&Lilliputian::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Lilliputian::ConstantPointForce*>(&Lilliputian::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Lilliputian::CountdownTimer*>(&Lilliputian::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Lilliputian::KinematicBody*>(&Lilliputian::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Lilliputian::Light*>(&Lilliputian::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Lilliputian::NavigationMeshAgent*>(&Lilliputian::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Lilliputian::NavigationMeshBoxObstacle*>(&Lilliputian::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Lilliputian::NavigationPath*>(&Lilliputian::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Lilliputian::PhysicsConstraint*>(&Lilliputian::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Lilliputian::PhysicsThruster*>(&Lilliputian::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Lilliputian::PropertyAnimation*>(&Lilliputian::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Lilliputian::BoxCollider*>(&Lilliputian::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Lilliputian::BoxTriggerSpace*>(&Lilliputian::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Lilliputian::RegularPolytopalMesh*>(&Lilliputian::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Lilliputian::RigidBody*>(&Lilliputian::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Lilliputian::Sprite*>(&Lilliputian::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Lilliputian::StaticFluid*>(&Lilliputian::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Lilliputian::Transform*>(&Lilliputian::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Lilliputian::UIButton*>(&Lilliputian::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Lilliputian::UIGraphEdit*>(&Lilliputian::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Lilliputian::UIGraphNode*>(&Lilliputian::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Lilliputian::UIHoverCard*>(&Lilliputian::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Lilliputian::UIItemList*>(&Lilliputian::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Lilliputian::UIPanel*>(&Lilliputian::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Lilliputian::UIProgressBar*>(&Lilliputian::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Lilliputian::UIRectangle*>(&Lilliputian::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Lilliputian::UIScrollbar*>(&Lilliputian::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Lilliputian::UISeparator*>(&Lilliputian::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Lilliputian::UISlider*>(&Lilliputian::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Lilliputian::UISpinBox*>(&Lilliputian::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Lilliputian::UITab*>(&Lilliputian::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Lilliputian::UITextEdit*>(&Lilliputian::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Lilliputian::UITextLabel*>(&Lilliputian::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Lilliputian::UITree*>(&Lilliputian::ComponentVariant::setTo));

	componentVariant.def("get_behaviour_tree", &Lilliputian::ComponentVariant::getBehaviourTree, pybind11::return_value_policy::reference);
	componentVariant.def("get_sight_perception", &Lilliputian::ComponentVariant::getSightPerception, pybind11::return_value_policy::reference);
	componentVariant.def("get_sound_perception", &Lilliputian::ComponentVariant::getSoundPerception, pybind11::return_value_policy::reference);
	componentVariant.def("get_animated_sprite", &Lilliputian::ComponentVariant::getAnimatedSprite, pybind11::return_value_policy::reference);
	componentVariant.def("get_audio_listener", &Lilliputian::ComponentVariant::getAudioListener, pybind11::return_value_policy::reference);
	componentVariant.def("get_audio_stream_source", &Lilliputian::ComponentVariant::getAudioStreamSource, pybind11::return_value_policy::reference);
	componentVariant.def("get_camera", &Lilliputian::ComponentVariant::getCamera, pybind11::return_value_policy::reference);
	componentVariant.def("get_ball_collider", &Lilliputian::ComponentVariant::getBallCollider, pybind11::return_value_policy::reference);
	componentVariant.def("get_constant_directional_force", &Lilliputian::ComponentVariant::getConstantDirectionalForce, pybind11::return_value_policy::reference);
	componentVariant.def("get_constant_point_force", &Lilliputian::ComponentVariant::getConstantPointForce, pybind11::return_value_policy::reference);
	componentVariant.def("get_countdown_timer", &Lilliputian::ComponentVariant::getCountdownTimer, pybind11::return_value_policy::reference);
	componentVariant.def("get_kinematic_body", &Lilliputian::ComponentVariant::getKinematicBody, pybind11::return_value_policy::reference);
	componentVariant.def("get_light", &Lilliputian::ComponentVariant::getLight, pybind11::return_value_policy::reference);
	componentVariant.def("get_navigation_mesh_agent", &Lilliputian::ComponentVariant::getNavigationMeshAgent, pybind11::return_value_policy::reference);
	componentVariant.def("get_navigation_mesh_box_obstacle", &Lilliputian::ComponentVariant::getNavigationMeshBoxObstacle, pybind11::return_value_policy::reference);
	componentVariant.def("get_naviation_path", &Lilliputian::ComponentVariant::getNavigationPath, pybind11::return_value_policy::reference);
	componentVariant.def("get_physics_constraint", &Lilliputian::ComponentVariant::getPhysicsConstraint, pybind11::return_value_policy::reference);
	componentVariant.def("get_physics_thruster", &Lilliputian::ComponentVariant::getPhysicsThruster, pybind11::return_value_policy::reference);
	componentVariant.def("get_property_animation", &Lilliputian::ComponentVariant::getPropertyAnimation, pybind11::return_value_policy::reference);
	componentVariant.def("get_box_collider", &Lilliputian::ComponentVariant::getBoxCollider, pybind11::return_value_policy::reference);
	componentVariant.def("get_box_trigger_space", &Lilliputian::ComponentVariant::getBoxTriggerSpace, pybind11::return_value_policy::reference);
	componentVariant.def("get_regular_polytopal_mesh", &Lilliputian::ComponentVariant::getRegularPolytopalMesh, pybind11::return_value_policy::reference);
	componentVariant.def("get_rigid_body", &Lilliputian::ComponentVariant::getRigidBody, pybind11::return_value_policy::reference);
	componentVariant.def("get_sprite", &Lilliputian::ComponentVariant::getSprite, pybind11::return_value_policy::reference);
	componentVariant.def("get_static_fluid", &Lilliputian::ComponentVariant::getStaticFluid, pybind11::return_value_policy::reference);
	componentVariant.def("get_transform", &Lilliputian::ComponentVariant::getTransform, pybind11::return_value_policy::reference);
	componentVariant.def("get_ui_button", &Lilliputian::ComponentVariant::getUIButton, pybind11::return_value_policy::reference);
	componentVariant.def("get_ui_graph_edit", &Lilliputian::ComponentVariant::getUIGraphEdit, pybind11::return_value_policy::reference);
	componentVariant.def("get_ui_graph_node", &Lilliputian::ComponentVariant::getUIGraphNode, pybind11::return_value_policy::reference);
	componentVariant.def("get_ui_hover_card", &Lilliputian::ComponentVariant::getUIHoverCard, pybind11::return_value_policy::reference);
	componentVariant.def("get_ui_item_list", &Lilliputian::ComponentVariant::getUIItemList, pybind11::return_value_policy::reference);
	componentVariant.def("get_ui_panel", &Lilliputian::ComponentVariant::getUIPanel, pybind11::return_value_policy::reference);;
	componentVariant.def("get_ui_progress_bar", &Lilliputian::ComponentVariant::getUIProgressBar, pybind11::return_value_policy::reference);
	componentVariant.def("get_ui_rectangle", &Lilliputian::ComponentVariant::getUIRectangle, pybind11::return_value_policy::reference);
	componentVariant.def("get_ui_scrollbar", &Lilliputian::ComponentVariant::getUIScrollbar, pybind11::return_value_policy::reference);
	componentVariant.def("get_ui_separator", &Lilliputian::ComponentVariant::getUISeparator, pybind11::return_value_policy::reference);
	componentVariant.def("get_ui_slider", &Lilliputian::ComponentVariant::getUISlider, pybind11::return_value_policy::reference);
	componentVariant.def("get_ui_spin_box", &Lilliputian::ComponentVariant::getUISpinBox, pybind11::return_value_policy::reference);
	componentVariant.def("get_ui_tab", &Lilliputian::ComponentVariant::getUITab, pybind11::return_value_policy::reference);
	componentVariant.def("get_ui_text_edit", &Lilliputian::ComponentVariant::getUITextEdit, pybind11::return_value_policy::reference);
	componentVariant.def("get_ui_text_label", &Lilliputian::ComponentVariant::getUITextLabel, pybind11::return_value_policy::reference);
	componentVariant.def("get_ui_tree", &Lilliputian::ComponentVariant::getUITree, pybind11::return_value_policy::reference);
	componentVariant.def("get_ui_viewport", &Lilliputian::ComponentVariant::getUIViewport, pybind11::return_value_policy::reference);

	pybind11::enum_<Lilliputian::ComponentVariant::Type>(componentVariant, "Type")
		.value("NONE", Lilliputian::ComponentVariant::Type::NONE)
		.value("BEHAVIOUR_TREE", Lilliputian::ComponentVariant::Type::BEHAVIOUR_TREE)
		.value("SIGHT_PERCEPTION", Lilliputian::ComponentVariant::Type::SIGHT_PERCEPTION)
		.value("SOUND_PERCEPTION", Lilliputian::ComponentVariant::Type::SOUND_PERCEPTION)
		.value("ANIMATED_SPRITE", Lilliputian::ComponentVariant::Type::ANIMATED_SPRITE)
		.value("AUDIO_LISTENER", Lilliputian::ComponentVariant::Type::AUDIO_LISTENER)
		.value("AUDIO_STREAM_SOURCE", Lilliputian::ComponentVariant::Type::AUDIO_STREAM_SOURCE)
		.value("CAMERA", Lilliputian::ComponentVariant::Type::CAMERA)
		.value("BALL_COLLIDER", Lilliputian::ComponentVariant::Type::BALL_COLLIDER)
		.value("CONSTANT_DIRECTIONAL_FORCE", Lilliputian::ComponentVariant::Type::CONSTANT_DIRECTIONAL_FORCE)
		.value("CONSTANT_POINT_FORCE", Lilliputian::ComponentVariant::Type::CONSTANT_POINT_FORCE)
		.value("COUNTDOWN_TIMER", Lilliputian::ComponentVariant::Type::COUNTDOWN_TIMER)
		.value("KINEMATIC_BODY", Lilliputian::ComponentVariant::Type::KINEMATIC_BODY)
		.value("LIGHT", Lilliputian::ComponentVariant::Type::LIGHT)
		.value("NAVIGATION_MESH", Lilliputian::ComponentVariant::Type::NAVIGATION_MESH)
		.value("NAVIGATION_MESH_AGENT", Lilliputian::ComponentVariant::Type::NAVIGATION_MESH_AGENT)
		.value("NAVIGATION_MESH_BOX_OBSTACLE", Lilliputian::ComponentVariant::Type::NAVIGATION_MESH_BOX_OBSTACLE)
		.value("NAVIGATION_PATH", Lilliputian::ComponentVariant::Type::NAVIGATION_PATH)
		.value("PHYSICS_CONSTRAINT", Lilliputian::ComponentVariant::Type::PHYSICS_CONSTRAINT)
		.value("PHYSICS_THRUSTER", Lilliputian::ComponentVariant::Type::PHYSICS_THRUSTER)
		.value("PROPERTY_ANIMATION", Lilliputian::ComponentVariant::Type::PROPERTY_ANIMATION)
		.value("BOX_COLIIDER", Lilliputian::ComponentVariant::Type::BOX_COLLIDER)
		.value("BOX_MESH", Lilliputian::ComponentVariant::Type::BOX_MESH)
		.value("BOX_TRIGGER_SPACE", Lilliputian::ComponentVariant::Type::BOX_TRIGGER_SPACE)
		.value("REGULAR_POLYTOPAL_MESH", Lilliputian::ComponentVariant::Type::REGULAR_POLYTOPAL_MESH)
		.value("RIGID_BODY", Lilliputian::ComponentVariant::Type::RIGID_BODY)
		.value("SPRITE", Lilliputian::ComponentVariant::Type::SPRITE)
		.value("STATIC_FLUID", Lilliputian::ComponentVariant::Type::STATIC_FLUID)
		.value("TRANSFORM", Lilliputian::ComponentVariant::Type::TRANSFORM)
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
		.value("UI_TREE", Lilliputian::ComponentVariant::Type::UI_TREE)
		.value("UI_VIEWPORT", Lilliputian::ComponentVariant::Type::UI_VIEWPORT)
		.export_values();

	/*Asset classes*/

	pybind11::class_<Lilliputian::AudioStream>(m, "AudioStream");
	pybind11::class_<Lilliputian::Font>(m, "Font");
	pybind11::class_<Lilliputian::Image>(m, "Image");
	pybind11::class_<Lilliputian::Text>(m, "Text");

	pybind11::class_<Lilliputian::AssetCache>(m, "AssetCache")
		.def("load_audio_stream", &Lilliputian::AssetCache::loadAudioStream)
		.def("load_font", &Lilliputian::AssetCache::loadFont)
		.def("load_text", &Lilliputian::AssetCache::loadText)
		.def("load_image", &Lilliputian::AssetCache::loadImage)
		.def("delete_audio_stream", &Lilliputian::AssetCache::deleteAudioStream)
		.def("delete_font", &Lilliputian::AssetCache::deleteFont)
		.def("delete_text", &Lilliputian::AssetCache::deleteText)
		.def("delete_image", &Lilliputian::AssetCache::deleteImage);

	/*Utility classes*/
	pybind11::class_<glm::vec2>(m, "Vector2")
		.def_readwrite("x", &glm::vec2::x)
		.def_readwrite("y", &glm::vec2::y);

	pybind11::class_<Lilliputian::Rectangle>(m, "Rectangle")
		.def_readwrite("height", &Lilliputian::Rectangle::height)
		.def_readwrite("width", &Lilliputian::Rectangle::width);
	pybind11::class_<Lilliputian::HiResTimer>(m, "HiResTimer")
		.def("set_start", &Lilliputian::HiResTimer::setStart)
		.def("set_end", &Lilliputian::HiResTimer::setEnd)
		.def("get_delta_ns", &Lilliputian::HiResTimer::getDelta_ns);
	pybind11::class_<Lilliputian::Colour>(m, "Colour")
		.def(pybind11::init<std::string>())
		.def(pybind11::init<uint8_t, uint8_t, uint8_t, uint8_t>())
		.def("get_red", &Lilliputian::Colour::getRed)
		.def("get_green", &Lilliputian::Colour::getGreen)
		.def("get_blue", &Lilliputian::Colour::getBlue)
		.def("get_alpha", &Lilliputian::Colour::getAlpha);
	pybind11::class_<Lilliputian::AABB2D>(m, "AABB2D")
		.def_readwrite("max_px", &Lilliputian::AABB2D::max_px)
		.def_readwrite("min_px", &Lilliputian::AABB2D::min_px);

	/*API getters*/
	m.def("get_command_line_api", &Lilliputian::ScriptingAPIs::getCommandLineAPI);
	m.def("get_input_api", &Lilliputian::ScriptingAPIs::getInputAPI);
	m.def("get_log_api", &Lilliputian::ScriptingAPIs::getLogAPI);
    m.def("get_scene_api", &Lilliputian::ScriptingAPIs::getSceneAPI);
	m.def("get_time_api", &Lilliputian::ScriptingAPIs::getTimeAPI);
	m.def("get_window_api", &Lilliputian::ScriptingAPIs::getWindowAPI);
	m.def("get_data_directory_path", &Lilliputian::ScriptingAPIs::getDataDirectoryPath);
	m.def("get_executable_directory_path", &Lilliputian::ScriptingAPIs::getExecutableDirectoryPath);
}