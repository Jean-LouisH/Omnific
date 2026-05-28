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
#include <memory>
#include <scene/components/camera.hpp>
#include <scene/components/viewport.hpp>
#include <foundations/transform.hpp>
#include <scene/components/light.hpp>
#include <scene/components/gui_element.hpp>
#include <scene/components/world_environment.hpp>
#include <foundations/singletons/configuration.hpp>
#include <foundations/singletons/profiler.hpp>
#include <foundations/singletons/scene_storage.hpp>
#include "render_device/opengl_render_device.hpp"
#include "render_device/sdl_gpu_render_device.hpp"
#include "render_device/vulkan_render_device.hpp"

#define RENDERING_SYSTEM_ON_OUTPUT_FRAME_TIME_CLOCK_NAME "rendering_system_on_output_frame_time"

Omnific::RenderingSystem::RenderingSystem()
{
	this->dummy_light = std::shared_ptr<Light>(new Light());
	this->dummy_light_transform = std::shared_ptr<Transform>(new Transform());
	this->dummy_light_transform->translate_y(5.0);
	this->dummy_light_transform->translate_z(5.0);
	this->dummy_light_transform->rotate_x(-45.0);
	this->type = TYPE_STRING;
}

Omnific::RenderingSystem::~RenderingSystem()
{
	this->finalize();
}

