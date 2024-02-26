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

#include <core/singletons/os/window.hpp>
#include <glad/glad.h>
#include <vector>
#include "opengl_shader_program.hpp"
#include "opengl_texture.hpp"
#include "opengl_vertex_array.hpp"
#include <core/assets/image.hpp>
#include <core/assets/shader.hpp>
#include <core/scene/scene.hpp>
#include "../../renderable_layer.hpp"
#include "core/utilities/aliases.hpp"
#include <string>
#include <map>

namespace Omnia
{
	/* The object that calls the rendering API functions. */
	class OpenGLRenderingBackend
	{
	public:
		std::unordered_map<AssetID, std::shared_ptr<OpenGLShaderProgram>> shader_programs;
		std::shared_ptr<OpenGLShaderProgram> built_in_shader_program_2d;
		std::shared_ptr<OpenGLShaderProgram> built_in_shader_program_3d;
		std::unordered_map<AssetID, std::shared_ptr<OpenGLTexture>> textures;
		std::unordered_map<AssetID, std::shared_ptr<OpenGLVertexArray>> vertex_arrays;
		uint8_t allowable_missed_frames = 0;
		std::unordered_map<AssetID, uint8_t> missed_frame_counts;

		void initialize();
		void clear_colour_buffer(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);
		void clear_depth_buffer();
		void enable_depth_test();
		void disable_depth_test();
		void enable_wireframe_mode();
		void disable_wireframe_mode();
		void enable_blending();
		void disable_blending();
		void set_viewport(uint32_t width, uint32_t height);
		void enable_face_culling();
		void disable_face_culling();
		void set_face_culling_to_front();
		void set_face_culling_to_back();
		void set_face_culling_to_front_and_back();
		void draw_triangles_from_elements(unsigned int index_count);
		void draw_triangles_from_arrays(unsigned int vertex_count);
		void swap_buffers();
		std::string get_rendering_backend_name();
		std::string get_default_2d_vertex_input();
		std::string get_default_2d_fragment_input();
		std::string get_default_3d_vertex_input();
		std::string get_default_3d_fragment_input();
		std::string get_light_source_fragment_input();
		std::string get_unlit_fragment_input();
		std::string get_phong_fragment_input();
		std::string get_pbr_fragment_input();
		std::shared_ptr<OpenGLTexture> get_texture(std::shared_ptr<Image> image);
		std::shared_ptr<OpenGLVertexArray> get_vertex_array(std::shared_ptr<Mesh> mesh);
		void collect_garbage();
	private:
	};
}

