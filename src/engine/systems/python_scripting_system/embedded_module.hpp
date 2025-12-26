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

#ifdef ENABLE_PYTHON_BUILD

#pragma once

#include <foundations/resources/audio_clip.hpp>
#include <foundations/resources/font.hpp>
#include <foundations/resources/image.hpp>
#include <foundations/resources/mesh.hpp>
#include "foundations/resources/script.hpp"
#include <foundations/resources/shader.hpp>
#include <foundations/resources/text.hpp>

#include <scene/components/component.hpp>
#include "python_entity_context.hpp"
#include <pybind11/embed.h>
#include <pybind11/stl.h>
#include <glm/glm.hpp>
#include <scene/scene.hpp>
#include <foundations/aliases.hpp>
#include <foundations/singletons/event_bus.hpp>
#include <foundations/singletons/profiler.hpp>
#include <foundations/singletons/thread_pool.hpp>
#include <memory>

#include <customization/class_registry.hpp>

PYBIND11_EMBEDDED_MODULE(omnific, m) 
{
		/*Utility classes*/
		pybind11::class_<glm::vec2>(m, "Vector2")
		.def_readwrite("x", &glm::vec2::x)
		.def_readwrite("y", &glm::vec2::y);

	pybind11::class_<glm::vec3>(m, "Vector3")
		.def_readwrite("x", &glm::vec3::x)
		.def_readwrite("y", &glm::vec3::y)
		.def_readwrite("z", &glm::vec3::z);

	pybind11::class_<Omnific::Transform, std::shared_ptr<Omnific::Transform>>(m, "Transform")
		.def_readwrite("translation", &Omnific::Transform::translation)
		.def_readwrite("rotation", &Omnific::Transform::rotation)
		.def_readwrite("scale", &Omnific::Transform::scale)
		.def("translate_x", &Omnific::Transform::translate_x)
		.def("translate_y", &Omnific::Transform::translate_y)
		.def("translate_z", &Omnific::Transform::translate_z)
		.def("rotate_x", &Omnific::Transform::rotate_x)
		.def("rotate_y", &Omnific::Transform::rotate_y)
		.def("rotate_z", &Omnific::Transform::rotate_z)
		.def("get_up_vector", &Omnific::Transform::get_up_vector)
		.def("get_front_vector", &Omnific::Transform::get_front_vector)
		.def("get_right_vector", &Omnific::Transform::get_right_vector)
		.def("get_rotation_in_euler_angles", &Omnific::Transform::get_rotation_in_euler_angles)
		.def("get_rotation_in_radians_euler_angles", &Omnific::Transform::get_rotation_in_radians_euler_angles)
		.def("get_rotation_in_degrees_euler_angles", &Omnific::Transform::get_rotation_in_degrees_euler_angles)
		.def("set_xyz_scale", &Omnific::Transform::set_xyz_scale)
		.def("set_x_rotation", &Omnific::Transform::set_x_rotation)
		.def("set_y_rotation", &Omnific::Transform::set_y_rotation)
		.def("set_z_rotation", &Omnific::Transform::set_z_rotation);

	pybind11::class_<Omnific::Clock, std::shared_ptr<Omnific::Clock>>(m, "Clock")
		.def(pybind11::init<std::string>())
		.def("set_start", &Omnific::Clock::set_start)
		.def("set_end", &Omnific::Clock::set_end)
		.def("get_delta", &Omnific::Clock::get_delta)
		.def("get_delta_in_seconds", &Omnific::Clock::get_delta_in_seconds);

	pybind11::class_<Omnific::Colour>(m, "Colour")
		.def(pybind11::init<std::string>())
		.def(pybind11::init<uint8_t, uint8_t, uint8_t, uint8_t>())
		.def("get_red", &Omnific::Colour::get_red)
		.def("get_green", &Omnific::Colour::get_green)
		.def("get_blue", &Omnific::Colour::get_blue)
		.def("get_alpha", &Omnific::Colour::get_alpha);

	pybind11::class_<Omnific::AABB2D>(m, "AABB2D")
		.def_readwrite("max", &Omnific::AABB2D::max)
		.def_readwrite("min", &Omnific::AABB2D::min);

	/*OS Classes*/

	pybind11::class_<Omnific::SharedLibraryAccess>(m, "SharedLibraryAccess");

	pybind11::class_<Omnific::FileAccess>(m, "FileAccess");

	pybind11::class_<Omnific::Inputs>(m, "Input")
		.def("is_on_press", pybind11::overload_cast<std::string>(&Omnific::Inputs::is_on_press))
		.def("is_on_press", pybind11::overload_cast<std::vector<std::string>>(&Omnific::Inputs::is_on_press))
		.def("is_on_press", pybind11::overload_cast<std::vector<std::string>, Omnific::PlayerID>(&Omnific::Inputs::is_on_press))
		.def("is_on_double_press", pybind11::overload_cast<std::string, unsigned int>(&Omnific::Inputs::is_on_double_press))
		.def("is_on_double_press", pybind11::overload_cast<std::vector<std::string>, unsigned int>(&Omnific::Inputs::is_on_double_press))
		.def("is_on_double_press", pybind11::overload_cast<std::vector<std::string>, unsigned int>(&Omnific::Inputs::is_on_double_press))
		.def("is_pressed", pybind11::overload_cast<std::string>(&Omnific::Inputs::is_pressed))
		.def("is_pressed", pybind11::overload_cast<std::vector<std::string>>(&Omnific::Inputs::is_pressed))
		.def("is_pressed", pybind11::overload_cast<std::vector<std::string>, Omnific::PlayerID>(&Omnific::Inputs::is_pressed))
		.def("is_on_release", pybind11::overload_cast<std::string>(&Omnific::Inputs::is_on_release))
		.def("is_on_release", pybind11::overload_cast<std::vector<std::string>>(&Omnific::Inputs::is_on_release))
		.def("is_on_release", pybind11::overload_cast<std::vector<std::string>, Omnific::PlayerID>(&Omnific::Inputs::is_on_release))
		.def("is_released", pybind11::overload_cast<std::string>(&Omnific::Inputs::is_released))
		.def("is_released", pybind11::overload_cast<std::string, Omnific::PlayerID>(&Omnific::Inputs::is_released))
		.def("get_axis", pybind11::overload_cast<std::string>(&Omnific::Inputs::get_axis))
		.def("get_axis", pybind11::overload_cast<std::string, Omnific::PlayerID>(&Omnific::Inputs::get_axis))
		.def("is_left_mouse_button_on_press", &Omnific::Inputs::is_left_mouse_button_on_press)
		.def("is_left_mouse_button_pressed", &Omnific::Inputs::is_left_mouse_button_pressed)
		.def("is_left_mouse_button_on_release", &Omnific::Inputs::is_left_mouse_button_on_release)
		.def("is_left_mouse_button_double_clicked", &Omnific::Inputs::is_left_mouse_button_double_clicked)
		.def("is_middle_mouse_button_on_press", &Omnific::Inputs::is_middle_mouse_button_on_press)
		.def("is_middle_mouse_button_pressed", &Omnific::Inputs::is_middle_mouse_button_pressed)
		.def("is_middle_mouse_button_on_release", &Omnific::Inputs::is_middle_mouse_button_on_release)
		.def("is_middle_mouse_button_double_clicked", &Omnific::Inputs::is_middle_mouse_button_double_clicked)
		.def("is_right_mouse_button_on_press", &Omnific::Inputs::is_right_mouse_button_on_press)
		.def("is_right_mouse_button_pressed", &Omnific::Inputs::is_right_mouse_button_pressed)
		.def("is_right_mouse_button_on_release", &Omnific::Inputs::is_right_mouse_button_on_release)
		.def("is_right_mouse_button_double_clicked", &Omnific::Inputs::is_right_mouse_button_double_clicked)
		.def("get_mouse_position", &Omnific::Inputs::get_mouse_position)
		.def("get_mouse_wheel_velocity", &Omnific::Inputs::get_mouse_wheel_velocity)
		.def("get_mouse_motion_velocity", &Omnific::Inputs::get_mouse_motion_velocity)
		.def("is_action_on_press", &Omnific::Inputs::is_action_on_press)
		.def("is_action_on_double_press", &Omnific::Inputs::is_action_on_double_press)
		.def("is_action_pressed", &Omnific::Inputs::is_action_pressed)
		.def("is_action_on_release", &Omnific::Inputs::is_action_on_release)
		.def("is_action_released", &Omnific::Inputs::is_action_released)
		.def("get_action_axis", &Omnific::Inputs::get_action_axis)
		.def("set_relative_mouse_mode", &Omnific::Inputs::set_relative_mouse_mode)
		.def("request_shutdown", &Omnific::Inputs::request_shutdown)
		.def("request_restart", &Omnific::Inputs::request_restart);

	pybind11::class_<Omnific::Logger>(m, "Logger")
		.def("write", &Omnific::Logger::write)
		.def("write_to_file", &Omnific::Logger::write_to_file)
		.def("get_last_message", &Omnific::Logger::get_last_message)
		.def("get_logs", &Omnific::Logger::get_logs);

	pybind11::class_<Omnific::NetworkAccess>(m, "NetworkAccess");

	pybind11::class_<Omnific::Platform>(m, "Platform")
		.def("get_logical_core_count", &Omnific::Platform::get_logical_core_count)
		.def("get_l1_cache_line_size", &Omnific::Platform::get_l1_cache_line_size)
		.def("get_system_ram", &Omnific::Platform::get_system_ram)
		.def("get_os_name", &Omnific::Platform::get_platform_name);

	pybind11::class_<Omnific::Profiler>(m, "Profiler")
		.def("add_timer", pybind11::overload_cast<std::string>(&Omnific::Profiler::add_clock))
		.def("add_timer", pybind11::overload_cast<std::string, std::vector<std::string>, bool>(&Omnific::Profiler::add_clock))
		.def("get_timer", &Omnific::Profiler::get_clock);

	pybind11::class_<Omnific::ThreadPool>(m, "ThreadPool");

	pybind11::class_<Omnific::Window>(m, "Window")
		.def("set_to_windowed", &Omnific::Window::set_to_windowed)
		.def("set_to_fullscreen", &Omnific::Window::set_to_fullscreen)
		.def("toggle_windowed_fullscreen", &Omnific::Window::toggle_windowed_fullscreen)
		.def("resize", &Omnific::Window::resize)
		.def("change_title", &Omnific::Window::change_title)
		.def("change_icon", &Omnific::Window::change_icon)
		.def("maximize", &Omnific::Window::maximize)
		.def("minimize", &Omnific::Window::minimize)
		.def("raise", &Omnific::Window::raise)
		.def("restore", &Omnific::Window::restore)
		.def("hide", &Omnific::Window::hide)
		.def("show", &Omnific::Window::show)
		.def("get_window_size", &Omnific::Window::get_window_size)
		.def("get_window_position", &Omnific::Window::get_window_position);

	/*Scene classes*/

	pybind11::class_<Omnific::Entity, std::shared_ptr<Omnific::Entity>>(m, "Entity")
		.def("get_id", &Omnific::Entity::get_id)
		.def("get_name", &Omnific::Entity::get_name)
		.def("get_transform", &Omnific::Entity::get_transform)
		.def_readwrite("parent_id", &Omnific::Entity::parent_id)
		.def_readwrite("child_ids", &Omnific::Entity::child_ids)
		.def("get_tags", &Omnific::Entity::get_tags);

	pybind11::class_<Omnific::Scene, std::shared_ptr<Omnific::Scene>>(m, "Scene")
		.def(pybind11::init<std::string>())
		.def("add_entity", &Omnific::Scene::add_entity)
		.def("add_empty_entity", &Omnific::Scene::add_empty_entity)
		.def("add_entity_to_parent_entity", &Omnific::Scene::add_entity_to_parent_entity)
		.def("add_entity_to_parent_entity_by_name", &Omnific::Scene::add_entity_to_parent_entity_by_name)
		.def("merge_another_scene_to_parent_entity", &Omnific::Scene::merge_another_scene_to_parent_entity)
		.def("merge_another_scene_to_parent_entity_by_name", &Omnific::Scene::merge_another_scene_to_parent_entity_by_name)
		.def("add_component", &Omnific::Scene::add_component)
		.def("add_component_to_last_entity", &Omnific::Scene::add_component_to_last_entity)
		.def("remove_entity", &Omnific::Scene::remove_entity)
		.def("remove_component", &Omnific::Scene::remove_component)
		.def("get_component", pybind11::overload_cast<std::string, Omnific::EntityID>(&Omnific::Scene::get_component))
		.def("get_components", &Omnific::Scene::get_components)
		.def("get_entity", &Omnific::Scene::get_entity)
		.def("get_entity_by_name", &Omnific::Scene::get_entity_by_name)
		.def("get_last_entity", &Omnific::Scene::get_last_entity)
		.def("get_entities", &Omnific::Scene::get_entities)
		.def("get_id", &Omnific::Scene::get_id);

	/*Resource classes*/

	pybind11::class_<Omnific::Resource, std::shared_ptr<Omnific::Resource>>(m, "Resource")
		.def("get_id", &Omnific::Resource::get_id)
		.def("set_name", &Omnific::Resource::set_name)
		.def("get_name", &Omnific::Resource::get_name)
		.def("get_type", &Omnific::Resource::get_type);

	pybind11::class_<Omnific::AudioClip, Omnific::Resource, std::shared_ptr<Omnific::AudioClip>>(m, Omnific::AudioClip::TYPE_STRING)
		.def("get_sample_rate", &Omnific::AudioClip::get_sample_rate)
		.def("get_channel_count", &Omnific::AudioClip::get_channel_count)
		.def("get_playback_length", &Omnific::AudioClip::get_playback_length)
		.def("get_spectrum", &Omnific::AudioClip::get_spectrum);

	pybind11::class_<Omnific::Font, Omnific::Resource, std::shared_ptr<Omnific::Font>>(m, Omnific::Font::TYPE_STRING);
	pybind11::class_<Omnific::Image, Omnific::Resource, std::shared_ptr<Omnific::Image>>(m, Omnific::Image::TYPE_STRING);
	pybind11::class_<Omnific::Text, Omnific::Resource, std::shared_ptr<Omnific::Text>>(m, Omnific::Text::TYPE_STRING);

	/*Component classes*/
	pybind11::class_<Omnific::Component, std::shared_ptr<Omnific::Component>>(m, Omnific::Component::TYPE_STRING)
		.def("get_id", &Omnific::Component::get_id)
		.def("get_entity_id", &Omnific::Component::get_entity_id)
		.def("get_entity_name", &Omnific::Component::get_entity_name)
		.def("is_attached_to_entity", &Omnific::Component::is_attached_to_entity)
		.def("get_type", &Omnific::Component::get_type)
		.def("is_renderable", &Omnific::Component::is_renderable);

	pybind11::class_<Omnific::ShaderParameters, std::shared_ptr<Omnific::ShaderParameters>>(m, "ShaderParameters")
		.def("set_int_uniform", &Omnific::ShaderParameters::set_int_uniform)
		.def("set_bool_uniform", &Omnific::ShaderParameters::set_bool_uniform)
		.def("set_float_uniform", &Omnific::ShaderParameters::set_float_uniform)
		.def("set_vec2_uniform", &Omnific::ShaderParameters::set_vec2_uniform)
		.def("set_vec3_uniform", &Omnific::ShaderParameters::set_vec3_uniform)
		.def("set_vec4_uniform", &Omnific::ShaderParameters::set_vec4_uniform)
		.def("set_mat4_uniform", &Omnific::ShaderParameters::set_mat4_uniform);

	pybind11::class_<Omnific::AudioListener, Omnific::Component, std::shared_ptr<Omnific::AudioListener>>(m, Omnific::AudioListener::TYPE_STRING);
	pybind11::class_<Omnific::AudioSource, Omnific::Component, std::shared_ptr<Omnific::AudioSource>>(m, Omnific::AudioSource::TYPE_STRING)
		.def("clear_audio_clip", &Omnific::AudioSource::clear_audio_clip)
		.def("remove_audio_clip", &Omnific::AudioSource::remove_audio_clip)
		.def("play_audio_clip", &Omnific::AudioSource::play_audio_clip)
		.def("play_audio_clip_infinitely", &Omnific::AudioSource::play_audio_clip_infinitely)
		.def("play", &Omnific::AudioSource::play)
		.def("play_infinitely", &Omnific::AudioSource::play_infinitely)
		.def("stop", &Omnific::AudioSource::stop)
		.def("resume", &Omnific::AudioSource::resume)
		.def("pause", &Omnific::AudioSource::pause)
		.def("reset", &Omnific::AudioSource::reset)
		.def("jump", &Omnific::AudioSource::jump)
		.def("set_volume", &Omnific::AudioSource::set_volume)
		.def("get_volume", &Omnific::AudioSource::get_volume)
		.def("get_current_playback_time", &Omnific::AudioSource::get_current_playback_time)
		.def("get_playback_state", &Omnific::AudioSource::get_playback_state)
		.def("get_audio_clip_names", &Omnific::AudioSource::get_audio_clip_names)
		.def("get_active_audio_clip", &Omnific::AudioSource::get_active_audio_clip)
		.def("get_audio_clip_by_name", &Omnific::AudioSource::get_audio_clip_by_name);
	pybind11::class_<Omnific::Camera, Omnific::Component, std::shared_ptr<Omnific::Camera>>(m, Omnific::Camera::TYPE_STRING)
		.def("toggle_wireframe_mode", &Omnific::Camera::toggle_wireframe_mode)
		.def("set_controller_state_by_string", &Omnific::Camera::set_controller_state_by_string)
		.def_readwrite("clear_colour", &Omnific::Camera::clear_colour)
		.def_readwrite("controller_state", &Omnific::Camera::controller_state);
	pybind11::class_<Omnific::Timer, Omnific::Component, std::shared_ptr<Omnific::Timer>>(m, Omnific::Timer::TYPE_STRING)
		.def("start", &Omnific::Timer::start)
		.def("stop", &Omnific::Timer::stop)
		.def("is_finished", &Omnific::Timer::is_finished);
	pybind11::class_<Omnific::PhysicsBody, Omnific::Component, std::shared_ptr<Omnific::PhysicsBody>>(m, Omnific::PhysicsBody::TYPE_STRING)
		.def_readwrite("linear_velocity", &Omnific::PhysicsBody::linear_velocity)
		.def("accelerate", pybind11::overload_cast<glm::vec3, float>(&Omnific::PhysicsBody::accelerate))
		.def("accelerate", pybind11::overload_cast<glm::vec3, float, float>(& Omnific::PhysicsBody::accelerate))
		.def("accelerate_x", &Omnific::PhysicsBody::accelerate_x)
		.def("accelerate_y", &Omnific::PhysicsBody::accelerate_y)
		.def("accelerate_z", &Omnific::PhysicsBody::accelerate_z)
		.def("decelerate", &Omnific::PhysicsBody::decelerate)
		.def("decelerate_x", &Omnific::PhysicsBody::decelerate_x)
		.def("decelerate_y", &Omnific::PhysicsBody::decelerate_y)
		.def("decelerate_z", &Omnific::PhysicsBody::decelerate_z);
	pybind11::class_<Omnific::Light, Omnific::Component, std::shared_ptr<Omnific::Light>>(m, Omnific::Light::TYPE_STRING);
	pybind11::class_<Omnific::Renderable, Omnific::Component, std::shared_ptr<Omnific::Renderable>>(m, Omnific::Renderable::TYPE_STRING)
		.def_readwrite("shader_parameters", &Omnific::Renderable::shader_parameters)
		.def("set_to_cube", &Omnific::Renderable::set_to_cube)
		.def("set_to_textured_cube", &Omnific::Renderable::set_to_textured_cube)
		.def("get_surface_mode_string", &Omnific::Renderable::get_reflection_models_as_string)
		.def("show", &Omnific::Renderable::show)
		.def("hide", &Omnific::Renderable::hide);
	pybind11::class_<Omnific::Animator, Omnific::Component, std::shared_ptr<Omnific::Animator>>(m, Omnific::Animator::TYPE_STRING);
	pybind11::class_<Omnific::Collider, Omnific::Component, std::shared_ptr<Omnific::Collider>>(m, Omnific::Collider::TYPE_STRING);
	pybind11::class_<Omnific::Sprite, Omnific::Renderable, std::shared_ptr<Omnific::Sprite>>(m, Omnific::Sprite::TYPE_STRING);
	pybind11::class_<Omnific::GUI, Omnific::Renderable, std::shared_ptr<Omnific::GUI>>(m, Omnific::GUI::TYPE_STRING)
		.def("set_to_label", &Omnific::GUI::set_to_label);
	pybind11::class_<Omnific::Viewport, Omnific::Component, std::shared_ptr<Omnific::Viewport>>(m, Omnific::Viewport::TYPE_STRING);

	/*Singletons*/

	pybind11::class_<Omnific::Event::Parameters>(m, "EventParameters")
		.def(pybind11::init<>())
		.def_readwrite("numbers", &Omnific::Event::Parameters::numbers)
		.def_readwrite("strings", &Omnific::Event::Parameters::strings)
		.def_readwrite("bools", &Omnific::Event::Parameters::bools)
		.def_readwrite("components", &Omnific::Event::Parameters::components)
		.def_readwrite("key", &Omnific::Event::Parameters::key);

	pybind11::class_<Omnific::Event>(m, "Event")
		.def(pybind11::init<std::string, Omnific::Event::Parameters>())
		.def(pybind11::init<std::string>())
		.def(pybind11::init<>())
		.def("get_name", &Omnific::Event::get_name)
		.def("get_parameters", &Omnific::Event::get_parameters)
		.def("get_timestamp", &Omnific::Event::get_timestamp);

	m.def("query_events", &Omnific::EventBus::query_events);
	m.def("remove_continuous_event", &Omnific::EventBus::remove_continuous_event);
	m.def("query_continuous_events", &Omnific::EventBus::query_continuous_events);
	m.def("query_continuous_event", &Omnific::EventBus::query_continuous_event);
	m.def("query_events_with_number_parameter",  &Omnific::EventBus::query_events_with_number_parameter);
	m.def("query_events_with_string_parameter",  &Omnific::EventBus::query_events_with_string_parameter);
	m.def("query_events_with_bool_parameter",  &Omnific::EventBus::query_events_with_bool_parameter);
	m.def("query_events_with_component_parameter",  &Omnific::EventBus::query_events_with_component_parameter);
	m.def("has_continuous_event", &Omnific::EventBus::has_continuous_event);
	m.def("query_event_count_with_parameter_key", &Omnific::EventBus::query_event_count_with_parameter_key);
	m.def("query_event_count", &Omnific::EventBus::query_event_count);
	m.def("has_event_with_parameter_key", &Omnific::EventBus::has_event_with_parameter_key);
	m.def("has_event", &Omnific::EventBus::has_event);
	m.def("publish_event", pybind11::overload_cast<Omnific::Event, bool>(&Omnific::EventBus::publish_event));
	m.def("publish_event", pybind11::overload_cast<std::string, std::unordered_map<std::string, std::string>, std::unordered_map<std::string, double>, std::unordered_map<std::string, bool>, std::unordered_map<std::string, std::shared_ptr<Omnific::Component>>, std::string, bool>(&Omnific::EventBus::publish_event));

	m.def("load_scene", pybind11::overload_cast<std::shared_ptr<Omnific::Scene>>(&Omnific::SceneStorage::load_scene));
	m.def("load_scene", pybind11::overload_cast<std::string>(&Omnific::SceneStorage::load_scene));
	m.def("remove_scene", &Omnific::SceneStorage::remove_scene);
	m.def("reload_active_scene", &Omnific::SceneStorage::reload_active_scene);
	m.def("get_active_scene", &Omnific::SceneStorage::get_active_scene);
	m.def("get_active_scene_name", &Omnific::SceneStorage::get_active_scene_name);
	m.def("has_no_scenes", &Omnific::SceneStorage::has_no_scenes);
	m.def("has_scene", &Omnific::SceneStorage::has_scene);
	m.def("has_active_scene_changed", &Omnific::SceneStorage::has_active_scene_changed);
	m.def("clear_scenes", &Omnific::SceneStorage::clear_scenes);
	m.def("get_scene_by_name", &Omnific::SceneStorage::get_scene_by_name);

	m.def("has_component", &Omnific::PythonEntityContext::has_component);
	m.def("get_entity", &Omnific::PythonEntityContext::get_entity);
	m.def("get_component", &Omnific::PythonEntityContext::get_component);
	m.def("get_transform", &Omnific::PythonEntityContext::get_transform);
	m.def("get_scene", &Omnific::PythonEntityContext::get_scene);
	m.def("get_time_delta", &Omnific::PythonEntityContext::get_time_delta);

	m.def("get_shared_library_access", &Omnific::Platform::get_shared_library_access, pybind11::return_value_policy::reference);
	m.def("get_file_access", &Omnific::Platform::get_file_access, pybind11::return_value_policy::reference);
	m.def("get_inputs", &Omnific::Platform::get_inputs, pybind11::return_value_policy::reference);
	m.def("get_logger", &Omnific::Platform::get_logger, pybind11::return_value_policy::reference);
	m.def("get_network_access", &Omnific::Platform::get_network_access, pybind11::return_value_policy::reference);
	m.def("get_window", &Omnific::Platform::get_window, pybind11::return_value_policy::reference);
	m.def("get_args", &Omnific::Platform::get_args);
}

#endif /*ENABLE_PYTHON_BUILD*/
