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

#include "opengl_render_device.hpp"
#include <foundations/resources/default_assets/shaders/fragment_light_source_glsl.hpp>
#include <foundations/resources/default_assets/shaders/fragment_pbr_glsl.hpp>
#include <foundations/resources/default_assets/shaders/fragment_simple_glsl.hpp>
#include <foundations/resources/default_assets/shaders/vertex_2d_glsl.hpp>
#include <foundations/resources/default_assets/shaders/vertex_3d_glsl.hpp>
#include <foundations/resources/default_assets/shaders/fragment_unlit_glsl.hpp>
#include <foundations/singletons/platform/platform.hpp>
#include <foundations/singletons/profiler.hpp>
#include <unordered_map>
#include <string>
#include <SDL3/SDL_video.h>

#ifdef _WEB_PLATFORM
    #include <GLES3/gl3.h>
#else
    #include <glad/glad.h>
#endif

void Omnific::OpenGLRenderDevice::initialize()
{
	Window& window = Platform::get_window();
	window.initialize_window_context("opengl");
#ifndef _WEB_PLATFORM

	if ((!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)))
	{
		Platform::get_logger().write("GLAD failed to initialize.");
	}
	else
#endif
	{
		glm::vec2 window_size = window.get_window_size();
		this->set_viewport(window_size.x, window_size.y);
		std::string render_device_name = (std::string)("OpenGL ") + (char*)glGetString(GL_VERSION);
		Profiler::set_render_device_name(render_device_name);
		Platform::get_logger().write("Rendering Backend initialized with " + render_device_name);
	}
}

void Omnific::OpenGLRenderDevice::clear_colour_buffer(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
{
	glClearColor(
		(float)(red) / 255.0, 
		(float)(green) / 255.0, 
		(float)(blue) / 255.0, 
		(float)(alpha) / 255.0);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Omnific::OpenGLRenderDevice::clear_colour_buffer(glm::vec4 colour)
{
	glClearColor(colour.x, colour.y, colour.z, colour.w);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Omnific::OpenGLRenderDevice::clear_depth_buffer()
{
	glClear(GL_DEPTH_BUFFER_BIT);
}

void Omnific::OpenGLRenderDevice::enable_depth_test()
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
}

void Omnific::OpenGLRenderDevice::disable_depth_test()
{
	glDisable(GL_DEPTH_TEST);
}

void Omnific::OpenGLRenderDevice::enable_wireframe_mode()
{
#ifndef _WEB_PLATFORM
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif
}

void Omnific::OpenGLRenderDevice::disable_wireframe_mode()
{
#ifndef _WEB_PLATFORM
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
#endif
}

void Omnific::OpenGLRenderDevice::enable_blending()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Omnific::OpenGLRenderDevice::disable_blending()
{
	glDisable(GL_BLEND);
}

void Omnific::OpenGLRenderDevice::set_viewport(uint32_t width, uint32_t height)
{
	glViewport(0, 0, width, height);
}

void Omnific::OpenGLRenderDevice::enable_face_culling()
{
	glEnable(GL_CULL_FACE);
}

void Omnific::OpenGLRenderDevice::disable_face_culling()
{
	glDisable(GL_CULL_FACE);
}

void Omnific::OpenGLRenderDevice::set_face_culling_to_front()
{
	glCullFace(GL_FRONT);
}

void Omnific::OpenGLRenderDevice::set_face_culling_to_back()
{
	glCullFace(GL_BACK);
}

void Omnific::OpenGLRenderDevice::set_face_culling_to_front_and_back()
{
	glCullFace(GL_FRONT_AND_BACK);
}

void Omnific::OpenGLRenderDevice::draw_indexed_triangles(std::shared_ptr<Mesh> mesh)
{
	glDrawElements(GL_TRIANGLES, (GLsizei)mesh->indices.size(), GL_UNSIGNED_INT, 0);
}

void Omnific::OpenGLRenderDevice::draw_triangles(std::shared_ptr<Mesh> mesh)
{
	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)mesh->vertices.size());
}

void Omnific::OpenGLRenderDevice::draw_points(std::shared_ptr<Mesh> mesh)
{
	glDrawArrays(GL_POINTS, 0, (GLsizei)mesh->vertices.size());
}