void Omnific::RenderingSystem::initialize()
{
	SDL_InitSubSystem(SDL_INIT_VIDEO);

	Configuration* configuration = Configuration::get_instance();
	std::string render_device_name = configuration->window_settings.render_device;

	if (render_device_name == "opengl")
	{
		this->render_device = std::shared_ptr<OpenGLRenderDevice>(new OpenGLRenderDevice());
	}
	else if (render_device_name == "sdl_gpu")
	{
		this->render_device = std::shared_ptr<SDLGPURenderDevice>(new SDLGPURenderDevice());
	}
	else if (render_device_name == "vulkan")
	{
		this->render_device = std::shared_ptr<VulkanRenderDevice>(new VulkanRenderDevice());
	}
	else
	{
		Platform::get_logger().write("Invalid render device specified in configuration file. Defaulting to OpenGL.");
		this->render_device = std::shared_ptr<OpenGLRenderDevice>(new OpenGLRenderDevice());
	}

	Platform::create_window(configuration->metadata.title.c_str(),
		configuration->window_settings.width,
		configuration->window_settings.height,
		configuration->window_settings.is_starting_fullscreen,
		render_device_name);

	Image image = Image(
		Platform::get_file_access().find_path(configuration->metadata.icon_filepath));
		
	Platform::get_window().change_icon(
		image.get_data(),
		image.get_width(),
		image.get_height(),
		image.get_depth(),
		image.get_pitch());

	this->render_device->initialize();
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

	std::shared_ptr<WorldEnvironment> world_environment;
	
	for (auto current_world_environment : scene->get_components_by_type<WorldEnvironment>())
		world_environment = current_world_environment;

	if (world_environment != nullptr)
	{
		switch(world_environment->background_mode)
		{
			case WorldEnvironment::BackgroundMode::SKY:; break;
			case WorldEnvironment::BackgroundMode::CLEAR_COLOUR: this->render_device->clear_colour_buffer(world_environment->clear_colour->get_rgba_in_vec4()); break;
		}
	}
	else
	{
		this->render_device->clear_colour_buffer(0, 0, 0, 255);
	}

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
						this->render_device->disable_depth_test();
					}
					else
					{
						this->render_device->clear_depth_buffer();
						this->render_device->enable_depth_test();
					}

					if (camera->get_is_wireframe_mode())
						this->render_device->enable_wireframe_mode();
					else
						this->render_device->disable_wireframe_mode();


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
						light_rotations.push_back(light_transform->get_rotation_in_euler_angles());					
					}

					if (this->rendering_path == RenderingPath::FORWARD)
					{
						for (auto& renderable : scene->get_renderables_in_order())
						{
							std::shared_ptr<Entity> renderable_entity = scene->get_entity(renderable->get_entity_id());
							std::shared_ptr<Transform> renderable_transform = renderable_entity->get_transform();

							if (renderable_entity->is_2d == camera_entity->is_2d &&
								!renderable->is_gui_element() &&
								!renderable->is_hidden() &&
								renderable->mesh != nullptr)
							{
								this->render_device->enable_blending();
								std::shared_ptr<Renderable::Material> material = renderable->material;
								std::shared_ptr<Shader> shader = renderable->get_shader();
								std::shared_ptr<ShaderParameters> shader_parameters = renderable->shader_parameters;

								if (shader == nullptr)
								{
									if (camera->shader != nullptr)
									{
										shader = camera->shader;
									}
									else if (world_environment != nullptr)
									{
										if (world_environment->shader != nullptr)
										{
											shader = world_environment->shader;
										}
									}
								}

								std::shared_ptr<Entity> top_entity = renderable_entity;
								EntityID parent_entity_id = renderable_entity->parent_id;

								/*Find the top entity of the hierarchy for the
									overriding shader and material. */
								while (parent_entity_id != 0)
								{
									top_entity = scene->get_entity(parent_entity_id);
									parent_entity_id = top_entity->parent_id;
								}

								if (top_entity->get_renderable_id() != 0)
								{
									std::shared_ptr<Renderable> overriding_renderable =
										std::dynamic_pointer_cast<Renderable>(scene->get_component_by_id(top_entity->get_renderable_id()));

									std::shared_ptr<Renderable::Material> overriding_material = overriding_renderable->overriding_material;
									std::shared_ptr<Shader> overriding_shader = overriding_renderable->get_overriding_shader();
								
									if (overriding_material != nullptr)
									{
										material = overriding_material;
									}

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
									case Renderable::FaceCullMode::BACK: this->render_device->set_face_culling_to_back(); break;
									case Renderable::FaceCullMode::FRONT: this->render_device->set_face_culling_to_front(); break;
									case Renderable::FaceCullMode::FRONT_AND_BACK: this->render_device->set_face_culling_to_front_and_back(); break;
								}

								if (face_cull_mode == Renderable::FaceCullMode::NONE)
									this->render_device->disable_face_culling();
								else
									this->render_device->enable_face_culling();

								
								this->render_device->bind_mesh(renderable->mesh);
								this->render_device->bind_material(material);

								if (shader != nullptr)
								{
									AssetID shader_id = shader->get_id();
									std::string default_vertex_input;
									std::string default_fragment_input;

									if (renderable_entity->is_2d)
									{
										default_vertex_input = this->render_device->get_default_2d_vertex_input();
										default_fragment_input = this->render_device->get_default_2d_fragment_input();
									}
									else
									{
										default_vertex_input = this->render_device->get_default_3d_vertex_input();
										default_fragment_input = this->render_device->get_default_3d_fragment_input();
									}

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
											this->render_device->get_light_source_fragment_input(),
											false,
											false));
									}
									else if (preset == "Shader::UNLIT")
									{
										complete_shader = std::shared_ptr<Shader>(new Shader(
											default_vertex_input,
											this->render_device->get_unlit_fragment_input(),
											false,
											false));
									}
									else if (preset == "Shader::SIMPLE")
									{
										complete_shader = std::shared_ptr<Shader>(new Shader(
											default_vertex_input,
											this->render_device->get_simple_fragment_input(),
											false,
											false));
									}
									else if (preset == "Shader::PBR")
									{
										complete_shader = std::shared_ptr<Shader>(new Shader(
											default_vertex_input,
											this->render_device->get_pbr_fragment_input(),
											false,
											false));
									}

									this->render_device->use_shader(complete_shader);
								}
								else if (renderable_entity->is_2d)
								{
									this->render_device->use_default_2d_shader();
								}
								else
								{
									this->render_device->use_default_3d_shader();
								}

								/* Custom uniforms. */
								for (auto const& [name, int_uniform] : shader_parameters->int_uniforms)
									this->render_device->set_int_uniform(name, int_uniform);

								for (auto const& [name, bool_uniform] : shader_parameters->bool_uniforms)
									this->render_device->set_bool_uniform(name, bool_uniform);

								for (auto const& [name, float_uniform] : shader_parameters->float_uniforms)
									this->render_device->set_float_uniform(name, float_uniform);

								for (auto const& [name, vec2_uniform] : shader_parameters->vec2_uniforms)
									this->render_device->set_vec2_uniform(name, vec2_uniform);

								for (auto const& [name, vec3_uniform] : shader_parameters->vec3_uniforms)
									this->render_device->set_vec3_uniform(name, vec3_uniform);

								for (auto const& [name, vec4_uniform] : shader_parameters->vec4_uniforms)
									this->render_device->set_vec4_uniform(name, vec4_uniform);

								for (auto const& [name, mat4_uniform] : shader_parameters->mat4_uniforms)
									this->render_device->set_mat4_uniform(name, mat4_uniform);

								/* Standard uniforms */
								this->render_device->set_mat4_uniform("mvp", mvp);
								this->render_device->set_mat4_uniform("model_to_world_matrix", model_to_world_matrix);
								this->render_device->set_mat4_uniform("world_to_model_matrix", glm::inverse(model_to_world_matrix));
								this->render_device->set_int_uniform("albedo_texture_sampler", 0);
								this->render_device->set_int_uniform("metallicity_texture_sampler", 1);
								this->render_device->set_int_uniform("roughness_texture_sampler", 2);
								this->render_device->set_int_uniform("emission_texture_sampler", 3);
								this->render_device->set_int_uniform("normal_texture_sampler", 4);
								this->render_device->set_int_uniform("occlusion_texture_sampler", 5);
								this->render_device->set_float_uniform("alpha", alpha);
								this->render_device->set_int_uniform("diffuse_reflection_model", (int)renderable->material->diffuse_reflection_model);
								this->render_device->set_int_uniform("specular_reflection_model", (int)renderable->material->specular_reflection_model);
								this->render_device->set_vec4_uniform("highlight_colour", renderable->highlight_colour->get_rgba_in_vec4());
								this->render_device->set_int_uniform("light_count", lights_count);
								this->render_device->set_int_array_uniform("light_modes", light_modes);
								this->render_device->set_vec3_array_uniform("light_colours", light_colours);
								this->render_device->set_vec3_array_uniform("shadow_colours", shadow_colours);
								this->render_device->set_float_array_uniform("light_intensities", light_intensities);
								this->render_device->set_float_array_uniform("light_ranges", light_ranges);
								this->render_device->set_bool_array_uniform("are_shadows_enabled", are_shadows_enabled);
								this->render_device->set_vec3_array_uniform("light_translations", light_translations);
								this->render_device->set_vec3_array_uniform("light_rotations", light_rotations);
								this->render_device->set_float_array_uniform("light_inner_cutoff_angles", light_inner_cutoff_angles);
								this->render_device->set_float_array_uniform("light_outer_cutoff_angles", light_outer_cutoff_angles);
								this->render_device->set_vec2_uniform("camera_viewport", camera->get_viewport());
								this->render_device->set_vec3_uniform("camera_translation", camera_transform->translation);
								this->render_device->set_vec3_uniform("camera_rotation", camera_transform->get_rotation_in_radians_euler_angles());
								this->render_device->set_vec3_uniform("entity_translation", renderable_transform->translation);
								this->render_device->set_vec3_uniform("entity_rotation", renderable_transform->get_rotation_in_radians_euler_angles());
								this->render_device->set_vec3_uniform("entity_scale", renderable_transform->scale);

								this->draw(renderable);

								this->render_device->unbind_mesh();
								this->render_device->unbind_material();
								this->render_device->disable_blending();
							}
						}
					}
				}			
			}
		}
	}

	this->render_device->disable_depth_test();
	this->render_device->disable_wireframe_mode();
	this->render_device->disable_face_culling();

	/* Render all GUIs */
	for (auto& renderable : scene->get_renderables_in_order())
	{
		if (renderable->is_gui_element())
		{
			std::shared_ptr<GUIElement> gui_element = std::dynamic_pointer_cast<GUIElement>(renderable);
			std::shared_ptr<Entity> gui_element_entity = scene->get_entity(gui_element->get_entity_id());

			if (renderable->mesh != nullptr)
			{
				this->render_device->enable_blending();
				this->render_device->bind_mesh(gui_element->mesh);
				this->render_device->bind_texture(gui_element->material->albedo_map, RenderDevice::TextureSemantic::ALBEDO, gui_element->material->albedo_texture_properties);
				this->render_device->use_shader(gui_element->get_shader());

				/* Standard GUI uniforms */
				this->render_device->set_vec2_uniform("gui_position", glm::vec2(gui_element_entity->get_transform()->translation) - gui_element->get_position_pivot_offset());
				this->render_device->set_vec2_uniform("gui_scale", glm::vec2(gui_element_entity->get_transform()->scale));
				this->render_device->set_vec2_uniform("screen_viewport", Platform::get_window().get_window_size());
				this->render_device->set_int_uniform("albedo_texture_sampler", 0);
				this->render_device->set_float_uniform("alpha", gui_element->get_alpha_in_percentage());
				this->render_device->set_float_uniform("highlight_opacity", gui_element->current_highlight_opacity);

				this->draw(gui_element);

				this->render_device->unbind_mesh();
				this->render_device->unbind_texture(RenderDevice::TextureSemantic::ALBEDO);
				this->render_device->disable_blending();
			}
		}
	}

	this->render_device->collect_garbage();
	Platform::get_window().swap_buffers();
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
		this->render_device->set_viewport(window_size.x, window_size.y);
		this->last_detected_window_size = window_size;
	}
}

void Omnific::RenderingSystem::draw(std::shared_ptr<Renderable> renderable)
{
	if (renderable->mesh->get_is_indexed())
	{
		this->render_device->draw_indexed_triangles(renderable->mesh);
	}
	else
	{
		switch (renderable->mesh->get_primitive_mode())
		{
			case Mesh::PrimitiveMode::TRIANGLES: this->render_device->draw_triangles(renderable->mesh); break;
			case Mesh::PrimitiveMode::POINTS: this->render_device->draw_points(renderable->mesh); break;
			case Mesh::PrimitiveMode::LINE_STRIP: this->render_device->draw_line_strip(renderable->mesh); break;
		}
	}
}

std::string Omnific::RenderingSystem::get_render_device_name()
{
	return this->render_device->get_name();
}