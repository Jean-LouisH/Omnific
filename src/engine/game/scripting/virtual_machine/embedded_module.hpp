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

#include <game/scripting/scripting_apis/scripting_apis.hpp>
#include <game/scene/components/component_variant.hpp>
#include <pybind11/embed.h>
#include <pybind11/stl.h>
#include <utilities/vector2.hpp>
#include <utilities/rectangle.hpp>
#include <utilities/hi_res_timer.hpp>
#include <utilities/colour.hpp>
#include <utilities/aabb_2d.hpp>

PYBIND11_EMBEDDED_MODULE(lilliputian, m) 
{
	/*API classes*/
	pybind11::class_<Lilliputian::CommandLineAPI>(m, "CommandLineAPI")
		.def("open_window", &Lilliputian::CommandLineAPI::openWindow)
		.def("close_window", &Lilliputian::CommandLineAPI::closeWindow);

	pybind11::class_<Lilliputian::InputAPI>(m, "InputAPI")
		.def("is_on_press", pybind11::overload_cast<std::string>(&Lilliputian::InputAPI::isOnPress))
		.def("is_on_press", pybind11::overload_cast<std::vector<std::string>>(&Lilliputian::InputAPI::isOnPress))
		.def("is_on_double_press", pybind11::overload_cast<std::string, unsigned int>(&Lilliputian::InputAPI::isOnDoublePress))
		.def("is_on_double_press", pybind11::overload_cast<std::vector<std::string>, unsigned int>(&Lilliputian::InputAPI::isOnDoublePress))
		.def("is_on_release", pybind11::overload_cast<std::string>(&Lilliputian::InputAPI::isOnRelease))
		.def("is_on_release", pybind11::overload_cast<std::vector<std::string>>(&Lilliputian::InputAPI::isOnRelease))
		.def("is_on_hold", pybind11::overload_cast<std::string>(&Lilliputian::InputAPI::isOnHold))
		.def("is_on_hold", pybind11::overload_cast<std::vector<std::string>>(&Lilliputian::InputAPI::isOnHold));

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
		.def("get_this_entity_2d", &Lilliputian::SceneAPI::getThisEntity2D, pybind11::return_value_policy::reference)
		.def("get_this_component_variant", &Lilliputian::SceneAPI::getThisComponentVariant, pybind11::return_value_policy::reference)
		.def("get_this_scene_tree_2d", &Lilliputian::SceneAPI::getThisSceneTree2D, pybind11::return_value_policy::reference)
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

	pybind11::class_<Lilliputian::Entity2D>(m, "Entity2D")
		.def_readonly("ID", &Lilliputian::Entity2D::ID)
		.def_readwrite("name", &Lilliputian::Entity2D::name)
		.def_readwrite("parentID", &Lilliputian::Entity2D::parentID)
		.def_readwrite("childIDs", &Lilliputian::Entity2D::childIDs)
		.def_readwrite("tags", &Lilliputian::Entity2D::tags);

	pybind11::class_<Lilliputian::SceneTree2D>(m, "SceneTree2D")
		.def("add_entity_2d", &Lilliputian::SceneTree2D::addEntity2D)
		.def("add_empty_entity_2d", &Lilliputian::SceneTree2D::addEmptyEntity2D)
		.def("add_component", &Lilliputian::SceneTree2D::addComponent)
		.def("add_component_to_last_entity", &Lilliputian::SceneTree2D::addComponentToLastEntity)
		.def("remove_entity_2d", &Lilliputian::SceneTree2D::removeEntity2D)
		.def("remove_component", &Lilliputian::SceneTree2D::removeComponent)
		.def("change_current_camera", &Lilliputian::SceneTree2D::changeCurrentCamera)
		.def("get_current_camera_id", &Lilliputian::SceneTree2D::getCurrentCameraID)
		.def("get_component_variants", &Lilliputian::SceneTree2D::getComponentVariants)
		.def("get_entity_transform", &Lilliputian::SceneTree2D::getEntityTransform)
		.def("get_entity_2d", &Lilliputian::SceneTree2D::getEntity2D)
		.def("get_entity_2d_by_name", &Lilliputian::SceneTree2D::getEntity2DByName)
		.def("get_last_entity_2d", &Lilliputian::SceneTree2D::getLastEntity2D)
		.def("get_entity_2ds", &Lilliputian::SceneTree2D::getEntity2Ds)
		.def("get_event_bus", &Lilliputian::SceneTree2D::getEventBus)
		.def("get_id", &Lilliputian::SceneTree2D::getID);

	pybind11::class_<Lilliputian::Scene>(m, "Scene")
		.def("add_scene_Tree_2d", &Lilliputian::Scene::addSceneTree2D)
		.def("remove_scene_tree_2d", &Lilliputian::Scene::removeSceneTree2D)
		.def("get_asset_cache", &Lilliputian::Scene::getAssetCache, pybind11::return_value_policy::reference)
		.def("get_scene_tree", &Lilliputian::Scene::getSceneTree, pybind11::return_value_policy::reference)
		.def("get_last_scene_tree_2d", &Lilliputian::Scene::getLastSceneTree2D, pybind11::return_value_policy::reference)
		.def("get_scene_trees_2d", &Lilliputian::Scene::getSceneTree2Ds, pybind11::return_value_policy::reference);

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

	/*Component classes*/
	pybind11::class_<Lilliputian::AIBehaviourTree>(m, "AIBehaviourTree");
	pybind11::class_<Lilliputian::AISightPerception2D>(m, "AISightPerception2D");
	pybind11::class_<Lilliputian::AISoundPerception2D>(m, "AISoundPerception2D");
	pybind11::class_<Lilliputian::AnimatedSprite>(m, "AnimatedSprite");
	pybind11::class_<Lilliputian::AudioListener2D>(m, "AudioListener2D");
	pybind11::class_<Lilliputian::AudioStreamSource2D>(m, "AudioStreamSource2D");
	pybind11::class_<Lilliputian::Camera2D>(m, "Camera2D");
	pybind11::class_<Lilliputian::CircleCollider2D>(m, "CircleCollider2D");
	pybind11::class_<Lilliputian::ConstantDirectionalForce2D>(m, "ConstantDirectionalForce2D");
	pybind11::class_<Lilliputian::ConstantPointForce2D>(m, "ConstantPointForce2D");
	pybind11::class_<Lilliputian::CountdownTimer>(m, "CountdownTimer")
		.def("start", &Lilliputian::CountdownTimer::start)
		.def("stop", &Lilliputian::CountdownTimer::stop)
		.def("is_finished", &Lilliputian::CountdownTimer::isFinished);
	pybind11::class_<Lilliputian::FixedTransform2D>(m, "FixedTransform2D");
	pybind11::class_<Lilliputian::KinematicBody2D>(m, "KinematicBody2D");
	pybind11::class_<Lilliputian::NavigationMeshAgent2D>(m, "NavigationMeshAgent2D");
	pybind11::class_<Lilliputian::NavigationMeshBoxObstacle2D>(m, "NavigationMeshBoxObstacle2D");
	pybind11::class_<Lilliputian::NavigationPath2D>(m, "NavigationPath2D");
	pybind11::class_<Lilliputian::PhysicsConstraint2D>(m, "PhysicsConstraint2D");
	pybind11::class_<Lilliputian::PhysicsThruster2D>(m, "PhysicsThruster2D");
	pybind11::class_<Lilliputian::PropertyAnimation>(m, "PropertyAnimation");
	pybind11::class_<Lilliputian::RectangularCollider2D>(m, "RectangularCollider2D");
	pybind11::class_<Lilliputian::RectangularMesh2D>(m, "RectangularMesh2D");
	pybind11::class_<Lilliputian::RectangularTriggerArea2D>(m, "RectangularTriggerArea2D");
	pybind11::class_<Lilliputian::RegularPolygonalMesh2D>(m, "RegularPolygonalMesh2D");
	pybind11::class_<Lilliputian::RigidBody2D>(m, "RigidBody2D");
	pybind11::class_<Lilliputian::Sprite>(m, "Sprite");
	pybind11::class_<Lilliputian::StaticFluid2D>(m, "StaticFluid2D");
	pybind11::class_<Lilliputian::Transform2D>(m, "Transform2D");
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

	/*ComponentVariant types*/
	pybind11::class_<Lilliputian::ComponentVariant> componentVariant(m, "CV");
	componentVariant.def("set_to", pybind11::overload_cast<Lilliputian::AIBehaviourTree*>(&Lilliputian::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Lilliputian::AISightPerception2D*>(&Lilliputian::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Lilliputian::AISoundPerception2D*>(&Lilliputian::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Lilliputian::AnimatedSprite*>(&Lilliputian::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Lilliputian::AudioListener2D*>(&Lilliputian::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Lilliputian::AudioStreamSource2D*>(&Lilliputian::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Lilliputian::Camera2D*>(&Lilliputian::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Lilliputian::CircleCollider2D*>(&Lilliputian::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Lilliputian::ConstantDirectionalForce2D*>(&Lilliputian::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Lilliputian::ConstantPointForce2D*>(&Lilliputian::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Lilliputian::CountdownTimer*>(&Lilliputian::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Lilliputian::FixedTransform2D*>(&Lilliputian::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Lilliputian::KinematicBody2D*>(&Lilliputian::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Lilliputian::NavigationMeshAgent2D*>(&Lilliputian::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Lilliputian::NavigationMeshBoxObstacle2D*>(&Lilliputian::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Lilliputian::NavigationPath2D*>(&Lilliputian::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Lilliputian::PhysicsConstraint2D*>(&Lilliputian::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Lilliputian::PhysicsThruster2D*>(&Lilliputian::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Lilliputian::PropertyAnimation*>(&Lilliputian::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Lilliputian::RectangularCollider2D*>(&Lilliputian::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Lilliputian::RectangularTriggerArea2D*>(&Lilliputian::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Lilliputian::RegularPolygonalMesh2D*>(&Lilliputian::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Lilliputian::RigidBody2D*>(&Lilliputian::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Lilliputian::Sprite*>(&Lilliputian::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Lilliputian::StaticFluid2D*>(&Lilliputian::ComponentVariant::setTo));
	componentVariant.def("set_to", pybind11::overload_cast<Lilliputian::Transform2D*>(&Lilliputian::ComponentVariant::setTo));
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

	componentVariant.def("get_ai_behaviour_tree", &Lilliputian::ComponentVariant::getAIBehaviourTree, pybind11::return_value_policy::reference);
	componentVariant.def("get_ai_sight_perception_2d", &Lilliputian::ComponentVariant::getAISightPerception2D, pybind11::return_value_policy::reference);
	componentVariant.def("get_ai_sound_perception_2d", &Lilliputian::ComponentVariant::getAISoundPerception2D, pybind11::return_value_policy::reference);
	componentVariant.def("get_animated_sprite", &Lilliputian::ComponentVariant::getAnimatedSprite, pybind11::return_value_policy::reference);
	componentVariant.def("get_audio_listener_2d", &Lilliputian::ComponentVariant::getAudioListener2D, pybind11::return_value_policy::reference);
	componentVariant.def("get_audio_stream_source_2d", &Lilliputian::ComponentVariant::getAudioStreamSource2D, pybind11::return_value_policy::reference);
	componentVariant.def("get_camera_2d", &Lilliputian::ComponentVariant::getCamera2D, pybind11::return_value_policy::reference);
	componentVariant.def("get_circle_collider_2d", &Lilliputian::ComponentVariant::getCircleCollider2D, pybind11::return_value_policy::reference);
	componentVariant.def("get_constant_directional_force_2d", &Lilliputian::ComponentVariant::getConstantDirectionalForce2D, pybind11::return_value_policy::reference);
	componentVariant.def("get_constant_point_force_2d", &Lilliputian::ComponentVariant::getConstantPointForce2D, pybind11::return_value_policy::reference);
	componentVariant.def("get_countdown_timer", &Lilliputian::ComponentVariant::getCountdownTimer, pybind11::return_value_policy::reference);
	componentVariant.def("get_fixed_transform_2d", &Lilliputian::ComponentVariant::getFixedTransform2D, pybind11::return_value_policy::reference);
	componentVariant.def("get_kinematic_body_2d", &Lilliputian::ComponentVariant::getKinematicBody2D, pybind11::return_value_policy::reference);
	componentVariant.def("get_navigation_mesh_agent_2d", &Lilliputian::ComponentVariant::getNavigationMeshAgent2D, pybind11::return_value_policy::reference);
	componentVariant.def("get_navigation_mesh_box_obstacle_2d", &Lilliputian::ComponentVariant::getNavigationMeshBoxObstacle2D, pybind11::return_value_policy::reference);
	componentVariant.def("get_naviation_path_2d", &Lilliputian::ComponentVariant::getNavigationPath2D, pybind11::return_value_policy::reference);
	componentVariant.def("get_physics_constraint_2d", &Lilliputian::ComponentVariant::getPhysicsConstraint2D, pybind11::return_value_policy::reference);
	componentVariant.def("get_physics_thruster_2d", &Lilliputian::ComponentVariant::getPhysicsThruster2D, pybind11::return_value_policy::reference);
	componentVariant.def("get_property_animation", &Lilliputian::ComponentVariant::getPropertyAnimation, pybind11::return_value_policy::reference);
	componentVariant.def("get_rectangular_collider_2d", &Lilliputian::ComponentVariant::getRectangularCollider2D, pybind11::return_value_policy::reference);
	componentVariant.def("get_rectangular_trigger_area_2d", &Lilliputian::ComponentVariant::getRectangularTriggerArea2D, pybind11::return_value_policy::reference);
	componentVariant.def("get_regular_polygonal_mesh_2d", &Lilliputian::ComponentVariant::getRegularPolygonalMesh2D, pybind11::return_value_policy::reference);
	componentVariant.def("get_rigid_body_2d", &Lilliputian::ComponentVariant::getRigidBody2D, pybind11::return_value_policy::reference);
	componentVariant.def("get_sprite", &Lilliputian::ComponentVariant::getSprite, pybind11::return_value_policy::reference);
	componentVariant.def("get_static_fluid_2d", &Lilliputian::ComponentVariant::getStaticFluid2D, pybind11::return_value_policy::reference);
	componentVariant.def("get_transform_2d", &Lilliputian::ComponentVariant::getTransform2D, pybind11::return_value_policy::reference);
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

	pybind11::enum_<Lilliputian::ComponentVariant::Type>(componentVariant, "Type")
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
		.value("KINEMATIC_BODY_2D", Lilliputian::ComponentVariant::Type::KINEMATIC_BODY_2D)
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
		.value("UI_TREE", Lilliputian::ComponentVariant::Type::UI_TREE)
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
	pybind11::class_<Lilliputian::Vector2>(m, "Vector2")
		.def_readwrite("x", &Lilliputian::Vector2::x)
		.def_readwrite("y", &Lilliputian::Vector2::y);
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
	m.def("get_data_directory", &Lilliputian::ScriptingAPIs::getDataDirectory);
	m.def("get_executable_directory", &Lilliputian::ScriptingAPIs::getExecutableDirectory);
}