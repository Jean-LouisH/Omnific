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

#include "core/systems/scripting_system/constants.hpp"

#if ENABLE_NON_NATIVE_SCRIPTING_LANGUAGE

#include <core/assets/audio_stream.hpp>
#include <core/assets/font.hpp>
#include <core/assets/image.hpp>
#include <core/assets/material.hpp>
#include <core/assets/mesh.hpp>
#include <core/assets/rig.hpp>
#include "core/assets/script.hpp"
#include <core/assets/shader.hpp>
#include <core/assets/text.hpp>

#include <core/component.hpp>
#include <core/singletons/entity_context.hpp>
#include <pybind11/embed.h>
#include <pybind11/stl.h>
#include <glm/glm.hpp>
#include <core/scene/scene.hpp>
#include <core/utilities/rectangle.hpp>
#include <core/utilities/hi_res_timer.hpp>
#include <core/utilities/colour.hpp>
#include <core/utilities/aabb_2d.hpp>
#include <core/utilities/aliases.hpp>
#include <memory>

#include <customization/class_registry/class_registry.hpp>

PYBIND11_EMBEDDED_MODULE(omnia, m) 
{
	/*OS Classes*/

	pybind11::class_<Omnia::SharedLibraryAccess>(m, "SharedLibraryAccess");

	pybind11::class_<Omnia::FileAccess>(m, "FileAccess");

	pybind11::class_<Omnia::Input>(m, "Input")
		.def("is_on_press", pybind11::overload_cast<std::string>(&Omnia::Input::isOnPress))
		.def("is_on_press", pybind11::overload_cast<std::vector<std::string>>(&Omnia::Input::isOnPress))
		.def("is_on_press", pybind11::overload_cast<std::vector<std::string>, Omnia::PlayerID>(&Omnia::Input::isOnPress))
		.def("is_on_double_press", pybind11::overload_cast<std::string, unsigned int>(&Omnia::Input::isOnDoublePress))
		.def("is_on_double_press", pybind11::overload_cast<std::vector<std::string>, unsigned int>(&Omnia::Input::isOnDoublePress))
		.def("is_on_double_press", pybind11::overload_cast<std::vector<std::string>, unsigned int>(&Omnia::Input::isOnDoublePress))
		.def("is_pressed", pybind11::overload_cast<std::string>(&Omnia::Input::isPressed))
		.def("is_pressed", pybind11::overload_cast<std::vector<std::string>>(&Omnia::Input::isPressed))
		.def("is_pressed", pybind11::overload_cast<std::vector<std::string>, Omnia::PlayerID>(&Omnia::Input::isPressed))
		.def("is_on_release", pybind11::overload_cast<std::string>(&Omnia::Input::isOnRelease))
		.def("is_on_release", pybind11::overload_cast<std::vector<std::string>>(&Omnia::Input::isOnRelease))
		.def("is_on_release", pybind11::overload_cast<std::vector<std::string>, Omnia::PlayerID>(&Omnia::Input::isOnRelease))
		.def("is_released", pybind11::overload_cast<std::string>(&Omnia::Input::isReleased))
		.def("is_released", pybind11::overload_cast<std::string, Omnia::PlayerID>(&Omnia::Input::isReleased))
		.def("get_axis", pybind11::overload_cast<std::string>(&Omnia::Input::getAxis))
		.def("get_axis", pybind11::overload_cast<std::string, Omnia::PlayerID>(&Omnia::Input::getAxis))
		.def("is_left_mouse_button_on_press", &Omnia::Input::isLeftMouseButtonOnPress)
		.def("is_left_mouse_button_on_release", &Omnia::Input::isLeftMouseButtonOnRelease)
		.def("is_left_mouse_button_double_clicked", &Omnia::Input::isLeftMouseButtonDoubleClicked)
		.def("is_middle_mouse_button_on_press", &Omnia::Input::isMiddleMouseButtonOnPress)
		.def("is_middle_mouse_button_on_release", &Omnia::Input::isMiddleMouseButtonOnRelease)
		.def("is_middle_mouse_button_double_clicked", &Omnia::Input::isMiddleMouseButtonDoubleClicked)
		.def("is_right_mouse_button_on_press", &Omnia::Input::isRightMouseButtonOnPress)
		.def("is_right_mouse_button_on_release", &Omnia::Input::isRightMouseButtonOnRelease)
		.def("is_right_mouse_button_double_clicked", &Omnia::Input::isRightMouseButtonDoubleClicked)
		.def("get_mouse_position", &Omnia::Input::getMousePosition)
		.def("get_mouse_wheel_velocity", &Omnia::Input::getMouseWheelVelocity)
		.def("get_mouse_motion_velocity", &Omnia::Input::getMouseMotionVelocity)
		.def("request_shutdown", &Omnia::Input::requestShutdown)
		.def("request_restart", &Omnia::Input::requestRestart);

	pybind11::class_<Omnia::Logger>(m, "Logger")
		.def("write", &Omnia::Logger::write)
		.def("write_to_file", &Omnia::Logger::writeToFile)
		.def("get_last_message", &Omnia::Logger::getLastMessage)
		.def("get_logs", &Omnia::Logger::getLogs);

	pybind11::class_<Omnia::NetworkAccess>(m, "NetworkAccess");

	pybind11::class_<Omnia::Platform>(m, "Platform")
		.def("get_logical_core_count", &Omnia::Platform::getLogicalCoreCount)
		.def("get_l1_cache_line_size", &Omnia::Platform::getL1CacheLineSize)
		.def("get_system_ram", &Omnia::Platform::getSystemRAM)
		.def("get_os_name", &Omnia::Platform::getOSName);

	pybind11::class_<Omnia::Profiler>(m, "Profiler")
		.def("add_timer", &Omnia::Profiler::addTimer)
		.def("get_timer", &Omnia::Profiler::getTimer)
		.def("get_fps", &Omnia::Profiler::getFPS);

	pybind11::class_<Omnia::ThreadPool>(m, "ThreadPool");

	pybind11::class_<Omnia::Window>(m, "Window")
		.def("set_to_windowed", &Omnia::Window::setToWindowed)
		.def("set_to_fullscreen", &Omnia::Window::setToFullscreen)
		.def("toggle_windowed_fullscreen", &Omnia::Window::toggleWindowedFullscreen)
		.def("resize", &Omnia::Window::resize)
		.def("change_title", &Omnia::Window::changeTitle)
		.def("change_icon", &Omnia::Window::changeIcon)
		.def("maximize", &Omnia::Window::maximize)
		.def("minimize", &Omnia::Window::minimize)
		.def("raise", &Omnia::Window::raise)
		.def("restore", &Omnia::Window::restore)
		.def("hide", &Omnia::Window::hide)
		.def("show", &Omnia::Window::show);

	/*Scene classes*/

	pybind11::class_<Omnia::Entity, std::shared_ptr<Omnia::Entity>>(m, "Entity")
		.def("get_id", &Omnia::Entity::getID)
		.def("get_name", &Omnia::Entity::getName)
		.def_readwrite("parentID", &Omnia::Entity::parentID)
		.def_readwrite("childIDs", &Omnia::Entity::childIDs)
		.def_readwrite("tags", &Omnia::Entity::tags);

	pybind11::class_<Omnia::SceneTree, std::shared_ptr<Omnia::SceneTree>>(m, "SceneTree")
		.def("add_entity", &Omnia::SceneTree::addEntity)
		.def("add_empty_entity", &Omnia::SceneTree::addEmptyEntity)
		.def("add_component", &Omnia::SceneTree::addComponent)
		.def("add_component_to_last_entity", &Omnia::SceneTree::addComponentToLastEntity)
		.def("remove_entity", &Omnia::SceneTree::removeEntity)
		.def("remove_component", &Omnia::SceneTree::removeComponent)
		.def("get_component", pybind11::overload_cast<std::string, Omnia::EntityID>(&Omnia::SceneTree::getComponent))
		.def("get_components", &Omnia::SceneTree::getComponents)
		.def("get_entity", &Omnia::SceneTree::getEntity)
		.def("get_entity_by_name", &Omnia::SceneTree::getEntityByName)
		.def("get_last_entity", &Omnia::SceneTree::getLastEntity)
		.def("get_entities", &Omnia::SceneTree::getEntities)
		.def("get_collision_registry", &Omnia::SceneTree::getCollisionRegistry)
		.def("get_event_bus", &Omnia::SceneTree::getEventBus)
		.def("get_haptic_signal_buffer", &Omnia::SceneTree::getHapticSignalBuffer)
		.def("get_id", &Omnia::SceneTree::getID);

	pybind11::class_<Omnia::Collision, std::shared_ptr<Omnia::Collision>>(m, "Collision")
		.def_readwrite("collider_entity_id", &Omnia::Collision::colliderEntityID)
		.def_readwrite("collider_name", &Omnia::Collision::colliderName)
		.def_readwrite("other_collider_entity_id", &Omnia::Collision::otherColliderEntityID)
		.def_readwrite("other_collider_name", &Omnia::Collision::otherColliderName)
		.def_readwrite("attack_angle", &Omnia::Collision::attackAngle)
		.def_readwrite("elasticity_ratio", &Omnia::Collision::elasticityRatio)
		.def_readwrite("mass", &Omnia::Collision::mass)
		.def_readwrite("linear_velocity", &Omnia::Collision::linearVelocity)
		.def_readwrite("rotation", &Omnia::Collision::rotation)
		.def_readwrite("other_elasticity_ratio", &Omnia::Collision::otherElasticityRatio)
		.def_readwrite("other_mass", &Omnia::Collision::otherMass)
		.def_readwrite("other_linear_velocity", &Omnia::Collision::otherLinearVelocity)
		.def_readwrite("other_rotation", &Omnia::Collision::otherRotation);

	pybind11::class_<Omnia::CollisionRegistry, std::shared_ptr<Omnia::CollisionRegistry>>(m, "CollisionRegistry")
		.def("add_or_update", &Omnia::CollisionRegistry::addOrUpdate)
		.def("remove", &Omnia::CollisionRegistry::remove)
		.def("query", &Omnia::CollisionRegistry::query, pybind11::return_value_policy::reference)
		.def("query_all", &Omnia::CollisionRegistry::queryAll)
		.def("is_colliding", &Omnia::CollisionRegistry::isColliding)
		.def("get_collision_count", &Omnia::CollisionRegistry::getCollisionCount);

	pybind11::class_<Omnia::Event::Parameters>(m, "EventParameters")
		.def_readwrite("numbers", &Omnia::Event::Parameters::numbers)
		.def_readwrite("strings", &Omnia::Event::Parameters::strings);

	pybind11::class_<Omnia::Event>(m, "Event")
		.def(pybind11::init<std::string, uint64_t, Omnia::Event::Parameters>())
		.def(pybind11::init<std::string, uint64_t>())
		.def("get_name", &Omnia::Event::getName)
		.def("get_parameters", &Omnia::Event::getParameters)
		.def("get_timestamp", &Omnia::Event::getTimestamp);

	pybind11::class_<Omnia::EventBus, std::shared_ptr<Omnia::EventBus>>(m, "EventBus")
		.def("clear", &Omnia::EventBus::clear)
		.def("query", &Omnia::EventBus::query)
		.def("query_count", &Omnia::EventBus::queryCount)
		.def("publish", pybind11::overload_cast<std::string>(&Omnia::EventBus::publish))
		.def("publish", pybind11::overload_cast<std::string, std::unordered_map<std::string, double>, std::unordered_map<std::string, std::string>>(&Omnia::EventBus::publish))
		.def("publish", pybind11::overload_cast<std::string, std::unordered_map<std::string, std::string>>(&Omnia::EventBus::publish))
		.def("publish", pybind11::overload_cast<std::string, std::unordered_map<std::string, double>>(&Omnia::EventBus::publish));

	pybind11::class_<Omnia::HapticSignal>(m, "HapticSignal")
		.def(pybind11::init<Omnia::PlayerID, float, uint16_t>())
		.def("get_duration", &Omnia::HapticSignal::getDuration)
		.def("get_player_id", &Omnia::HapticSignal::getPlayerID)
		.def("get_strength", &Omnia::HapticSignal::getStrength);

	pybind11::class_<Omnia::HapticSignalBuffer, std::shared_ptr<Omnia::HapticSignalBuffer>>(m, "HapticSignalBuffer")
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
	pybind11::class_<Omnia::Component, std::shared_ptr<Omnia::Component>>(m, Omnia::Component::TYPE_STRING)
		.def("set_entity_id", &Omnia::Component::setEntityID)
		.def("get_id", &Omnia::Component::getID)
		.def("is_attached_to_entity", &Omnia::Component::isAttachedToEntity)
		.def("get_type", &Omnia::Component::getType)
		.def("is_renderable", &Omnia::Component::isRenderable);

	pybind11::class_<Omnia::ShaderParameters, std::shared_ptr<Omnia::ShaderParameters>>(m, "ShaderParameters")
		.def_readwrite("int_uniforms", &Omnia::ShaderParameters::intUniforms)
		.def_readwrite("bool_uniforms", &Omnia::ShaderParameters::boolUniforms)
		.def_readwrite("float_uniforms", &Omnia::ShaderParameters::floatUniforms)
		.def_readwrite("vec2_uniforms", &Omnia::ShaderParameters::vec2Uniforms)
		.def_readwrite("vec3_uniforms", &Omnia::ShaderParameters::vec3Uniforms)
		.def_readwrite("vec4_uniforms", &Omnia::ShaderParameters::vec4Uniforms)
		.def_readwrite("mat4_uniforms", &Omnia::ShaderParameters::mat4Uniforms);

	pybind11::class_<Omnia::RenderableComponent, Omnia::Component, std::shared_ptr<Omnia::RenderableComponent>>(m, Omnia::RenderableComponent::TYPE_STRING)
		.def_readwrite("shader_parameters", &Omnia::RenderableComponent::shaderParameters);

	pybind11::class_<Omnia::AudioListener, Omnia::Component, std::shared_ptr<Omnia::AudioListener>>(m, Omnia::AudioListener::TYPE_STRING);
	pybind11::class_<Omnia::AudioSource, Omnia::Component, std::shared_ptr<Omnia::AudioSource>>(m, Omnia::AudioSource::TYPE_STRING);
	pybind11::class_<Omnia::Camera, Omnia::Component, std::shared_ptr<Omnia::Camera>>(m, Omnia::Camera::TYPE_STRING)
		.def("toggle_wireframe_mode", &Omnia::Camera::toggleWireframeMode);
	pybind11::class_<Omnia::Timer, Omnia::Component, std::shared_ptr<Omnia::Timer>>(m, Omnia::Timer::TYPE_STRING)
		.def("start", &Omnia::Timer::start)
		.def("stop", &Omnia::Timer::stop)
		.def("is_finished", &Omnia::Timer::isFinished);
	pybind11::class_<Omnia::PhysicsBody, Omnia::Component, std::shared_ptr<Omnia::PhysicsBody>>(m, Omnia::PhysicsBody::TYPE_STRING)
		.def_readwrite("linear_velocity", &Omnia::PhysicsBody::linearVelocity)
		.def("accelerate", pybind11::overload_cast<glm::vec3, float>(&Omnia::PhysicsBody::accelerate))
		.def("accelerate", pybind11::overload_cast<glm::vec3, float, float>(& Omnia::PhysicsBody::accelerate))
		.def("accelerate_x", &Omnia::PhysicsBody::accelerateX)
		.def("accelerate_y", &Omnia::PhysicsBody::accelerateY)
		.def("accelerate_z", &Omnia::PhysicsBody::accelerateZ)
		.def("decelerate", &Omnia::PhysicsBody::decelerate)
		.def("decelerate_x", &Omnia::PhysicsBody::decelerateX)
		.def("decelerate_y", &Omnia::PhysicsBody::decelerateY)
		.def("decelerate_z", &Omnia::PhysicsBody::decelerateZ);
	pybind11::class_<Omnia::Light, Omnia::Component, std::shared_ptr<Omnia::Light>>(m, Omnia::Light::TYPE_STRING);
	pybind11::class_<Omnia::Model, Omnia::RenderableComponent, std::shared_ptr<Omnia::Model>>(m, Omnia::Model::TYPE_STRING)
		.def("set_to_cube", &Omnia::Model::setToCube)
		.def("set_to_textured_cube", &Omnia::Model::setToTexturedCube);
	pybind11::class_<Omnia::PropertyAnimation, Omnia::Component, std::shared_ptr<Omnia::PropertyAnimation>>(m, Omnia::PropertyAnimation::TYPE_STRING);
	pybind11::class_<Omnia::Collider, Omnia::Component, std::shared_ptr<Omnia::Collider>>(m, Omnia::Collider::TYPE_STRING);
	pybind11::class_<Omnia::Sprite, Omnia::RenderableComponent, std::shared_ptr<Omnia::Sprite>>(m, Omnia::Sprite::TYPE_STRING);
	pybind11::class_<Omnia::Transform, Omnia::Component, std::shared_ptr<Omnia::Transform>>(m, Omnia::Transform::TYPE_STRING)
		.def_readwrite("translation", &Omnia::Transform::translation)
		.def_readwrite("rotation", &Omnia::Transform::rotation)
		.def_readwrite("scale", &Omnia::Transform::scale)
		.def("translate_x", &Omnia::Transform::translateX)
		.def("translate_y", &Omnia::Transform::translateY)
		.def("translate_z", &Omnia::Transform::translateZ)
		.def("rotate_x", &Omnia::Transform::rotateX)
		.def("rotate_y", &Omnia::Transform::rotateY)
		.def("rotate_z", &Omnia::Transform::rotateZ);
	pybind11::class_<Omnia::GUI, Omnia::RenderableComponent, std::shared_ptr<Omnia::GUI>>(m, Omnia::GUI::TYPE_STRING);
	pybind11::class_<Omnia::Viewport, Omnia::Component, std::shared_ptr<Omnia::Viewport>>(m, Omnia::Viewport::TYPE_STRING);

	/*Utility classes*/
	pybind11::class_<glm::vec2>(m, "Vector2")
		.def_readwrite("x", &glm::vec2::x)
		.def_readwrite("y", &glm::vec2::y);

	pybind11::class_<glm::vec3>(m, "Vector3")
		.def_readwrite("x", &glm::vec3::x)
		.def_readwrite("y", &glm::vec3::y)
		.def_readwrite("z", &glm::vec3::z);

	pybind11::class_<Omnia::Rectangle>(m, "Rectangle")
		.def_readwrite("height", &Omnia::Rectangle::height)
		.def_readwrite("width", &Omnia::Rectangle::width);

	pybind11::class_<Omnia::HiResTimer, std::shared_ptr<Omnia::HiResTimer>>(m, "HiResTimer")
		.def("set_start", &Omnia::HiResTimer::setStart)
		.def("set_end", &Omnia::HiResTimer::setEnd)
		.def("get_delta", &Omnia::HiResTimer::getDelta)
		.def("get_delta_in_seconds", &Omnia::HiResTimer::getDeltaInSeconds)
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

	/*Singletons*/

	m.def("add_scene", pybind11::overload_cast<std::shared_ptr<Omnia::Scene>>(&Omnia::SceneStorage::addScene));
	m.def("add_scene", pybind11::overload_cast<std::string>(&Omnia::SceneStorage::addScene));
	m.def("add_and_change_to_scene", pybind11::overload_cast<std::shared_ptr<Omnia::Scene>>(&Omnia::SceneStorage::addAndChangeToScene));
	m.def("add_and_change_to_scene", pybind11::overload_cast<std::string>(&Omnia::SceneStorage::addAndChangeToScene));
	m.def("remove_scene", &Omnia::SceneStorage::removeScene);
	m.def("load_scene", &Omnia::SceneStorage::changeToScene);
	m.def("reload_active_scene", &Omnia::SceneStorage::reloadActiveScene);
	m.def("get_active_scene", &Omnia::SceneStorage::getActiveScene);
	m.def("get_active_scene_name", &Omnia::SceneStorage::getActiveSceneName);
	m.def("has_no_scenes", &Omnia::SceneStorage::hasNoScenes);
	m.def("has_scene", &Omnia::SceneStorage::hasScene);
	m.def("has_active_scene_changed", &Omnia::SceneStorage::hasActiveSceneChanged);
	m.def("clear_scenes", &Omnia::SceneStorage::clearScenes);
	m.def("get_scene_by_name", &Omnia::SceneStorage::getSceneByName);

	m.def("has_component", &Omnia::EntityContext::hasComponent);
	m.def("get_entity", &Omnia::EntityContext::getEntity);
	m.def("get_component", &Omnia::EntityContext::getComponent);
	m.def("get_scene", &Omnia::EntityContext::getScene);
	m.def("get_scene_tree", &Omnia::EntityContext::getSceneTree);
	m.def("get_time_delta", &Omnia::EntityContext::getTimeDelta);

	m.def("get_shared_library_access", &Omnia::OS::getSharedLibraryAccess, pybind11::return_value_policy::reference);
	m.def("get_file_access", &Omnia::OS::getFileAccess, pybind11::return_value_policy::reference);
	m.def("get_input", &Omnia::OS::getInput, pybind11::return_value_policy::reference);
	m.def("get_logger", &Omnia::OS::getLogger, pybind11::return_value_policy::reference);
	m.def("get_network_access", &Omnia::OS::getNetworkAccess, pybind11::return_value_policy::reference);
	m.def("get_platform", &Omnia::OS::getPlatform, pybind11::return_value_policy::reference);
	m.def("get_profiler", &Omnia::OS::getProfiler, pybind11::return_value_policy::reference);
	m.def("get_thread_pool", &Omnia::OS::getThreadPool, pybind11::return_value_policy::reference);
	m.def("get_window", &Omnia::OS::getWindow, pybind11::return_value_policy::reference);
	m.def("get_args", &Omnia::OS::getArgs);
}

#endif