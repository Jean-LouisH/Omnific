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
#include "opengl_built_in_shaders.hpp"
#include <foundations/singletons/platform/platform.hpp>
#include <string>
#include <SDL_video.h>

void Omnia::OpenGLRenderingBackend::initialize()
{
	Window& window = Platform::get_window();
	window.initialize_window_context("opengl");

	if ((!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)))
	{
		Platform::get_logger().write("GLAD failed to initialize.");
	}
	else
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
		Platform::get_logger().write((std::string)("Rendering Backend initialized with ") +
			"OpenGL " + (char*)glGetString(GL_VERSION));
	}
}

void Omnia::OpenGLRenderingBackend::clear_colour_buffer(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
{
	glClearColor(
		(float)(red) / 255.0, 
		(float)(green) / 255.0, 
		(float)(blue) / 255.0, 
		(float)(alpha) / 255.0);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Omnia::OpenGLRenderingBackend::clear_depth_buffer()
{
	glClear(GL_DEPTH_BUFFER_BIT);
}

void Omnia::OpenGLRenderingBackend::enable_depth_test()
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
}

void Omnia::OpenGLRenderingBackend::disable_depth_test()
{
	glDisable(GL_DEPTH_TEST);
}

void Omnia::OpenGLRenderingBackend::enable_wireframe_mode()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void Omnia::OpenGLRenderingBackend::disable_wireframe_mode()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Omnia::OpenGLRenderingBackend::enable_blending()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Omnia::OpenGLRenderingBackend::disable_blending()
{
	glDisable(GL_BLEND);
}

void Omnia::OpenGLRenderingBackend::set_viewport(uint32_t width, uint32_t height)
{
	glViewport(0, 0, width, height);
}

void Omnia::OpenGLRenderingBackend::enable_face_culling()
{
	glEnable(GL_CULL_FACE);
}

void Omnia::OpenGLRenderingBackend::disable_face_culling()
{
	glDisable(GL_CULL_FACE);
}

void Omnia::OpenGLRenderingBackend::set_face_culling_to_front()
{
	glCullFace(GL_FRONT);
}

void Omnia::OpenGLRenderingBackend::set_face_culling_to_back()
{
	glCullFace(GL_BACK);
}

void Omnia::OpenGLRenderingBackend::set_face_culling_to_front_and_back()
{
	glCullFace(GL_FRONT_AND_BACK);
}

void Omnia::OpenGLRenderingBackend::draw_triangles_from_elements(unsigned int index_count)
{
	glDrawElements(GL_TRIANGLES, (GLsizei)index_count, GL_UNSIGNED_INT, 0);
}

void Omnia::OpenGLRenderingBackend::draw_triangles_from_arrays(unsigned int vertex_count)
{
	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)vertex_count);
}

void Omnia::OpenGLRenderingBackend::swap_buffers()
{
	SDL_GL_SwapWindow(Platform::get_window().get_sdlwindow());
}

std::string Omnia::OpenGLRenderingBackend::get_rendering_backend_name()
{
	return "opengl";
}

std::string Omnia::OpenGLRenderingBackend::get_default_2d_vertex_input()
{
	return OpenGLBuiltInShaders::Vertex::standard_2d;
}

std::string Omnia::OpenGLRenderingBackend::get_default_2d_fragment_input()
{
	return OpenGLBuiltInShaders::Fragment::image_2d;
}

std::string Omnia::OpenGLRenderingBackend::get_default_3d_vertex_input()
{
	return OpenGLBuiltInShaders::Vertex::standard_3d;
}

std::string Omnia::OpenGLRenderingBackend::get_default_3d_fragment_input()
{
	return this->get_pbr_fragment_input();
}

std::string Omnia::OpenGLRenderingBackend::get_light_source_fragment_input()
{
	return OpenGLBuiltInShaders::Fragment::light_source_3d;
}

std::string Omnia::OpenGLRenderingBackend::get_unlit_fragment_input()
{
	return OpenGLBuiltInShaders::Fragment::unlit_3d;
}

std::string Omnia::OpenGLRenderingBackend::get_phong_fragment_input()
{
	return OpenGLBuiltInShaders::Fragment::phong_3d;
}

std::string Omnia::OpenGLRenderingBackend::get_pbr_fragment_input()
{
	return OpenGLBuiltInShaders::Fragment::pbr_3d;
}

std::shared_ptr<Omnia::OpenGLTexture> Omnia::OpenGLRenderingBackend::get_texture(std::shared_ptr<Image> image)
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

std::shared_ptr<Omnia::OpenGLVertexArray> Omnia::OpenGLRenderingBackend::get_vertex_array(std::shared_ptr<Mesh> mesh)
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

void Omnia::OpenGLRenderingBackend::collect_garbage()
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

	for (size_t i = 0; i < assets_to_delete.size(); i++)
		this->missed_frame_counts.erase(assets_to_delete.at(i));

	for (auto it = this->missed_frame_counts.begin(); it != this->missed_frame_counts.end(); it++)
		this->missed_frame_counts.at(it->first)++;
}