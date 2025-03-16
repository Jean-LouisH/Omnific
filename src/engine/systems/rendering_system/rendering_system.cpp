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
#include <foundations/singletons/configuration.hpp>

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
	this->dummy_light_transform->rotate_x(45.0);
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
}

void Omnific::RenderingSystem::on_output(std::shared_ptr<Scene> scene)
{
	this->on_window_resize();

	if (this->has_scene_changed(scene))
		this->build_renderables(scene);

	this->opengl_backend->clear_colour_buffer(0, 0, 0, 255);

	for (int i = 0; i < this->renderable_layer_lists.size(); i++)
	{
		std::vector<RenderableLayer> renderable_layer_list = this->renderable_layer_lists[i];
		RenderableLayer* renderable_layer_list_data = renderable_layer_list.data();
		size_t renderable_layer_list_count = renderable_layer_list.size();

		for (size_t j = 0; j < renderable_layer_list_count; j++)
		{
			RenderableLayer& renderable_layer = renderable_layer_list_data[j];

			if (renderable_layer.camera->get_is_streaming())
			{
				Renderable* renderables_data = renderable_layer.renderables.data();
				size_t renderable_count = renderable_layer.renderables.size();
				glm::mat4 world_to_view_matrix = glm::inverse(renderable_layer.camera_transform->get_transform_matrix());
				glm::mat4 view_to_projection_matrix = renderable_layer.camera->get_view_to_projection_matrix();

				if (renderable_layer.is_2d)
				{
					this->opengl_backend->disable_depth_test();
				}
				else
				{
					this->opengl_backend->clear_depth_buffer();
					this->opengl_backend->enable_depth_test();
				}

				if (renderable_layer.camera->get_is_wireframe_mode())
					this->opengl_backend->enable_wireframe_mode();
				else
					this->opengl_backend->disable_wireframe_mode();

				/* Memory allocated out of the tight loop. */
				std::shared_ptr<Transform> global_transform(new Transform());
				std::vector<std::shared_ptr<Light>> active_lights;
				std::vector<std::shared_ptr<Transform>> active_light_transforms;

				if (renderable_layer.lights.size() > 0)
				{
					active_lights = renderable_layer.lights;
					active_light_transforms = renderable_layer.light_transforms;
				}
				else
				{
					active_lights.push_back(this->dummy_light);
					active_light_transforms.push_back(this->dummy_light_transform);
				}

				std::vector<int> light_modes;
				std::vector<glm::vec3> light_colours;
				std::vector<glm::vec3> shadow_colours;
				std::vector<float> light_intensities;
				std::vector<float> light_ranges;
				std::vector<bool> are_shadows_enabled;
				std::vector<glm::vec3> light_translations;
				std::vector<glm::vec3> light_rotations;

				size_t lights_count = active_lights.size();
				for (size_t k = 0; k < lights_count; k++)
				{
					std::shared_ptr<Light> light = active_lights[k];
					std::shared_ptr<Transform> light_transform = active_light_transforms[k];

					light_modes.push_back((int)light->mode);
					light_colours.push_back(light->colour->get_rgb_in_vec3());
					shadow_colours.push_back(light->shadow_colour->get_rgb_in_vec3());
					light_intensities.push_back(light->intensity);
					light_ranges.push_back(light->range);
					are_shadows_enabled.push_back(light->is_shadow_enabled);
					light_translations.push_back(light_transform->translation);
					light_rotations.push_back(light_transform->rotation);
				}


				/* Forward Rendering */
				for (size_t k = 0; k < renderable_count; k++)
				{
					Renderable& renderable = renderables_data[k];
					std::shared_ptr<Shader> shader = renderable.overriding_shader;
					std::shared_ptr<ShaderParameters> shader_parameters = renderable.overriding_shader_parameters;

					if (shader == nullptr && shader_parameters == nullptr)
					{
						shader = renderable.model->get_shader();
						shader_parameters = renderable.model->shader_parameters;
					}

					global_transform = scene->get_scene_layer(renderable.scene_layer_id)->compute_global_transform(renderable.entity_id);
					glm::mat4 model_to_world_matrix = global_transform->get_transform_matrix();
					glm::mat4 mvp = view_to_projection_matrix * world_to_view_matrix * model_to_world_matrix;
					float alpha = renderable.model->get_alpha_in_percentage();
					const float cull_alpha_threshold = 1.0 - 0.001;
					Model::FaceCullMode face_cull_mode = renderable.model->get_face_cull_mode();

					switch (face_cull_mode)
					{
						case Model::FaceCullMode::NONE:
						case Model::FaceCullMode::BACK: this->opengl_backend->set_face_culling_to_back(); break;
						case Model::FaceCullMode::FRONT: this->opengl_backend->set_face_culling_to_front(); break;
						case Model::FaceCullMode::FRONT_AND_BACK: this->opengl_backend->set_face_culling_to_front_and_back(); break;
					}

					if (face_cull_mode == Model::FaceCullMode::NONE)
						this->opengl_backend->disable_face_culling();
					else
						this->opengl_backend->enable_face_culling();

					this->opengl_backend->enable_blending();

					std::shared_ptr<Mesh> mesh = renderable.model->mesh;
					std::shared_ptr<OpenGLVertexArray> vertex_array = this->opengl_backend->get_vertex_array(mesh);
					vertex_array->bind();

					std::shared_ptr<Material> material = renderable.model->material;
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

						if (renderable_layer.is_2d)
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

							if (renderable_layer.is_2d || (!renderable_layer.is_2d && preset == "Shader::CUSTOM"))
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
							else if (preset == "Shader::PHONG")
							{
								complete_shader = std::shared_ptr<Shader>(new Shader(
									default_vertex_input,
									this->opengl_backend->get_phong_fragment_input(),
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
					else if (!renderable_layer.is_2d)
					{
						shader_program = this->opengl_backend->built_in_shader_program_3d;
					}
					else
					{
						shader_program = this->opengl_backend->built_in_shader_program_2d;
					}

					shader_program->use();

					/* Custom uniforms. */
					for (auto const& int_uniform_pair : shader_parameters->int_uniforms)
						shader_program->set_int(int_uniform_pair.first, int_uniform_pair.second);

					for (auto const& bool_uniform_pair : shader_parameters->bool_uniforms)
						shader_program->set_bool(bool_uniform_pair.first, bool_uniform_pair.second);

					for (auto const& float_uniform_pair : shader_parameters->float_uniforms)
						shader_program->set_float(float_uniform_pair.first, float_uniform_pair.second);

					for (auto const& vec2_uniform_pair : shader_parameters->vec2_uniforms)
						shader_program->set_vec2(vec2_uniform_pair.first, vec2_uniform_pair.second);

					for (auto const& vec3_uniform_pair : shader_parameters->vec3_uniforms)
						shader_program->set_vec3(vec3_uniform_pair.first, vec3_uniform_pair.second);

					for (auto const& vec4_uniform_pair : shader_parameters->vec4_uniforms)
						shader_program->set_vec4(vec4_uniform_pair.first, vec4_uniform_pair.second);

					for (auto const& mat4_uniform_pair : shader_parameters->mat4_uniforms)
						shader_program->set_mat4(mat4_uniform_pair.first, mat4_uniform_pair.second);

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
					shader_program->set_int("light_count", lights_count);
					shader_program->set_int_array("light_modes", light_modes);
					shader_program->set_vec3_array("light_colours", light_colours);
					shader_program->set_vec3_array("shadow_colours", shadow_colours);
					shader_program->set_float_array("light_intensities", light_intensities);
					shader_program->set_float_array("light_ranges", light_ranges);
					shader_program->set_bool_array("are_shadows_enabled", are_shadows_enabled);
					shader_program->set_vec3_array("light_translations", light_translations);
					shader_program->set_vec3_array("light_rotations", light_rotations);
					shader_program->set_vec2("camera_viewport", renderable_layer.camera->get_viewport());
					shader_program->set_vec3("camera_translation", renderable_layer.camera_transform->translation);
					shader_program->set_vec3("camera_rotation", renderable_layer.camera_transform->rotation);
					shader_program->set_vec3("entity_translation", renderable.transform->translation);
					shader_program->set_vec3("entity_rotation", renderable.transform->rotation);
					shader_program->set_vec3("entity_scale", renderable.transform->scale);

					if (vertex_array->get_index_count() > 0)
						this->opengl_backend->draw_triangles_from_elements(vertex_array->get_index_count());
					else
						this->opengl_backend->draw_triangles_from_arrays(vertex_array->get_vertex_count());

					vertex_array->unbind();
					this->opengl_backend->disable_blending();
				}
			}
		}
	}

	this->opengl_backend->collect_garbage();
	this->opengl_backend->swap_buffers();
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

	if (last_detected_window_size.x != window_size.x ||
		last_detected_window_size.y != window_size.y)
	{
		int renderable_layer_list_size = this->renderable_layer_lists.size();

		for (int i = 0; renderable_layer_list_size; i++)
		{
			std::vector<RenderableLayer> renderable_layers = this->renderable_layer_lists.at(renderable_layer_list_size - 1);
			int renderable_layer_size = renderable_layers.size();
			if (renderable_layer_size > 0)
			{
				RenderableLayer& renderable_layer = renderable_layers.at(renderable_layer_size - 1);

				if (renderable_layer.renderables.size() > 0)
				{
					Renderable& entity_renderable = renderable_layer.renderables.at(0);
					if (entity_renderable.model->is_type(GUI::TYPE_STRING))
					{
						renderable_layer.camera->set_viewport_width(window_size.x);
						renderable_layer.camera->set_viewport_height(window_size.y);
					}
				}
			}
		}

		this->opengl_backend->set_viewport(window_size.x, window_size.y);
		last_detected_window_size = window_size;
	}
}

void Omnific::RenderingSystem::build_renderables(std::shared_ptr<Scene> scene)
{
	this->renderable_layer_lists.clear();

	std::vector<std::shared_ptr<Model>> gui_renderable_components;
	std::vector<std::shared_ptr<SceneLayer>> gui_scene_layers;

	for (const auto scene_layer_it : scene->get_scene_layers())
	{
		std::shared_ptr<SceneLayer> scene_layer = scene_layer_it.second;
		std::vector<RenderableLayer> renderable_layer_list;

		std::vector<std::shared_ptr<Viewport>> ui_viewports = scene_layer->get_components_by_type<Viewport>();
		std::vector<size_t> render_order_index_cache = scene_layer->get_render_order_index_cache();

		/* One or more Viewport RenderableLayers for each SceneLayer. */
		for (int i = 0; i < ui_viewports.size(); i++)
		{
			std::shared_ptr<Viewport> ui_viewport = ui_viewports.at(i);
			std::shared_ptr<Entity> camera_entity = scene_layer->get_entity_by_name(ui_viewport->get_camera_entity_name());
			std::shared_ptr<Camera> camera = scene_layer->get_component_by_type<Camera>(camera_entity->get_id());
			RenderableLayer renderable_layer;
			std::shared_ptr<Transform> camera_transform = camera_entity->get_transform();

			renderable_layer.is_2d = scene_layer->is_2d;
			renderable_layer.camera = camera;
			renderable_layer.camera_transform = camera_transform;

			for (std::shared_ptr<Light> light : scene_layer->get_components_by_type<Light>())
			{
				renderable_layer.lights.push_back(light);
				renderable_layer.light_transforms.push_back(scene_layer->get_entity(light->get_entity_id())->get_transform());
			}

			/* Entity RenderableLayer for each Viewport*/
			for (int i = 0; i < render_order_index_cache.size(); i++)
			{
				Renderable entity_renderable;
				std::shared_ptr<Model> renderable_component =
					std::dynamic_pointer_cast<Model>(scene_layer->get_components().at(render_order_index_cache.at(i)));

				/* GUI Components are deferred to a final RenderableLayer, 
				   while other Renderables are in the usual order. */
				if (renderable_component->is_type(GUI::TYPE_STRING))
				{
					gui_renderable_components.push_back(renderable_component);
					gui_scene_layers.push_back(scene_layer);
				}
				else
				{
					std::shared_ptr<Entity> entity = scene_layer->get_entity(renderable_component->get_entity_id());
					entity_renderable.entity_name = entity->get_name();
					entity_renderable.transform = entity->get_transform();
					entity_renderable.model = renderable_component;
					entity_renderable.scene_layer_id = scene_layer->get_id();
					entity_renderable.entity_id = entity->get_id();

					std::shared_ptr<Entity> top_entity = entity;
					EntityID parent_entity_id = entity->parent_id;

					/*Find the top entity of the hierarchy for the
					  overriding shader. */
					while (parent_entity_id != 0)
					{
						top_entity = scene_layer->get_entity(parent_entity_id);
						parent_entity_id = top_entity->parent_id;
					}

					if (top_entity->renderable_component_id != 0)
					{
						std::shared_ptr<Model> renderable_component =
							std::dynamic_pointer_cast<Model>(scene_layer->get_component_by_id(top_entity->renderable_component_id));

						std::shared_ptr<Shader> overriding_shader = renderable_component->get_overriding_shader();

						if (overriding_shader != nullptr)
						{
							entity_renderable.overriding_shader = overriding_shader;
							entity_renderable.overriding_shader_parameters = renderable_component->shader_parameters;
						}
					}

					renderable_layer.renderables.push_back(entity_renderable);
				}
			}
			renderable_layer_list.push_back(renderable_layer);
		}

		this->renderable_layer_lists.push_back(renderable_layer_list);
	}

	/* Put the deferred GUIs on a final RenderableLayer to be rendered last. */
	RenderableLayer renderable_layer;
	std::vector<RenderableLayer> renderable_layer_list;
	std::shared_ptr<Camera> camera(new Camera());
	std::shared_ptr<Transform> camera_transform(new Transform());
	glm::vec2 window_size = Platform::get_window().get_window_size();

	/* a virtual Camera for the GUI. */
	camera->set_viewport_width(window_size.x);
	camera->set_viewport_height(window_size.y);
	camera->set_is_streaming(true);
	camera->set_wireframe_mode(false);
	camera_transform->translation.x = window_size.x / 2.0;
	camera_transform->translation.y = window_size.y / 2.0;

	renderable_layer.is_2d = true;
	renderable_layer.camera = camera;
	renderable_layer.camera_transform = camera_transform;

	for (int i = 0; i < gui_renderable_components.size(); i++)
	{
		Renderable entity_renderable;
		std::shared_ptr<Model> gui_renderable_component = gui_renderable_components[i];
		std::shared_ptr<SceneLayer> gui_scene_layer = gui_scene_layers[i];
		std::shared_ptr<Entity> entity = gui_scene_layer->get_entity(gui_renderable_component->get_entity_id());

		entity_renderable.transform = entity->get_transform();
		entity_renderable.model = gui_renderable_component;
		entity_renderable.entity_id = entity->get_id();
		entity_renderable.entity_name = entity->get_name();
		entity_renderable.scene_layer_id = gui_scene_layer->get_id();
		renderable_layer.renderables.push_back(entity_renderable);
	}

	/* There is only a single virtual Viewport, so one RenderableLayer. */
	renderable_layer_list.push_back(renderable_layer);
	this->renderable_layer_lists.push_back(renderable_layer_list);
}

std::string Omnific::RenderingSystem::get_rendering_backend_name()
{
	return this->opengl_backend->get_rendering_backend_name();
}