void Omnific::OpenGLRenderDevice::draw_line_strip(std::shared_ptr<Mesh> mesh)
{
	glDrawArrays(GL_LINE_STRIP, 0, (GLsizei)mesh->vertices.size());
}

std::string Omnific::OpenGLRenderDevice::get_name()
{
	return "opengl";
}

std::string Omnific::OpenGLRenderDevice::get_default_2d_vertex_input()
{
	return DefaultAssets::vertex_2d_glsl;
}

std::string Omnific::OpenGLRenderDevice::get_default_2d_fragment_input()
{
	return this->get_unlit_fragment_input();
}

std::string Omnific::OpenGLRenderDevice::get_default_3d_vertex_input()
{
	return DefaultAssets::vertex_3d_glsl;
}

std::string Omnific::OpenGLRenderDevice::get_default_3d_fragment_input()
{
	return this->get_pbr_fragment_input();
}

std::string Omnific::OpenGLRenderDevice::get_light_source_fragment_input()
{
	return DefaultAssets::fragment_light_source_glsl;
}

std::string Omnific::OpenGLRenderDevice::get_unlit_fragment_input()
{
	return DefaultAssets::fragment_unlit_glsl;
}

std::string Omnific::OpenGLRenderDevice::get_simple_fragment_input()
{
	return DefaultAssets::fragment_simple_glsl;
}

std::string Omnific::OpenGLRenderDevice::get_pbr_fragment_input()
{
	return DefaultAssets::fragment_pbr_glsl;
}

void Omnific::OpenGLRenderDevice::collect_garbage()
{
	std::vector<AssetID> assets_to_delete;

	for (auto it = this->missed_frame_counts.begin(); it != this->missed_frame_counts.end(); it++)
	{
		if (this->missed_frame_counts.at(it->first) > this->allowable_missed_frames)
		{
			if (this->textures.count(it->first) > 0)
			{
				GLuint texture_id = this->textures.at(it->first);
				glDeleteTextures(1, &texture_id);
				this->textures.erase(it->first);
			}

			if (this->vertex_arrays.count(it->first) > 0)
			{
				GLuint vertex_buffer_id = this->vertex_buffers.at(it->first);
				glDeleteBuffers(1, &vertex_buffer_id);
				this->vertex_buffers.erase(it->first);

				GLuint index_buffer_id = this->index_buffers.at(it->first);
				glDeleteBuffers(1, &index_buffer_id);
				this->index_buffers.erase(it->first);

				GLuint vertex_array_id = this->vertex_arrays.at(it->first);
				glDeleteVertexArrays(1, &vertex_array_id);
				this->vertex_arrays.erase(it->first);
			}

			assets_to_delete.push_back(it->first);
		}
	}

	for (size_t i = 0; i < assets_to_delete.size(); ++i)
		this->missed_frame_counts.erase(assets_to_delete.at(i));

	for (auto it = this->missed_frame_counts.begin(); it != this->missed_frame_counts.end(); it++)
		this->missed_frame_counts.at(it->first)++;
}

void Omnific::OpenGLRenderDevice::clear_stencil_buffer() 
{

}

void Omnific::OpenGLRenderDevice::enable_depth_mask() 
{

}

void Omnific::OpenGLRenderDevice::disable_depth_mask() 
{

}

void Omnific::OpenGLRenderDevice::colour_mask(bool red, bool green, bool blue, bool alpha) 
{

}

void Omnific::OpenGLRenderDevice::enable_stencil_test() 
{

}

void Omnific::OpenGLRenderDevice::disable_stencil_test() 
{

}

void Omnific::OpenGLRenderDevice::enable_scissor_test() 
{

}

void Omnific::OpenGLRenderDevice::disable_scissor_test() 
{

}

void Omnific::OpenGLRenderDevice::set_scissor_rectangle(int x, int y, int width, int height) 
{

}

void Omnific::OpenGLRenderDevice::stencil_function(RenderDevice::StencilFunction func, int ref, uint32_t mask) 
{

}

void Omnific::OpenGLRenderDevice::stencil_operation(RenderDevice::StencilOperation sfail, RenderDevice::StencilOperation dpfail, RenderDevice::StencilOperation dppass) 
{

}

