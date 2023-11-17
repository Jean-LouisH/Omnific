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
#include "python_entity_context.hpp"
#include <pybind11/embed.h>
#include <pybind11/stl.h>
#include <glm/glm.hpp>
#include <core/scene/scene.hpp>
#include <core/utilities/aliases.hpp>
#include <core/singletons/event_bus.hpp>
#include <memory>

#include <customization/class_registry/class_registry.hpp>

PYBIND11_EMBEDDED_MODULE(omnia, m) 
{
	/*OS Classes*/

	pybind11::class_<Omnia::SharedLibraryAccess>(m, "SharedLibraryAccess");

	pybind11::class_<Omnia::FileAccess>(m, "FileAccess");

	pybind11::class_<Omnia::Input>(m, "Input")
		.def("is_on_press", pybind11::overload_cast<std::string>(&Omnia::Input::is_on_press))
		.def("is_on_press", pybind11::overload_cast<std::vector<std::string>>(&Omnia::Input::is_on_press))
		.def("is_on_press", pybind11::overload_cast<std::vector<std::string>, Omnia::PlayerID>(&Omnia::Input::is_on_press))
		.def("is_on_double_press", pybind11::overload_cast<std::string, unsigned int>(&Omnia::Input::is_on_double_press))
		.def("is_on_double_press", pybind11::overload_cast<std::vector<std::string>, unsigned int>(&Omnia::Input::is_on_double_press))
		.def("is_on_double_press", pybind11::overload_cast<std::vector<std::string>, unsigned int>(&Omnia::Input::is_on_double_press))
		.def("is_pressed", pybind11::overload_cast<std::string>(&Omnia::Input::is_pressed))
		.def("is_pressed", pybind11::overload_cast<std::vector<std::string>>(&Omnia::Input::is_pressed))
		.def("is_pressed", pybind11::overload_cast<std::vector<std::string>, Omnia::PlayerID>(&Omnia::Input::is_pressed))
		.def("is_on_release", pybind11::overload_cast<std::string>(&Omnia::Input::is_on_release))
		.def("is_on_release", pybind11::overload_cast<std::vector<std::string>>(&Omnia::Input::is_on_release))
		.def("is_on_release", pybind11::overload_cast<std::vector<std::string>, Omnia::PlayerID>(&Omnia::Input::is_on_release))
		.def("is_released", pybind11::overload_cast<std::string>(&Omnia::Input::is_released))
		.def("is_released", pybind11::overload_cast<std::string, Omnia::PlayerID>(&Omnia::Input::is_released))
		.def("get_axis", pybind11::overload_cast<std::string>(&Omnia::Input::get_axis))
		.def("get_axis", pybind11::overload_cast<std::string, Omnia::PlayerID>(&Omnia::Input::get_axis))
		.def("is_left_mouse_button_on_press", &Omnia::Input::is_left_mouse_button_on_press)
		.def("is_left_mouse_button_on_release", &Omnia::Input::is_left_mouse_button_on_release)
		.def("is_left_mouse_button_double_clicked", &Omnia::Input::is_left_mouse_button_double_clicked)
		.def("is_middle_mouse_button_on_press", &Omnia::Input::is_middle_mouse_button_on_press)
		.def("is_middle_mouse_button_on_release", &Omnia::Input::is_middle_mouse_button_on_release)
		.def("is_middle_mouse_button_double_clicked", &Omnia::Input::is_middle_mouse_button_double_clicked)
		.def("is_right_mouse_button_on_press", &Omnia::Input::is_right_mouse_button_on_press)
		.def("is_right_mouse_button_on_release", &Omnia::Input::is_right_mouse_button_on_release)
		.def("is_right_mouse_button_double_clicked", &Omnia::Input::is_right_mouse_button_double_clicked)
		.def("get_mouse_position", &Omnia::Input::get_mouse_position)
		.def("get_mouse_wheel_velocity", &Omnia::Input::get_mouse_wheel_velocity)
		.def("get_mouse_motion_velocity", &Omnia::Input::get_mouse_motion_velocity)
		.def("request_shutdown", &Omnia::Input::request_shutdown)
		.def("request_restart", &Omnia::Input::request_restart);

	pybind11::class_<Omnia::Logger>(m, "Logger")
		.def("write", &Omnia::Logger::write)
		.def("write_to_file", &Omnia::Logger::write_to_file)
		.def("get_last_message", &Omnia::Logger::get_last_message)
		.def("get_logs", &Omnia::Logger::get_logs);

	pybind11::class_<Omnia::NetworkAccess>(m, "NetworkAccess");

	pybind11::class_<Omnia::Platform>(m, "Platform")
		.def("get_logical_core_count", &Omnia::Platform::get_logical_core_count)
		.def("get_l1_cache_line_size", &Omnia::Platform::get_l1cache_line_size)
		.def("get_system_ram", &Omnia::Platform::get_system_ram)
		.def("get_os_name", &Omnia::Platform::get_osname);

	pybind11::class_<Omnia::Profiler>(m, "Profiler")
		.def("add_timer", &Omnia::Profiler::add_timer)
		.def("get_timer", &Omnia::Profiler::get_timer)
		.def("get_fps", &Omnia::Profiler::get_fps);

	pybind11::class_<Omnia::ThreadPool>(m, "ThreadPool");

	pybind11::class_<Omnia::Window>(m, "Window")
		.def("set_to_windowed", &Omnia::Window::set_to_windowed)
		.def("set_to_fullscreen", &Omnia::Window::set_to_fullscreen)
		.def("toggle_windowed_fullscreen", &Omnia::Window::toggle_windowed_fullscreen)
		.def("resize", &Omnia::Window::resize)
		.def("change_title", &Omnia::Window::change_title)
		.def("change_icon", &Omnia::Window::change_icon)
		.def("maximize", &Omnia::Window::maximize)
		.def("minimize", &Omnia::Window::minimize)
		.def("raise", &Omnia::Window::raise)
		.def("restore", &Omnia::Window::restore)
		.def("hide", &Omnia::Window::hide)
		.def("show", &Omnia::Window::show);

	/*Scene classes*/

	pybind11::class_<Omnia::Entity, std::shared_ptr<Omnia::Entity>>(m, "Entity")
		.def("get_id", &Omnia::Entity::get_id)
		.def("get_name", &Omnia::Entity::get_name)
		.def_readwrite("parent_id", &Omnia::Entity::parent_id)
		.def_readwrite("child_ids", &Omnia::Entity::child_ids)
		.def_readwrite("tags", &Omnia::Entity::tags);

	pybind11::class_<Omnia::SceneLayer, std::shared_ptr<Omnia::SceneLayer>>(m, "SceneLayer")
		.def("add_entity", &Omnia::SceneLayer::add_entity)
		.def("add_empty_entity", &Omnia::SceneLayer::add_empty_entity)
		.def("add_component", &Omnia::SceneLayer::add_component)
		.def("add_component_to_last_entity", &Omnia::SceneLayer::add_component_to_last_entity)
		.def("remove_entity", &Omnia::SceneLayer::remove_entity)
		.def("remove_component", &Omnia::SceneLayer::remove_component)
		.def("get_component", pybind11::overload_cast<std::string, Omnia::EntityID>(&Omnia::SceneLayer::get_component))
		.def("get_components", &Omnia::SceneLayer::get_components)
		.def("get_entity", &Omnia::SceneLayer::get_entity)
		.def("get_entity_by_name", &Omnia::SceneLayer::get_entity_by_name)
		.def("get_last_entity", &Omnia::SceneLayer::get_last_entity)
		.def("get_entities", &Omnia::SceneLayer::get_entities)
		.def("get_collision_registry", &Omnia::SceneLayer::get_collision_registry)
		.def("get_haptic_signal_buffer", &Omnia::SceneLayer::get_haptic_signal_buffer)
		.def("get_id", &Omnia::SceneLayer::get_id);

	pybind11::class_<Omnia::Collision, std::shared_ptr<Omnia::Collision>>(m, "Collision")
		.def_readwrite("collider_entity_id", &Omnia::Collision::collider_entity_id)
		.def_readwrite("collider_name", &Omnia::Collision::collider_name)
		.def_readwrite("other_collider_entity_id", &Omnia::Collision::other_collider_entity_id)
		.def_readwrite("other_collider_name", &Omnia::Collision::other_collider_name)
		.def_readwrite("attack_angle", &Omnia::Collision::attack_angle)
		.def_readwrite("elasticity_ratio", &Omnia::Collision::elasticity_ratio)
		.def_readwrite("mass", &Omnia::Collision::mass)
		.def_readwrite("linear_velocity", &Omnia::Collision::linear_velocity)
		.def_readwrite("rotation", &Omnia::Collision::rotation)
		.def_readwrite("other_elasticity_ratio", &Omnia::Collision::other_elasticity_ratio)
		.def_readwrite("other_mass", &Omnia::Collision::other_mass)
		.def_readwrite("other_linear_velocity", &Omnia::Collision::other_linear_velocity)
		.def_readwrite("other_rotation", &Omnia::Collision::other_rotation);

	pybind11::class_<Omnia::CollisionRegistry, std::shared_ptr<Omnia::CollisionRegistry>>(m, "CollisionRegistry")
		.def("add_or_update", &Omnia::CollisionRegistry::add_or_update)
		.def("remove", &Omnia::CollisionRegistry::remove)
		.def("query", &Omnia::CollisionRegistry::query, pybind11::return_value_policy::reference)
		.def("query_all", &Omnia::CollisionRegistry::query_all)
		.def("is_colliding", &Omnia::CollisionRegistry::is_colliding)
		.def("get_collision_count", &Omnia::CollisionRegistry::get_collision_count);

	pybind11::class_<Omnia::HapticSignal>(m, "HapticSignal")
		.def(pybind11::init<Omnia::PlayerID, float, uint16_t>())
		.def("get_duration", &Omnia::HapticSignal::get_duration)
		.def("get_player_id", &Omnia::HapticSignal::get_player_id)
		.def("get_strength", &Omnia::HapticSignal::get_strength);

	pybind11::class_<Omnia::HapticSignalBuffer, std::shared_ptr<Omnia::HapticSignalBuffer>>(m, "HapticSignalBuffer")
		.def("publish", &Omnia::HapticSignalBuffer::publish)
		.def("query", &Omnia::HapticSignalBuffer::query)
		.def("get_haptic_signals", &Omnia::HapticSignalBuffer::get_haptic_signals)
		.def("clear", &Omnia::HapticSignalBuffer::clear);

	/*Asset classes*/

	pybind11::class_<Omnia::Asset, std::shared_ptr<Omnia::Asset>>(m, "Asset")
		.def("get_id", &Omnia::Asset::get_id)
		.def("set_name", &Omnia::Asset::set_name)
		.def("get_name", &Omnia::Asset::get_name)
		.def("get_type", &Omnia::Asset::get_type);

	pybind11::class_<Omnia::AudioStream, Omnia::Asset, std::shared_ptr<Omnia::AudioStream>>(m, Omnia::AudioStream::TYPE_STRING);
	pybind11::class_<Omnia::Font, Omnia::Asset, std::shared_ptr<Omnia::Font>>(m, Omnia::Font::TYPE_STRING);
	pybind11::class_<Omnia::Image, Omnia::Asset, std::shared_ptr<Omnia::Image>>(m, Omnia::Image::TYPE_STRING);
	pybind11::class_<Omnia::Text, Omnia::Asset, std::shared_ptr<Omnia::Text>>(m, Omnia::Text::TYPE_STRING);

	/*Component classes*/
	pybind11::class_<Omnia::Component, std::shared_ptr<Omnia::Component>>(m, Omnia::Component::TYPE_STRING)
		.def("set_entity_id", &Omnia::Component::set_entity_id)
		.def("get_id", &Omnia::Component::get_id)
		.def("is_attached_to_entity", &Omnia::Component::is_attached_to_entity)
		.def("get_type", &Omnia::Component::get_type)
		.def("is_renderable", &Omnia::Component::is_renderable);

	pybind11::class_<Omnia::ShaderParameters, std::shared_ptr<Omnia::ShaderParameters>>(m, "ShaderParameters")
		.def("set_int_uniform", &Omnia::ShaderParameters::set_int_uniform)
		.def("set_bool_uniform", &Omnia::ShaderParameters::set_bool_uniform)
		.def("set_float_uniform", &Omnia::ShaderParameters::set_float_uniform)
		.def("set_vec2_uniform", &Omnia::ShaderParameters::set_vec2_uniform)
		.def("set_vec3_uniform", &Omnia::ShaderParameters::set_vec3_uniform)
		.def("set_vec4_uniform", &Omnia::ShaderParameters::set_vec4_uniform)
		.def("set_mat4_uniform", &Omnia::ShaderParameters::set_mat4_uniform);

	pybind11::class_<Omnia::RenderableComponent, Omnia::Component, std::shared_ptr<Omnia::RenderableComponent>>(m, Omnia::RenderableComponent::TYPE_STRING)
		.def_readwrite("shader_parameters", &Omnia::RenderableComponent::shader_parameters);

	pybind11::class_<Omnia::AudioListener, Omnia::Component, std::shared_ptr<Omnia::AudioListener>>(m, Omnia::AudioListener::TYPE_STRING);
	pybind11::class_<Omnia::AudioSource, Omnia::Component, std::shared_ptr<Omnia::AudioSource>>(m, Omnia::AudioSource::TYPE_STRING);
	pybind11::class_<Omnia::Camera, Omnia::Component, std::shared_ptr<Omnia::Camera>>(m, Omnia::Camera::TYPE_STRING)
		.def("toggle_wireframe_mode", &Omnia::Camera::toggle_wireframe_mode);
	pybind11::class_<Omnia::Timer, Omnia::Component, std::shared_ptr<Omnia::Timer>>(m, Omnia::Timer::TYPE_STRING)
		.def("start", &Omnia::Timer::start)
		.def("stop", &Omnia::Timer::stop)
		.def("is_finished", &Omnia::Timer::is_finished);
	pybind11::class_<Omnia::PhysicsBody, Omnia::Component, std::shared_ptr<Omnia::PhysicsBody>>(m, Omnia::PhysicsBody::TYPE_STRING)
		.def_readwrite("linear_velocity", &Omnia::PhysicsBody::linear_velocity)
		.def("accelerate", pybind11::overload_cast<glm::vec3, float>(&Omnia::PhysicsBody::accelerate))
		.def("accelerate", pybind11::overload_cast<glm::vec3, float, float>(& Omnia::PhysicsBody::accelerate))
		.def("accelerate_x", &Omnia::PhysicsBody::accelerate_x)
		.def("accelerate_y", &Omnia::PhysicsBody::accelerate_y)
		.def("accelerate_z", &Omnia::PhysicsBody::accelerate_z)
		.def("decelerate", &Omnia::PhysicsBody::decelerate)
		.def("decelerate_x", &Omnia::PhysicsBody::decelerate_x)
		.def("decelerate_y", &Omnia::PhysicsBody::decelerate_y)
		.def("decelerate_z", &Omnia::PhysicsBody::decelerate_z);
	pybind11::class_<Omnia::Light, Omnia::Component, std::shared_ptr<Omnia::Light>>(m, Omnia::Light::TYPE_STRING);
	pybind11::class_<Omnia::Model, Omnia::RenderableComponent, std::shared_ptr<Omnia::Model>>(m, Omnia::Model::TYPE_STRING)
		.def("set_to_cube", &Omnia::Model::set_to_cube)
		.def("set_to_textured_cube", &Omnia::Model::set_to_textured_cube);
	pybind11::class_<Omnia::PropertyAnimation, Omnia::Component, std::shared_ptr<Omnia::PropertyAnimation>>(m, Omnia::PropertyAnimation::TYPE_STRING);
	pybind11::class_<Omnia::Collider, Omnia::Component, std::shared_ptr<Omnia::Collider>>(m, Omnia::Collider::TYPE_STRING);
	pybind11::class_<Omnia::Sprite, Omnia::RenderableComponent, std::shared_ptr<Omnia::Sprite>>(m, Omnia::Sprite::TYPE_STRING);
	pybind11::class_<Omnia::Transform, Omnia::Component, std::shared_ptr<Omnia::Transform>>(m, Omnia::Transform::TYPE_STRING)
		.def_readwrite("translation", &Omnia::Transform::translation)
		.def_readwrite("rotation", &Omnia::Transform::rotation)
		.def_readwrite("scale", &Omnia::Transform::scale)
		.def("translate_x", &Omnia::Transform::translate_x)
		.def("translate_y", &Omnia::Transform::translate_y)
		.def("translate_z", &Omnia::Transform::translate_z)
		.def("rotate_x", &Omnia::Transform::rotate_x)
		.def("rotate_y", &Omnia::Transform::rotate_y)
		.def("rotate_z", &Omnia::Transform::rotate_z);
	pybind11::class_<Omnia::GUI, Omnia::RenderableComponent, std::shared_ptr<Omnia::GUI>>(m, Omnia::GUI::TYPE_STRING)
		.def("set_as_text", &Omnia::GUI::set_as_text);
	pybind11::class_<Omnia::Viewport, Omnia::Component, std::shared_ptr<Omnia::Viewport>>(m, Omnia::Viewport::TYPE_STRING);

	/*Utility classes*/
	pybind11::class_<glm::vec2>(m, "Vector2")
		.def_readwrite("x", &glm::vec2::x)
		.def_readwrite("y", &glm::vec2::y);

	pybind11::class_<glm::vec3>(m, "Vector3")
		.def_readwrite("x", &glm::vec3::x)
		.def_readwrite("y", &glm::vec3::y)
		.def_readwrite("z", &glm::vec3::z);

	pybind11::class_<Omnia::HiResTimer, std::shared_ptr<Omnia::HiResTimer>>(m, "HiResTimer")
		.def("set_start", &Omnia::HiResTimer::set_start)
		.def("set_end", &Omnia::HiResTimer::set_end)
		.def("get_delta", &Omnia::HiResTimer::get_delta)
		.def("get_delta_in_seconds", &Omnia::HiResTimer::get_delta_in_seconds)
		.def("get_delta_in_nanoseconds", &Omnia::HiResTimer::get_delta_in_nanoseconds);

	pybind11::class_<Omnia::Colour>(m, "Colour")
		.def(pybind11::init<std::string>())
		.def(pybind11::init<uint8_t, uint8_t, uint8_t, uint8_t>())
		.def("get_red", &Omnia::Colour::get_red)
		.def("get_green", &Omnia::Colour::get_green)
		.def("get_blue", &Omnia::Colour::get_blue)
		.def("get_alpha", &Omnia::Colour::get_alpha);

	pybind11::class_<Omnia::AABB2D>(m, "AABB2D")
		.def_readwrite("max", &Omnia::AABB2D::max)
		.def_readwrite("min", &Omnia::AABB2D::min);

	/*Singletons*/

	pybind11::class_<Omnia::Event::Parameters>(m, "EventParameters")
		.def_readwrite("numbers", &Omnia::Event::Parameters::numbers)
		.def_readwrite("strings", &Omnia::Event::Parameters::strings);

	pybind11::class_<Omnia::Event>(m, "Event")
		.def(pybind11::init<std::string, uint64_t, Omnia::Event::Parameters>())
		.def(pybind11::init<std::string, uint64_t>())
		.def("get_name", &Omnia::Event::get_name)
		.def("get_parameters", &Omnia::Event::get_parameters)
		.def("get_timestamp", &Omnia::Event::get_timestamp);

	m.def("query_event", &Omnia::EventBus::query);
	m.def("query_event_count", &Omnia::EventBus::query_count);
	m.def("publish_event", pybind11::overload_cast<std::string>(&Omnia::EventBus::publish));
	m.def("publish_event", pybind11::overload_cast<std::string, std::unordered_map<std::string, double>, std::unordered_map<std::string, std::string>>(&Omnia::EventBus::publish));
	m.def("publish_event", pybind11::overload_cast<std::string, std::unordered_map<std::string, std::string>>(&Omnia::EventBus::publish));
	m.def("publish_event", pybind11::overload_cast<std::string, std::unordered_map<std::string, double>>(&Omnia::EventBus::publish));

	m.def("preload_scene", pybind11::overload_cast<std::shared_ptr<Omnia::Scene>>(&Omnia::SceneStorage::pre_load_scene));
	m.def("preload_scene", pybind11::overload_cast<std::string>(&Omnia::SceneStorage::pre_load_scene));
	m.def("change_to_scene", pybind11::overload_cast<std::shared_ptr<Omnia::Scene>>(&Omnia::SceneStorage::change_to_scene));
	m.def("change_to_scene", pybind11::overload_cast<std::string>(&Omnia::SceneStorage::change_to_scene));
	m.def("load_scene", pybind11::overload_cast<std::shared_ptr<Omnia::Scene>>(&Omnia::SceneStorage::load_scene));
	m.def("load_scene", pybind11::overload_cast<std::string>(&Omnia::SceneStorage::load_scene));
	m.def("remove_scene", &Omnia::SceneStorage::remove_scene);
	m.def("reload_active_scene", &Omnia::SceneStorage::reload_active_scene);
	m.def("get_active_scene", &Omnia::SceneStorage::get_active_scene);
	m.def("get_active_scene_name", &Omnia::SceneStorage::get_active_scene_name);
	m.def("has_no_scenes", &Omnia::SceneStorage::has_no_scenes);
	m.def("has_scene", &Omnia::SceneStorage::has_scene);
	m.def("has_active_scene_changed", &Omnia::SceneStorage::has_active_scene_changed);
	m.def("clear_scenes", &Omnia::SceneStorage::clear_scenes);
	m.def("get_scene_by_name", &Omnia::SceneStorage::get_scene_by_name);

	m.def("has_component", &Omnia::PythonEntityContext::has_component);
	m.def("get_entity", &Omnia::PythonEntityContext::get_entity);
	m.def("get_component", &Omnia::PythonEntityContext::get_component);
	m.def("get_scene", &Omnia::PythonEntityContext::get_scene);
	m.def("get_scene_layer", &Omnia::PythonEntityContext::get_scene_layer);
	m.def("get_time_delta", &Omnia::PythonEntityContext::get_time_delta);

	m.def("get_shared_library_access", &Omnia::OS::get_shared_library_access, pybind11::return_value_policy::reference);
	m.def("get_file_access", &Omnia::OS::get_file_access, pybind11::return_value_policy::reference);
	m.def("get_input", &Omnia::OS::get_input, pybind11::return_value_policy::reference);
	m.def("get_logger", &Omnia::OS::get_logger, pybind11::return_value_policy::reference);
	m.def("get_network_access", &Omnia::OS::get_network_access, pybind11::return_value_policy::reference);
	m.def("get_platform", &Omnia::OS::get_platform, pybind11::return_value_policy::reference);
	m.def("get_profiler", &Omnia::OS::get_profiler, pybind11::return_value_policy::reference);
	m.def("get_thread_pool", &Omnia::OS::get_thread_pool, pybind11::return_value_policy::reference);
	m.def("get_window", &Omnia::OS::get_window, pybind11::return_value_policy::reference);
	m.def("get_args", &Omnia::OS::get_args);
}