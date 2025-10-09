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

#include "opengl_rendering_backend.hpp"
#include <foundations/resources/default_assets/shaders/fragment_light_source_glsl.hpp>
#include <foundations/resources/default_assets/shaders/fragment_pbr_glsl.hpp>
#include <foundations/resources/default_assets/shaders/fragment_simple_glsl.hpp>
#include <foundations/resources/default_assets/shaders/vertex_2d_glsl.hpp>
#include <foundations/resources/default_assets/shaders/vertex_3d_glsl.hpp>
#include <foundations/resources/default_assets/shaders/fragment_unlit_glsl.hpp>
#include <foundations/singletons/platform/platform.hpp>
#include <foundations/singletons/profiler.hpp>
#include <string>
#include <SDL_video.h>

#ifdef _WEB_PLATFORM
    #include <GLES3/gl3.h>
#else
    #include <glad/glad.h>
#endif

void Omnific::OpenGLRenderingBackend::initialize()
{
	Window& window = Platform::get_window();
#ifdef _WEB_PLATFORM
	window.initialize_window_context("webgl");
#else
	window.initialize_window_context("opengl");

	if ((!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)))
	{
		Platform::get_logger().write("GLAD failed to initialize.");
	}
	else
#endif
	{
		this->built_in_shader_program_2d = std::shared_ptr<OpenGLShaderProgram>(new OpenGLShaderProgram(std::shared_ptr<Shader>(new Shader(
			this->get_default_2d_vertex_input(),
			this->get_default_2d_fragment_input(),
			false,
			false))));

		this->built_in_shader_program_3d = std::shared_ptr<OpenGLShaderProgram>(new OpenGLShaderProgram(std::shared_ptr<Shader>(new Shader(
			this->get_default_3d_vertex_input(),
			this->get_default_3d_fragment_input(),
			false,
			false))));

		glm::vec2 window_size = window.get_window_size();
		this->set_viewport(window_size.x, window_size.y);
		std::string rendering_backend_name = (std::string)("OpenGL ") + (char*)glGetString(GL_VERSION);
		Profiler::set_rendering_backend_name(rendering_backend_name);
		Platform::get_logger().write("Rendering Backend initialized with " + rendering_backend_name);
	}
}

void Omnific::OpenGLRenderingBackend::clear_colour_buffer(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
{
	glClearColor(
		(float)(red) / 255.0, 
		(float)(green) / 255.0, 
		(float)(blue) / 255.0, 
		(float)(alpha) / 255.0);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Omnific::OpenGLRenderingBackend::clear_depth_buffer()
{
	glClear(GL_DEPTH_BUFFER_BIT);
}

void Omnific::OpenGLRenderingBackend::enable_depth_test()
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
}

void Omnific::OpenGLRenderingBackend::disable_depth_test()
{
	glDisable(GL_DEPTH_TEST);
}

void Omnific::OpenGLRenderingBackend::enable_wireframe_mode()
{
#ifndef _WEB_PLATFORM
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif
}

void Omnific::OpenGLRenderingBackend::disable_wireframe_mode()
{
#ifndef _WEB_PLATFORM
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
#endif
}

void Omnific::OpenGLRenderingBackend::enable_blending()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Omnific::OpenGLRenderingBackend::disable_blending()
{
	glDisable(GL_BLEND);
}

void Omnific::OpenGLRenderingBackend::set_viewport(uint32_t width, uint32_t height)
{
	glViewport(0, 0, width, height);
}

void Omnific::OpenGLRenderingBackend::enable_face_culling()
{
	glEnable(GL_CULL_FACE);
}

void Omnific::OpenGLRenderingBackend::disable_face_culling()
{
	glDisable(GL_CULL_FACE);
}

void Omnific::OpenGLRenderingBackend::set_face_culling_to_front()
{
	glCullFace(GL_FRONT);
}

void Omnific::OpenGLRenderingBackend::set_face_culling_to_back()
{
	glCullFace(GL_BACK);
}

void Omnific::OpenGLRenderingBackend::set_face_culling_to_front_and_back()
{
	glCullFace(GL_FRONT_AND_BACK);
}

void Omnific::OpenGLRenderingBackend::draw_triangles_from_elements(unsigned int index_count)
{
	glDrawElements(GL_TRIANGLES, (GLsizei)index_count, GL_UNSIGNED_INT, 0);
}

void Omnific::OpenGLRenderingBackend::draw_triangles_from_arrays(unsigned int vertex_count)
{
	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)vertex_count);
}