void Omnific::OpenGLRenderDevice::stencil_mask(uint32_t mask) 
{

}

void Omnific::OpenGLRenderDevice::bind_mesh(std::shared_ptr<Mesh> mesh) 
{
	if (mesh == nullptr)
		return;

	AssetID mesh_id = mesh->get_id();

	if (this->vertex_arrays.count(mesh_id) == 0)
	{
		GLuint vertex_array_id = 0;
		glGenVertexArrays(1, &vertex_array_id);
		glBindVertexArray(vertex_array_id);

		GLuint vertex_buffer_id = 0;
		glGenBuffers(1, &vertex_buffer_id);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
		glBufferData(GL_ARRAY_BUFFER, mesh->vertices.size() * sizeof(Mesh::Vertex), mesh->vertices.data(), GL_STATIC_DRAW);

		GLuint index_buffer_id = 0;
		if (mesh->get_is_indexed())
		{

			glGenBuffers(1, &index_buffer_id);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_id);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indices.size() * sizeof(uint32_t),
				&mesh->indices[0], GL_STATIC_DRAW);
		}

		typedef Mesh::Vertex Vertex;

		// vertex positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		// vertex normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
		// vertex uvs
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
		// vertex tangent
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));

		glEnableVertexAttribArray(4);
        glVertexAttribIPointer(4, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, joint_indices));

		glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, joint_weights));

		glBindVertexArray(0);

		this->vertex_arrays.emplace(mesh_id, vertex_array_id);
		this->vertex_buffers.emplace(mesh_id, vertex_buffer_id);
		this->index_buffers.emplace(mesh_id, index_buffer_id);
		this->missed_frame_counts.emplace(mesh_id, 0);
	}
	else
	{
		this->missed_frame_counts.at(mesh_id) = 0;
	}

	glBindVertexArray(this->vertex_arrays.at(mesh_id));
}

void Omnific::OpenGLRenderDevice::unbind_mesh() 
{
	glBindVertexArray(0);
}


void Omnific::OpenGLRenderDevice::bind_texture(std::shared_ptr<Image> image, TextureSemantic semantic, const Renderable::Material::TextureProperties& texture_properties) 
{
	if (image == nullptr)
		return;

	AssetID image_id = image->get_id();

	if (this->textures.count(image_id) == 0)
	{
		GLuint texture_id = 0;

		glGenTextures(1, &texture_id);

		GLint last_active_texture;
		glGetIntegerv(GL_ACTIVE_TEXTURE, &last_active_texture);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_id);

		GLint texture_wrap_s = GL_REPEAT;
		GLint texture_wrap_t = GL_REPEAT;

#ifdef _WEB_PLATFORM
   		std::unordered_map<Renderable::Material::TextureWrap, GLint> wrap_mode_map = {
			{ Renderable::Material::TextureWrap::REPEAT, GL_REPEAT },
			{ Renderable::Material::TextureWrap::MIRRORED_REPEAT, GL_MIRRORED_REPEAT },
			{ Renderable::Material::TextureWrap::CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE }
		};
#else
		std::unordered_map<Renderable::Material::TextureWrap, GLint> wrap_mode_map = {
			{ Renderable::Material::TextureWrap::REPEAT, GL_REPEAT },
			{ Renderable::Material::TextureWrap::MIRRORED_REPEAT, GL_MIRRORED_REPEAT },
			{ Renderable::Material::TextureWrap::CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE },
			{ Renderable::Material::TextureWrap::CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER }
		};
#endif

		texture_wrap_s = wrap_mode_map[texture_properties.wrap_s];
		texture_wrap_t = wrap_mode_map[texture_properties.wrap_t];

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texture_wrap_s);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texture_wrap_t);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


#ifdef _WEB_PLATFORM
		if (texture_properties.wrap_s == Renderable::Material::TextureWrap::CLAMP_TO_BORDER || 
			texture_properties.wrap_t == Renderable::Material::TextureWrap::CLAMP_TO_BORDER)
		{
			Platform::get_logger().write("Warning: CLAMP_TO_BORDER is not supported on WebGL. Using CLAMP_TO_EDGE instead.");
		}

		if (texture_properties.swizzle_r != Renderable::Material::TextureSwizzle::RED ||
			texture_properties.swizzle_g != Renderable::Material::TextureSwizzle::GREEN ||
			texture_properties.swizzle_b != Renderable::Material::TextureSwizzle::BLUE ||
			texture_properties.swizzle_a != Renderable::Material::TextureSwizzle::ALPHA)
		{
			Platform::get_logger().write("Warning: Texture swizzling is not supported on WebGL. Swizzling will be ignored.");
		}
