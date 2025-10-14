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

#include "rendering_system.hpp"
#include <foundations/resources/shader.hpp>
#include <foundations/resources/image.hpp>
#include <foundations/singletons/platform/platform.hpp>
#include <scene/components/camera.hpp>
#include <scene/components/viewport.hpp>
#include <foundations/transform.hpp>
#include <scene/components/gui.hpp>
#include <scene/components/light.hpp>
#include <foundations/singletons/configuration.hpp>
#include <foundations/singletons/profiler.hpp>
#include <foundations/singletons/scene_storage.hpp>

#define RENDERING_SYSTEM_ON_OUTPUT_FRAME_TIME_CLOCK_NAME "rendering_system_on_output_frame_time"

Omnific::RenderingSystem::RenderingSystem()
{
	Platform::create_window("",
		640,
		480,
		false,
		this->get_rendering_backend_name());

	this->dummy_light = std::shared_ptr<Light>(new Light());
	this->dummy_light_transform = std::shared_ptr<Transform>(new Transform());
	this->dummy_light_transform->translate_y(5.0);
	this->dummy_light_transform->translate_z(5.0);
	this->dummy_light_transform->rotate_x(-45.0);
	this->opengl_backend = std::shared_ptr<OpenGLRenderingBackend>(new OpenGLRenderingBackend());
	this->type = TYPE_STRING;
}

Omnific::RenderingSystem::~RenderingSystem()
{
	this->finalize();
}

void Omnific::RenderingSystem::initialize()
{
	Image image = Image(
		Platform::get_file_access().find_path(Configuration::get_instance()->metadata.icon_filepath));
	Platform::get_window().change_icon(
		image.get_data(),
		image.get_width(),
		image.get_height(),
		image.get_depth(),
		image.get_pitch());

	this->opengl_backend->initialize();
	this->is_initialized = true;
	Profiler::add_clock(RENDERING_SYSTEM_ON_OUTPUT_FRAME_TIME_CLOCK_NAME, {"rendering_system", "on_output_frame_time"});
	Platform::get_logger().write("Initialized Rendering System");
}