void Omnific::OpenGLRenderingBackend::draw_points_from_arrays(unsigned int vertex_count)
{
	glDrawArrays(GL_POINTS, 0, (GLsizei)vertex_count);
}

void Omnific::OpenGLRenderingBackend::draw_line_strip_from_arrays(unsigned int vertex_count)
{
	glDrawArrays(GL_LINE_STRIP, 0, (GLsizei)vertex_count);
}

void Omnific::OpenGLRenderingBackend::swap_buffers()
{
	SDL_GL_SwapWindow(Platform::get_window().get_sdlwindow());
}

std::string Omnific::OpenGLRenderingBackend::get_rendering_backend_name()
{
	return "opengl";
}

std::string Omnific::OpenGLRenderingBackend::get_default_2d_vertex_input()
{
	return DefaultAssets::vertex_2d_glsl;
}

std::string Omnific::OpenGLRenderingBackend::get_default_2d_fragment_input()
{
	return this->get_unlit_fragment_input();
}

std::string Omnific::OpenGLRenderingBackend::get_default_3d_vertex_input()
{
	return DefaultAssets::vertex_3d_glsl;
}

std::string Omnific::OpenGLRenderingBackend::get_default_3d_fragment_input()
{
	return this->get_pbr_fragment_input();
}

std::string Omnific::OpenGLRenderingBackend::get_light_source_fragment_input()
{
	return DefaultAssets::fragment_light_source_glsl;
}

std::string Omnific::OpenGLRenderingBackend::get_unlit_fragment_input()
{
	return DefaultAssets::fragment_unlit_glsl;
}

std::string Omnific::OpenGLRenderingBackend::get_simple_fragment_input()
{
	return DefaultAssets::fragment_simple_glsl;
}

std::string Omnific::OpenGLRenderingBackend::get_pbr_fragment_input()
{
	return DefaultAssets::fragment_pbr_glsl;
}

std::shared_ptr<Omnific::OpenGLTexture> Omnific::OpenGLRenderingBackend::get_texture(std::shared_ptr<Image> image)
{
	std::shared_ptr<OpenGLTexture> texture;

	if (image != nullptr)
	{
		AssetID asset_id = image->get_id();

		if (this->textures.count(asset_id) == 0)
		{
			this->textures.emplace(asset_id, std::shared_ptr<OpenGLTexture>(new OpenGLTexture(image)));
			this->missed_frame_counts.emplace(asset_id, 0);
		}
		else
		{
			this->missed_frame_counts.at(asset_id) = 0;
		}

		texture = this->textures.at(asset_id);
	}
	else
	{
		texture = std::shared_ptr<OpenGLTexture>(new OpenGLTexture());
	}

	return texture;
}

std::shared_ptr<Omnific::OpenGLVertexArray> Omnific::OpenGLRenderingBackend::get_vertex_array(std::shared_ptr<Mesh> mesh)
{
	AssetID mesh_id;
	
	if (mesh != nullptr)
		mesh_id = mesh->get_id();

	if (this->vertex_arrays.count(mesh_id) == 0)
	{
		this->vertex_arrays.emplace(mesh_id, std::shared_ptr<OpenGLVertexArray>(new OpenGLVertexArray(mesh)));
		this->missed_frame_counts.emplace(mesh_id, 0);
	}
	else
	{
		this->missed_frame_counts.at(mesh_id) = 0;
	}

	return this->vertex_arrays.at(mesh_id);
}

void Omnific::OpenGLRenderingBackend::collect_garbage()
{
	std::vector<AssetID> assets_to_delete;

	for (auto it = this->missed_frame_counts.begin(); it != this->missed_frame_counts.end(); it++)
	{
		if (this->missed_frame_counts.at(it->first) > this->allowable_missed_frames)
		{
			if (this->textures.count(it->first) > 0)
				this->textures.erase(it->first);
			if (this->vertex_arrays.count(it->first) > 0)
				this->vertex_arrays.erase(it->first);

			assets_to_delete.push_back(it->first);
		}
	}

	for (size_t i = 0; i < assets_to_delete.size(); ++i)
		this->missed_frame_counts.erase(assets_to_delete.at(i));

	for (auto it = this->missed_frame_counts.begin(); it != this->missed_frame_counts.end(); it++)
		this->missed_frame_counts.at(it->first)++;
}