#else
    	if (texture_properties.wrap_s == Renderable::Material::TextureWrap::CLAMP_TO_BORDER || 
			texture_properties.wrap_t == Renderable::Material::TextureWrap::CLAMP_TO_BORDER)
		{
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, texture_properties.border_color);
		}

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, texture_properties.lod_bias);

		GLint swizzle_mask[4];

		std::unordered_map<Renderable::Material::TextureSwizzle, GLint> swizzle_map = {
			{ Renderable::Material::TextureSwizzle::RED, GL_RED },
			{ Renderable::Material::TextureSwizzle::GREEN, GL_GREEN },
			{ Renderable::Material::TextureSwizzle::BLUE, GL_BLUE },
			{ Renderable::Material::TextureSwizzle::ALPHA, GL_ALPHA },
			{ Renderable::Material::TextureSwizzle::ZERO, GL_ZERO },
			{ Renderable::Material::TextureSwizzle::ONE, GL_ONE }
		};

		swizzle_mask[0] = swizzle_map[texture_properties.swizzle_r];
		swizzle_mask[1] = swizzle_map[texture_properties.swizzle_g];
		swizzle_mask[2] = swizzle_map[texture_properties.swizzle_b];
		swizzle_mask[3] = swizzle_map[texture_properties.swizzle_a];

		glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzle_mask);
#endif

		uint64_t format = GL_RGBA8;
		uint64_t internal_format = GL_RGBA;

		switch (image->get_bytes_per_pixel())
		{
			case 1: 
				internal_format = GL_R8;
				format = GL_RED; 
				break;
			case 3: 
				internal_format = GL_RGB8; 
				format = GL_RGB; 
				break;
			case 4: 
				internal_format = GL_RGBA8; 
				format = GL_RGBA; 
				break;
		}

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			internal_format,
			image->get_width(),
			image->get_height(),
			0,
			format,
			GL_UNSIGNED_BYTE,
			image->get_data());

		if (texture_properties.minification_filter == Renderable::Material::TextureFilter::LINEAR_MIPMAP_LINEAR  || 
			texture_properties.minification_filter == Renderable::Material::TextureFilter::LINEAR_MIPMAP_NEAREST ||
			texture_properties.minification_filter == Renderable::Material::TextureFilter::NEAREST_MIPMAP_LINEAR || 
			texture_properties.minification_filter == Renderable::Material::TextureFilter::NEAREST_MIPMAP_NEAREST)
		{
			glGenerateMipmap(GL_TEXTURE_2D);
		}

		this->textures.emplace(image_id, texture_id);
		this->missed_frame_counts.emplace(image_id, 0);

		glActiveTexture(last_active_texture);
	}
	else
	{
		this->missed_frame_counts.at(image_id) = 0;
	}

	this->currently_bound_texture = this->textures.at(image_id);
	glActiveTexture(GL_TEXTURE0 + (uint8_t)semantic);
	glBindTexture(GL_TEXTURE_2D, this->currently_bound_texture);
}