void Omnific::RenderingSystem::on_output()
{
	std::shared_ptr<Clock> frame_time_clock = Profiler::get_clock(RENDERING_SYSTEM_ON_OUTPUT_FRAME_TIME_CLOCK_NAME);
	frame_time_clock->set_start();
	std::shared_ptr<Scene> scene = SceneStorage::get_active_scene();
	std::vector<size_t> rendering_order_index_cache = scene->get_rendering_order_index_cache();
	this->on_window_resize();
	this->opengl_backend->clear_colour_buffer(0, 0, 0, 255);

	/* Render all models except GUIs */
	for (auto& viewport : scene->get_components_by_type<Viewport>())
	{
		std::shared_ptr<Entity> camera_entity = scene->get_entity_by_name(viewport->get_camera_entity_name());
		if (camera_entity != nullptr)
		{
			std::shared_ptr<Camera> camera = scene->get_component_by_type<Camera>(camera_entity->get_id());
			if (camera != nullptr)
			{
				if (camera->get_is_streaming())
				{
					std::shared_ptr<Transform> camera_transform = camera_entity->get_transform();
					std::shared_ptr<Transform> camera_global_transform = scene->calculate_global_transform(camera->get_entity_id());
					glm::mat4 world_to_view_matrix = glm::inverse(camera_global_transform->get_transform_matrix());
					glm::mat4 view_to_projection_matrix = camera->get_view_to_projection_matrix();

					if (camera_entity->is_2d)
					{
						this->opengl_backend->disable_depth_test();
					}
					else
					{
						this->opengl_backend->clear_depth_buffer();
						this->opengl_backend->enable_depth_test();
					}

					if (camera->get_is_wireframe_mode())
						this->opengl_backend->enable_wireframe_mode();
					else
						this->opengl_backend->disable_wireframe_mode();


					std::vector<std::shared_ptr<Light>> lights = scene->get_components_by_type<Light>();

					if (lights.size() == 0)
						lights.push_back(this->dummy_light);

					std::vector<int> light_modes;
					std::vector<glm::vec3> light_colours;
					std::vector<glm::vec3> shadow_colours;
					std::vector<float> light_intensities;
					std::vector<float> light_ranges;
					std::vector<float> light_inner_cutoff_angles;
					std::vector<float> light_outer_cutoff_angles;
					std::vector<bool> are_shadows_enabled;
					std::vector<glm::vec3> light_translations;
					std::vector<glm::vec3> light_rotations;
					size_t lights_count = lights.size();

					for (auto& light: lights)
					{
						std::shared_ptr<Transform> light_transform; 
						std::shared_ptr<Entity> light_entity = scene->get_entity(light->get_entity_id());

						if (light_entity == nullptr)
							light_transform = this->dummy_light_transform;
						else
							light_transform = scene->calculate_global_transform(light->get_entity_id());

						light_modes.push_back((int)light->mode);
						light_colours.push_back(light->colour->get_rgb_in_vec3());
						shadow_colours.push_back(light->shadow_colour->get_rgb_in_vec3());
						light_intensities.push_back(light->intensity);
						light_ranges.push_back(light->range);
						light_inner_cutoff_angles.push_back(glm::radians(light->inner_cutoff_angle));
						light_outer_cutoff_angles.push_back(glm::radians(light->outer_cutoff_angle));
						are_shadows_enabled.push_back(light->is_shadow_enabled);
						light_translations.push_back(light_transform->translation);
						light_rotations.push_back(glm::vec3(
								glm::radians(light_transform->rotation.x),
								glm::radians(light_transform->rotation.y),
								glm::radians(light_transform->rotation.z)));					
					}

					if (this->rendering_path == RenderingPath::FORWARD)
					{
						for (auto& renderable : scene->get_renderables_in_rendering_order())
						{
							std::shared_ptr<Entity> renderable_entity = scene->get_entity(renderable->get_entity_id());
							std::shared_ptr<Transform> renderable_transform = renderable_entity->get_transform();

							if (renderable_entity->is_2d == camera_entity->is_2d &&
								(!renderable->is_type(GUI::TYPE_STRING) || !renderable_entity->is_2d) &&
								!renderable->is_hidden() &&
								renderable->mesh != nullptr)
							{
								this->opengl_backend->enable_blending();
								std::shared_ptr<Shader> shader = renderable->get_shader();
								std::shared_ptr<ShaderParameters> shader_parameters = renderable->shader_parameters;

								std::shared_ptr<Entity> top_entity = renderable_entity;
								EntityID parent_entity_id = renderable_entity->parent_id;

								/*Find the top entity of the hierarchy for the
									overriding shader. */
								while (parent_entity_id != 0)
								{
									top_entity = scene->get_entity(parent_entity_id);
									parent_entity_id = top_entity->parent_id;
								}

								if (top_entity->get_renderable_id() != 0)
								{
									std::shared_ptr<Renderable> overriding_renderable =
										std::dynamic_pointer_cast<Renderable>(scene->get_component_by_id(top_entity->get_renderable_id()));

									std::shared_ptr<Shader> overriding_shader = overriding_renderable->get_overriding_shader();
									
									if (overriding_shader != nullptr)
									{
										shader = overriding_shader;
										shader_parameters = overriding_renderable->shader_parameters;
									}
								}

								std::shared_ptr<Transform> renderable_global_transform = scene->calculate_global_transform(renderable_entity->get_id());
								glm::mat4 model_to_world_matrix = renderable_global_transform->get_transform_matrix();
								glm::mat4 mvp = view_to_projection_matrix * world_to_view_matrix * model_to_world_matrix;
								float alpha = renderable->get_alpha_in_percentage();
								const float cull_alpha_threshold = 1.0 - 0.001;
								Renderable::FaceCullMode face_cull_mode = renderable->get_face_cull_mode();

								switch (face_cull_mode)
								{
									case Renderable::FaceCullMode::NONE:
									case Renderable::FaceCullMode::BACK: this->opengl_backend->set_face_culling_to_back(); break;
									case Renderable::FaceCullMode::FRONT: this->opengl_backend->set_face_culling_to_front(); break;
									case Renderable::FaceCullMode::FRONT_AND_BACK: this->opengl_backend->set_face_culling_to_front_and_back(); break;
								}

								if (face_cull_mode == Renderable::FaceCullMode::NONE)
									this->opengl_backend->disable_face_culling();
								else
									this->opengl_backend->enable_face_culling();

								
								std::shared_ptr<OpenGLVertexArray> vertex_array = this->opengl_backend->get_vertex_array(renderable->mesh);
								vertex_array->bind();

								std::shared_ptr<Material> material = renderable->material;
								this->opengl_backend->get_texture(material->albedo_map)->bind(OpenGLTexture::Unit::_0);
								this->opengl_backend->get_texture(material->metallic_map)->bind(OpenGLTexture::Unit::_1);
								this->opengl_backend->get_texture(material->roughness_map)->bind(OpenGLTexture::Unit::_2);
								this->opengl_backend->get_texture(material->emission_map)->bind(OpenGLTexture::Unit::_3);
								this->opengl_backend->get_texture(material->normal_map)->bind(OpenGLTexture::Unit::_4);
								this->opengl_backend->get_texture(material->occlusion_map)->bind(OpenGLTexture::Unit::_5);

								std::shared_ptr<OpenGLShaderProgram> shader_program;

								if (shader != nullptr)
								{
									AssetID shader_id = shader->get_id();
									std::string default_vertex_input;
									std::string default_fragment_input;

									if (renderable_entity->is_2d)
									{
										default_vertex_input = this->opengl_backend->get_default_2d_vertex_input();
										default_fragment_input = this->opengl_backend->get_default_2d_fragment_input();
									}
									else
									{
										default_vertex_input = this->opengl_backend->get_default_3d_vertex_input();
										default_fragment_input = this->opengl_backend->get_default_3d_fragment_input();
									}

									if (!this->opengl_backend->shader_programs.count(shader_id))
									{
										std::shared_ptr<Shader> complete_shader;

										//Check for a selected Shader preset. Otherwise, load custom shaders.
										std::string preset = shader->get_preset();

										if (renderable_entity->is_2d || (!renderable_entity->is_2d && preset == "Shader::CUSTOM"))
										{
											std::string vertex_source_input = default_vertex_input;
											std::string fragment_source_input = default_fragment_input;

											if (shader->get_vertex_source() != "")
												vertex_source_input = shader->get_vertex_source();

											if (shader->get_fragment_source() != "")
												fragment_source_input = shader->get_fragment_source();

											complete_shader = std::shared_ptr<Shader>(new Shader(
												vertex_source_input,
												fragment_source_input,
												false,
												false));
										}
										else if (preset == "Shader::LIGHT_SOURCE")
										{
											complete_shader = std::shared_ptr<Shader>(new Shader(
												default_vertex_input,
												this->opengl_backend->get_light_source_fragment_input(),
												false,
												false));
										}
										else if (preset == "Shader::UNLIT")
										{
											complete_shader = std::shared_ptr<Shader>(new Shader(
												default_vertex_input,
												this->opengl_backend->get_unlit_fragment_input(),
												false,
												false));
										}
										else if (preset == "Shader::SIMPLE")
										{
											complete_shader = std::shared_ptr<Shader>(new Shader(
												default_vertex_input,
												this->opengl_backend->get_simple_fragment_input(),
												false,
												false));
										}
										else if (preset == "Shader::PBR")
										{
											complete_shader = std::shared_ptr<Shader>(new Shader(
												default_vertex_input,
												this->opengl_backend->get_pbr_fragment_input(),
												false,
												false));
										}

										this->opengl_backend->shader_programs.emplace(
											shader_id,
											std::shared_ptr<OpenGLShaderProgram>(new OpenGLShaderProgram(complete_shader)));
									}

									shader_program = this->opengl_backend->shader_programs.at(shader_id);
								}
								else if (renderable_entity->is_2d)
								{
									shader_program = this->opengl_backend->built_in_shader_program_2d;
								}
								else
								{
									shader_program = this->opengl_backend->built_in_shader_program_3d;
								}

								shader_program->use();

								/* Custom uniforms. */
								for (auto const& [name, int_uniform] : shader_parameters->int_uniforms)
									shader_program->set_int(name, int_uniform);

								for (auto const& [name, bool_uniform] : shader_parameters->bool_uniforms)
									shader_program->set_bool(name, bool_uniform);

								for (auto const& [name, float_uniform] : shader_parameters->float_uniforms)
									shader_program->set_float(name, float_uniform);

								for (auto const& [name, vec2_uniform] : shader_parameters->vec2_uniforms)
									shader_program->set_vec2(name, vec2_uniform);

								for (auto const& [name, vec3_uniform] : shader_parameters->vec3_uniforms)
									shader_program->set_vec3(name, vec3_uniform);

								for (auto const& [name, vec4_uniform] : shader_parameters->vec4_uniforms)
									shader_program->set_vec4(name, vec4_uniform);

								for (auto const& [name, mat4_uniform] : shader_parameters->mat4_uniforms)
									shader_program->set_mat4(name, mat4_uniform);

								/* Standard uniforms */
								shader_program->set_mat4("mvp", mvp);
								shader_program->set_mat4("model_to_world_matrix", model_to_world_matrix);
								shader_program->set_mat4("world_to_model_matrix", glm::inverse(model_to_world_matrix));
								shader_program->set_int("albedo_texture_sampler", 0);
								shader_program->set_int("metallicity_texture_sampler", 1);
								shader_program->set_int("roughness_texture_sampler", 2);
								shader_program->set_int("emission_texture_sampler", 3);
								shader_program->set_int("normal_texture_sampler", 4);
								shader_program->set_int("occlusion_texture_sampler", 5);
								shader_program->set_float("alpha", alpha);
								shader_program->set_vec4("highlight_colour", renderable->highlight_colour->get_rgba_in_vec4());
								shader_program->set_int("light_count", lights_count);
								shader_program->set_int_array("light_modes", light_modes);
								shader_program->set_vec3_array("light_colours", light_colours);
								shader_program->set_vec3_array("shadow_colours", shadow_colours);
								shader_program->set_float_array("light_intensities", light_intensities);
								shader_program->set_float_array("light_ranges", light_ranges);
								shader_program->set_bool_array("are_shadows_enabled", are_shadows_enabled);
								shader_program->set_vec3_array("light_translations", light_translations);
								shader_program->set_vec3_array("light_rotations", light_rotations);
								shader_program->set_float_array("light_inner_cutoff_angles", light_inner_cutoff_angles);
								shader_program->set_float_array("light_outer_cutoff_angles", light_outer_cutoff_angles);
								shader_program->set_vec2("camera_viewport", camera->get_viewport());
								shader_program->set_vec3("camera_translation", camera_transform->translation);
								shader_program->set_vec3("camera_rotation", glm::vec3(
									glm::radians(camera_transform->rotation.x),
									glm::radians(camera_transform->rotation.y),
									glm::radians(camera_transform->rotation.z)));
								shader_program->set_vec3("entity_translation", renderable_transform->translation);
								shader_program->set_vec3("entity_rotation", glm::vec3(
									glm::radians(renderable_transform->rotation.x),
									glm::radians(renderable_transform->rotation.y),
									glm::radians(renderable_transform->rotation.z)));
								shader_program->set_vec3("entity_scale", renderable_transform->scale);

								if (vertex_array->get_index_count() > 0)
								{
									this->opengl_backend->draw_triangles_from_elements(vertex_array->get_index_count());
								}
								else
								{
									Mesh::PrimitiveMode primitive_mode = renderable->mesh->get_primitive_mode();
									if (primitive_mode == Mesh::PrimitiveMode::TRIANGLES)
									{
										this->opengl_backend->draw_triangles_from_arrays(vertex_array->get_vertex_count());
									}
									else if (primitive_mode == Mesh::PrimitiveMode::POINTS)
									{
										this->opengl_backend->draw_points_from_arrays(vertex_array->get_vertex_count());
									}
									else if (primitive_mode == Mesh::PrimitiveMode::LINE_STRIP)
									{
										this->opengl_backend->draw_line_strip_from_arrays(vertex_array->get_vertex_count());
									}
								}

								vertex_array->unbind();
								this->opengl_backend->disable_blending();
							}
						}
					}
				}			
			}
		}
	}

	this->opengl_backend->disable_depth_test();
	this->opengl_backend->disable_wireframe_mode();
	this->opengl_backend->disable_face_culling();

	/* Render all GUIs */
	for (auto& gui : scene->get_components_by_type<GUI>())
	{
		std::shared_ptr<Entity> gui_entity = scene->get_entity(gui->get_entity_id());

		if (gui->mesh != nullptr && gui_entity->is_2d)
		{
			this->opengl_backend->enable_blending();
			std::shared_ptr<OpenGLVertexArray> vertex_array = this->opengl_backend->get_vertex_array(gui->mesh);
			vertex_array->bind();
			this->opengl_backend->get_texture(gui->material->albedo_map)->bind(OpenGLTexture::Unit::_0);

			std::shared_ptr<OpenGLShaderProgram> shader_program;
			std::shared_ptr<Shader> shader = gui->get_shader();
			AssetID shader_id = shader->get_id();

			if (!this->opengl_backend->shader_programs.count(shader_id))
			{
				this->opengl_backend->shader_programs.emplace(
					shader_id,
					std::shared_ptr<OpenGLShaderProgram>(new OpenGLShaderProgram(shader)));
			}

			shader_program = this->opengl_backend->shader_programs.at(shader_id);
			shader_program->use();

			std::shared_ptr<GUIElement> root_element = gui->get_root_element();

			/* Standard GUI uniforms */
			shader_program->set_vec2("gui_position", root_element->get_position() - root_element->get_pivot_offset());
			shader_program->set_vec2("screen_viewport", Platform::get_window().get_window_size());
			shader_program->set_int("albedo_texture_sampler", 0);
			shader_program->set_float("alpha", gui->get_alpha_in_percentage());
			shader_program->set_vec4("highlight_colour", gui->highlight_colour->get_rgba_in_vec4());

			this->opengl_backend->draw_triangles_from_elements(vertex_array->get_index_count());
			this->opengl_backend->disable_blending();
		}
	}

	this->opengl_backend->collect_garbage();
	this->opengl_backend->swap_buffers();
	frame_time_clock->set_end();
}

void Omnific::RenderingSystem::finalize()
{
	if (this->is_initialized)
		SDL_QuitSubSystem(SDL_INIT_VIDEO);

	this->is_initialized = false;
}

void Omnific::RenderingSystem::on_window_resize()
{
	glm::vec2 window_size = Platform::get_window().get_window_size();

	if (this->last_detected_window_size.x != window_size.x ||
		this->last_detected_window_size.y != window_size.y)
	{
		this->opengl_backend->set_viewport(window_size.x, window_size.y);
		this->last_detected_window_size = window_size;
	}
}

std::string Omnific::RenderingSystem::get_rendering_backend_name()
{
	return this->opengl_backend->get_rendering_backend_name();
}