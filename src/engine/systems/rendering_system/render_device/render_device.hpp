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

#include <foundations/resources/image.hpp>
#include <foundations/resources/shader.hpp>
#include <foundations/resources/mesh.hpp>
#include <scene/scene.hpp>
#include <string>

namespace Omnific
{
	/* The object that calls the rendering API functions. */
    // Virtual interface for other rendering backends
	class RenderDevice
	{
	public:
        enum class TextureSemantic 
        { 
            ALBEDO, 
            METALLIC, 
            ROUGHNESS, 
            EMISSION, 
            NORMAL, 
            OCCLUSION 
        };

        enum class StencilFunction
        {
            NEVER,
            LESS,
            EQUAL,
            LEQUAL,
            GREATER,
            NOTEQUAL,
            GEQUAL,
            ALWAYS
        };

        enum class StencilOperation
        {
            KEEP,
            ZERO,
            REPLACE,
            INCR,
            INCR_WRAP,
            DECR,
            DECR_WRAP,
            INVERT
        };

        virtual ~RenderDevice() = default;
		virtual void initialize() = 0;
		virtual void clear_colour_buffer(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) = 0;
		virtual void clear_colour_buffer(glm::vec4 colour) = 0;
		virtual void clear_depth_buffer() = 0;
        virtual void clear_stencil_buffer() = 0;
		virtual void enable_depth_test() = 0;
		virtual void disable_depth_test() = 0;
        virtual void enable_depth_mask() = 0;
        virtual void disable_depth_mask() = 0;
        virtual void colour_mask(bool red, bool green, bool blue, bool alpha) = 0;
		virtual void enable_wireframe_mode() = 0;
		virtual void disable_wireframe_mode() = 0;
		virtual void enable_blending() = 0;
		virtual void disable_blending() = 0;
        virtual void enable_stencil_test() = 0;
        virtual void disable_stencil_test() = 0;
        virtual void enable_scissor_test() = 0;
        virtual void disable_scissor_test() = 0;
        virtual void set_scissor_rectangle(int x, int y, int width, int height) = 0;
		virtual void set_viewport(uint32_t width, uint32_t height) = 0;
        virtual void stencil_function(RenderDevice::StencilFunction func, int ref, uint32_t mask) = 0;
        virtual void stencil_operation(RenderDevice::StencilOperation sfail, RenderDevice::StencilOperation dpfail, RenderDevice::StencilOperation dppass) = 0;
        virtual void stencil_mask(uint32_t mask) = 0;
		virtual void enable_face_culling() = 0;
		virtual void disable_face_culling() = 0;
		virtual void set_face_culling_to_front() = 0;
		virtual void set_face_culling_to_back() = 0;
		virtual void set_face_culling_to_front_and_back() = 0;
		virtual void draw_indexed_triangles(std::shared_ptr<Mesh> mesh) = 0;
		virtual void draw_triangles(std::shared_ptr<Mesh> mesh) = 0;
		virtual void draw_points(std::shared_ptr<Mesh> mesh) = 0;
		virtual void draw_line_strip(std::shared_ptr<Mesh> mesh) = 0;
		virtual std::string get_name() = 0;
		virtual std::string get_default_2d_vertex_input() = 0;
		virtual std::string get_default_2d_fragment_input() = 0;
		virtual std::string get_default_3d_vertex_input() = 0;
		virtual std::string get_default_3d_fragment_input() = 0;
		virtual std::string get_light_source_fragment_input() = 0;
		virtual std::string get_unlit_fragment_input() = 0;
		virtual std::string get_simple_fragment_input() = 0;
		virtual std::string get_pbr_fragment_input() = 0;
        virtual void bind_mesh(std::shared_ptr<Mesh> mesh) = 0;
        virtual void unbind_mesh() = 0;
        virtual void bind_texture(std::shared_ptr<Image> image, TextureSemantic semantic, const Renderable::Material::TextureProperties& texture_properties) = 0;
        virtual void unbind_texture(TextureSemantic semantic) = 0;
        virtual void bind_material(std::shared_ptr<Renderable::Material> material) = 0;
        virtual void unbind_material() = 0;
        virtual void use_shader(std::shared_ptr<Shader> shader) = 0;
        virtual void use_default_2d_shader() = 0;
		virtual void use_default_3d_shader() = 0;
        virtual void set_int_uniform(std::string name, int value) = 0;
		virtual void set_bool_uniform(std::string name, bool value) = 0;
		virtual void set_float_uniform(std::string name, float value) = 0;
		virtual void set_vec2_uniform(std::string name, glm::vec2 value) = 0;
		virtual void set_vec3_uniform(std::string name, glm::vec3 value) = 0;
		virtual void set_vec4_uniform(std::string name, glm::vec4 value) = 0;
		virtual void set_mat4_uniform(std::string name, glm::mat4 value) = 0;
		virtual void set_int_array_uniform(std::string name, std::vector<int> values) = 0;
		virtual void set_bool_array_uniform(std::string name, std::vector<bool> values) = 0;
		virtual void set_float_array_uniform(std::string name, std::vector<float> values) = 0;
		virtual void set_vec2_array_uniform(std::string name, std::vector<glm::vec2> values) = 0;
		virtual void set_vec3_array_uniform(std::string name, std::vector<glm::vec3> values) = 0;
		virtual void set_vec4_array_uniform(std::string name, std::vector<glm::vec4> values) = 0;
		virtual void collect_garbage() = 0;
	private:
	};
}