void Omnific::OpenGLRenderDevice::unbind_texture(TextureSemantic semantic) 
{
	glActiveTexture(GL_TEXTURE0 + (uint8_t)semantic);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Omnific::OpenGLRenderDevice::bind_material(std::shared_ptr<Renderable::Material> material) 
{
	this->bind_texture(material->albedo_map, RenderDevice::TextureSemantic::ALBEDO, material->albedo_texture_properties);
	this->bind_texture(material->metallic_map, RenderDevice::TextureSemantic::METALLIC, material->metallic_texture_properties);
	this->bind_texture(material->roughness_map, RenderDevice::TextureSemantic::ROUGHNESS, material->roughness_texture_properties);
	this->bind_texture(material->emission_map, RenderDevice::TextureSemantic::EMISSION, material->emission_texture_properties);
	this->bind_texture(material->normal_map, RenderDevice::TextureSemantic::NORMAL, material->normal_texture_properties);
	this->bind_texture(material->occlusion_map, RenderDevice::TextureSemantic::OCCLUSION, material->occlusion_texture_properties);
}

void Omnific::OpenGLRenderDevice::unbind_material() 
{
	this->unbind_texture(RenderDevice::TextureSemantic::ALBEDO);
	this->unbind_texture(RenderDevice::TextureSemantic::METALLIC);
	this->unbind_texture(RenderDevice::TextureSemantic::ROUGHNESS);
	this->unbind_texture(RenderDevice::TextureSemantic::EMISSION);
	this->unbind_texture(RenderDevice::TextureSemantic::NORMAL);
	this->unbind_texture(RenderDevice::TextureSemantic::OCCLUSION);
}

void Omnific::OpenGLRenderDevice::use_shader(std::shared_ptr<Shader> shader) 
{
#ifdef _WEB_PLATFORM
    const char* GLSL_HEADER = 
        "#version 300 es\n"
        "precision mediump float;\n";
#else
    const char* GLSL_HEADER = 
        "#version 330 core\n"
        "precision mediump float;\n";
#endif

	if (shader == nullptr)
		return;

	AssetID shader_id = shader->get_id();

	if (this->shader_programs.count(shader_id) == 0)
	{
		bool compilation_success = true;

		GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
		std::string vertex_shader_source = std::string(GLSL_HEADER) + shader->get_vertex_source();
		GLchar* gl_vertex_shader_source = (GLchar*)vertex_shader_source.c_str();
		glShaderSource(vertex_shader_id, 1, &gl_vertex_shader_source, NULL);
		glCompileShader(vertex_shader_id);
		compilation_success &= this->check_shader_compile_time_errors(vertex_shader_id, GL_COMPILE_STATUS);

		GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
		std::string fragment_shader_source = std::string(GLSL_HEADER) + shader->get_fragment_source();
		GLchar* gl_fragment_shader_source = (GLchar*)fragment_shader_source.c_str();
		glShaderSource(fragment_shader_id, 1, &gl_fragment_shader_source, NULL);
		glCompileShader(fragment_shader_id);
		compilation_success &= this->check_shader_compile_time_errors(fragment_shader_id, GL_COMPILE_STATUS);

		GLuint program_id = 0;
		if (compilation_success)
		{
			program_id = glCreateProgram();
			glAttachShader(program_id, vertex_shader_id);
			glAttachShader(program_id, fragment_shader_id);
			glLinkProgram(program_id);
			compilation_success &= this->check_shader_compile_time_errors(program_id, GL_LINK_STATUS);
		}

		glDeleteShader(vertex_shader_id);
		glDeleteShader(fragment_shader_id);

		if (compilation_success)
			this->shader_programs.emplace(shader_id, program_id);
	}

	this->currently_bound_shader_program = this->shader_programs.at(shader_id);
	glUseProgram(this->currently_bound_shader_program);
}

void Omnific::OpenGLRenderDevice::use_default_2d_shader()
{
	if (this->default_2d_shader_program == 0)
	{
		this->use_shader(std::shared_ptr<Shader>(new Shader(
			this->get_default_2d_vertex_input(),
			this->get_default_2d_fragment_input(),
			false,
			false)));
	}
}

void Omnific::OpenGLRenderDevice::use_default_3d_shader()
{
	if (this->default_3d_shader_program == 0)
	{
		this->use_shader(std::shared_ptr<Shader>(new Shader(
			this->get_default_3d_vertex_input(),
			this->get_default_3d_fragment_input(),
			false,
			false)));
	}
}

void Omnific::OpenGLRenderDevice::set_int_uniform(std::string name, int value)
{
	glUniform1i(glGetUniformLocation(this->currently_bound_shader_program, name.c_str()), value);
}

void Omnific::OpenGLRenderDevice::set_bool_uniform(std::string name, bool value)
{
	glUniform1i(glGetUniformLocation(this->currently_bound_shader_program, name.c_str()), (int)value);
}

void Omnific::OpenGLRenderDevice::set_float_uniform(std::string name, float value)
{
	glUniform1f(glGetUniformLocation(this->currently_bound_shader_program, name.c_str()), value);
}

void Omnific::OpenGLRenderDevice::set_vec2_uniform(std::string name, glm::vec2 value)
{
	glUniform2f(glGetUniformLocation(this->currently_bound_shader_program, name.c_str()), value.x, value.y);
}

void Omnific::OpenGLRenderDevice::set_vec3_uniform(std::string name, glm::vec3 value)
{
	glUniform3f(glGetUniformLocation(this->currently_bound_shader_program, name.c_str()), value.x, value.y, value.z);
}

void Omnific::OpenGLRenderDevice::set_vec4_uniform(std::string name, glm::vec4 value)
{
	glUniform4f(glGetUniformLocation(this->currently_bound_shader_program, name.c_str()), value.x, value.y, value.z, value.w);
}

void Omnific::OpenGLRenderDevice::set_mat4_uniform(std::string name, glm::mat4 value)
{
	glUniformMatrix4fv(glGetUniformLocation(this->currently_bound_shader_program, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

void Omnific::OpenGLRenderDevice::set_int_array_uniform(std::string name, std::vector<int> values)
{
	glUniform1iv(glGetUniformLocation(this->currently_bound_shader_program, name.c_str()), (GLsizei)values.size(), values.data());
}

void Omnific::OpenGLRenderDevice::set_bool_array_uniform(std::string name, std::vector<bool> values)
{
	std::vector<int> int_values;
	for (int i = 0; i < values.size(); ++i)
		int_values.push_back((int)values.at(i));
	glUniform1iv(glGetUniformLocation(this->currently_bound_shader_program, name.c_str()), (GLsizei)values.size(), int_values.data());
}

void Omnific::OpenGLRenderDevice::set_float_array_uniform(std::string name, std::vector<float> values)
{
	glUniform1fv(glGetUniformLocation(this->currently_bound_shader_program, name.c_str()), (GLsizei)values.size(), values.data());
}

void Omnific::OpenGLRenderDevice::set_vec2_array_uniform(std::string name, std::vector<glm::vec2> values)
{
	std::vector<float> vec2_values;
	for (int i = 0; i < values.size(); ++i)
	{
		glm::vec2 vec2_value = values.at(i);
		vec2_values.push_back(vec2_value.x);
		vec2_values.push_back(vec2_value.y);
	}
	glUniform2fv(glGetUniformLocation(this->currently_bound_shader_program, name.c_str()), (GLsizei)values.size(), vec2_values.data());
}

void Omnific::OpenGLRenderDevice::set_vec3_array_uniform(std::string name, std::vector<glm::vec3> values)
{
	std::vector<float> vec3_values;
	for (int i = 0; i < values.size(); ++i)
	{
		glm::vec3 vec3_value = values.at(i);
		vec3_values.push_back(vec3_value.x);
		vec3_values.push_back(vec3_value.y);
		vec3_values.push_back(vec3_value.z);
	}
	glUniform3fv(glGetUniformLocation(this->currently_bound_shader_program, name.c_str()), (GLsizei)values.size(), vec3_values.data());
}

void Omnific::OpenGLRenderDevice::set_vec4_array_uniform(std::string name, std::vector<glm::vec4> values)
{
	std::vector<float> vec4_values;
	for (int i = 0; i < values.size(); ++i)
	{
		glm::vec4 vec4_value = values.at(i);
		vec4_values.push_back(vec4_value.x);
		vec4_values.push_back(vec4_value.y);
		vec4_values.push_back(vec4_value.z);
		vec4_values.push_back(vec4_value.w);
	}
	glUniform4fv(glGetUniformLocation(this->currently_bound_shader_program, name.c_str()), (GLsizei)values.size(), vec4_values.data());
}

bool Omnific::OpenGLRenderDevice::check_shader_compile_time_errors(GLuint ID, GLuint status)
{
	GLint compilation_success = GL_FALSE;
	char info_log[512];

	if (status == GL_COMPILE_STATUS)
		glGetShaderiv(ID, status, &compilation_success);
	else if (status == GL_LINK_STATUS)
		glGetProgramiv(ID, status, &compilation_success);

	if (!compilation_success)
	{
		glGetShaderInfoLog(ID, 512, NULL, info_log);
		Platform::get_logger().write(info_log);
	}

	return compilation_success;
}
