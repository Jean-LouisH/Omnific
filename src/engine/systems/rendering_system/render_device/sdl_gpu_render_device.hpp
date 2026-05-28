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

#include <foundations/singletons/platform/window.hpp>
#include <foundations/resources/image.hpp>
#include <foundations/resources/shader.hpp>
#include <scene/scene.hpp>
#include "foundations/aliases.hpp"
#include <string>
#include <unordered_map>
#include "render_device.hpp"

namespace Omnific
{
	/* The object that calls the rendering API functions. */
	class SDLGPURenderDevice: public RenderDevice
	{
	public:
		virtual void initialize() override;
		virtual void clear_colour_buffer(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) override;
		virtual void clear_colour_buffer(glm::vec4 colour) override;
		virtual void clear_depth_buffer() override;
        virtual void clear_stencil_buffer() override;
		virtual void enable_depth_test() override;
		virtual void disable_depth_test() override;
        virtual void enable_depth_mask() override;
        virtual void disable_depth_mask() override;
        virtual void colour_mask(bool red, bool green, bool blue, bool alpha) override;
		virtual void enable_wireframe_mode() override;
		virtual void disable_wireframe_mode() override;
		virtual void enable_blending() override;
		virtual void disable_blending() override;
        virtual void enable_stencil_test() override;
        virtual void disable_stencil_test() override;
        virtual void enable_scissor_test() override;
        virtual void disable_scissor_test() override;
        virtual void set_scissor_rectangle(int x, int y, int width, int height) override;
		virtual void set_viewport(uint32_t width, uint32_t height) override;
        virtual void stencil_function(RenderDevice::StencilFunction func, int ref, uint32_t mask) override;
        virtual void stencil_operation(RenderDevice::StencilOperation sfail, RenderDevice::StencilOperation dpfail, RenderDevice::StencilOperation dppass) override;
        virtual void stencil_mask(uint32_t mask) override;
		virtual void enable_face_culling() override;
		virtual void disable_face_culling() override;
		virtual void set_face_culling_to_front() override;
		virtual void set_face_culling_to_back() override;
		virtual void set_face_culling_to_front_and_back() override;
		virtual void draw_indexed_triangles(std::shared_ptr<Mesh> mesh) override;
		virtual void draw_triangles(std::shared_ptr<Mesh> mesh) override;
		virtual void draw_points(std::shared_ptr<Mesh> mesh) override;
		virtual void draw_line_strip(std::shared_ptr<Mesh> mesh) override;
		virtual std::string get_name() override;
		virtual std::string get_default_2d_vertex_input() override;
		virtual std::string get_default_2d_fragment_input() override;
		virtual std::string get_default_3d_vertex_input() override;
		virtual std::string get_default_3d_fragment_input() override;
		virtual std::string get_light_source_fragment_input() override;
		virtual std::string get_unlit_fragment_input() override;
		virtual std::string get_simple_fragment_input() override;
		virtual std::string get_pbr_fragment_input() override;
        virtual void bind_mesh(std::shared_ptr<Mesh> mesh) override;
        virtual void unbind_mesh() override;
        virtual void bind_texture(std::shared_ptr<Image> image, TextureSemantic semantic, const Renderable::Material::TextureProperties& texture_properties) override;
        virtual void unbind_texture(TextureSemantic semantic) override;
        virtual void bind_material(std::shared_ptr<Renderable::Material> material) override;
        virtual void unbind_material() override;
        virtual void use_shader(std::shared_ptr<Shader> shader) override;
		virtual void use_default_2d_shader() override;
		virtual void use_default_3d_shader() override;
        virtual void set_int_uniform(std::string name, int value) override;
		virtual void set_bool_uniform(std::string name, bool value) override;
		virtual void set_float_uniform(std::string name, float value) override;
		virtual void set_vec2_uniform(std::string name, glm::vec2 value) override;
		virtual void set_vec3_uniform(std::string name, glm::vec3 value) override;
		virtual void set_vec4_uniform(std::string name, glm::vec4 value) override;
		virtual void set_mat4_uniform(std::string name, glm::mat4 value) override;
		virtual void set_int_array_uniform(std::string name, std::vector<int> values) override;
		virtual void set_bool_array_uniform(std::string name, std::vector<bool> values) override;
		virtual void set_float_array_uniform(std::string name, std::vector<float> values) override;
		virtual void set_vec2_array_uniform(std::string name, std::vector<glm::vec2> values) override;
		virtual void set_vec3_array_uniform(std::string name, std::vector<glm::vec3> values) override;
		virtual void set_vec4_array_uniform(std::string name, std::vector<glm::vec4> values) override;
		virtual void collect_garbage() override;
	private:
	};
}

