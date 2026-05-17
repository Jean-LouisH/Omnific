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

#include "vulkan_render_device.hpp"
#include <foundations/singletons/platform/platform.hpp>
#include <foundations/singletons/profiler.hpp>
#include <string>
#include <SDL3/SDL_video.h>


void Omnific::VulkanRenderDevice::initialize()
{

}

void Omnific::VulkanRenderDevice::clear_colour_buffer(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
{

}

void Omnific::VulkanRenderDevice::clear_colour_buffer(glm::vec4 colour)
{

}

void Omnific::VulkanRenderDevice::clear_depth_buffer()
{

}

void Omnific::VulkanRenderDevice::enable_depth_test()
{

}

void Omnific::VulkanRenderDevice::disable_depth_test()
{

}

void Omnific::VulkanRenderDevice::enable_wireframe_mode()
{

}

void Omnific::VulkanRenderDevice::disable_wireframe_mode()
{

}

void Omnific::VulkanRenderDevice::enable_blending()
{

}

void Omnific::VulkanRenderDevice::disable_blending()
{

}

void Omnific::VulkanRenderDevice::set_viewport(uint32_t width, uint32_t height)
{

}

void Omnific::VulkanRenderDevice::enable_face_culling()
{

}

void Omnific::VulkanRenderDevice::disable_face_culling()
{

}

void Omnific::VulkanRenderDevice::set_face_culling_to_front()
{

}

void Omnific::VulkanRenderDevice::set_face_culling_to_back()
{

}

void Omnific::VulkanRenderDevice::set_face_culling_to_front_and_back()
{

}

void Omnific::VulkanRenderDevice::draw_indexed_triangles(std::shared_ptr<Mesh> mesh)
{

}

void Omnific::VulkanRenderDevice::draw_triangles(std::shared_ptr<Mesh> mesh)
{

}

void Omnific::VulkanRenderDevice::draw_points(std::shared_ptr<Mesh> mesh)
{

}

void Omnific::VulkanRenderDevice::draw_line_strip(std::shared_ptr<Mesh> mesh)
{

}

std::string Omnific::VulkanRenderDevice::get_name()
{
	return "vulkan";
}

std::string Omnific::VulkanRenderDevice::get_default_2d_vertex_input()
{
	return "";
}

std::string Omnific::VulkanRenderDevice::get_default_2d_fragment_input()
{
	return this->get_unlit_fragment_input();
}

std::string Omnific::VulkanRenderDevice::get_default_3d_vertex_input()
{
	return "";
}

std::string Omnific::VulkanRenderDevice::get_default_3d_fragment_input()
{
	return this->get_pbr_fragment_input();
}

std::string Omnific::VulkanRenderDevice::get_light_source_fragment_input()
{
	return "";
}

std::string Omnific::VulkanRenderDevice::get_unlit_fragment_input()
{
	return "";
}

std::string Omnific::VulkanRenderDevice::get_simple_fragment_input()
{
	return "";
}

std::string Omnific::VulkanRenderDevice::get_pbr_fragment_input()
{
	return "";
}

void Omnific::VulkanRenderDevice::collect_garbage()
{

}

void Omnific::VulkanRenderDevice::clear_stencil_buffer() 
{

}

void Omnific::VulkanRenderDevice::enable_depth_mask() 
{

}

void Omnific::VulkanRenderDevice::disable_depth_mask() 
{

}

void Omnific::VulkanRenderDevice::colour_mask(bool red, bool green, bool blue, bool alpha) 
{

}

void Omnific::VulkanRenderDevice::enable_stencil_test() 
{

}

void Omnific::VulkanRenderDevice::disable_stencil_test() 
{

}

void Omnific::VulkanRenderDevice::enable_scissor_test() 
{

}

void Omnific::VulkanRenderDevice::disable_scissor_test() 
{

}

void Omnific::VulkanRenderDevice::set_scissor_rectangle(int x, int y, int width, int height) 
{

}

void Omnific::VulkanRenderDevice::stencil_function(RenderDevice::StencilFunction func, int ref, uint32_t mask) 
{

}

void Omnific::VulkanRenderDevice::stencil_operation(RenderDevice::StencilOperation sfail, RenderDevice::StencilOperation dpfail, RenderDevice::StencilOperation dppass) 
{

}

void Omnific::VulkanRenderDevice::stencil_mask(uint32_t mask) 
{

}

void Omnific::VulkanRenderDevice::bind_mesh(std::shared_ptr<Mesh> mesh) 
{

}

void Omnific::VulkanRenderDevice::unbind_mesh() 
{

}


void Omnific::VulkanRenderDevice::bind_texture(std::shared_ptr<Image> image, TextureSemantic semantic) 
{

}

void Omnific::VulkanRenderDevice::unbind_texture(TextureSemantic semantic) 
{

}

void Omnific::VulkanRenderDevice::bind_material(std::shared_ptr<Renderable::Material> material) 
{
	this->bind_texture(material->albedo_map, RenderDevice::TextureSemantic::ALBEDO);
	this->bind_texture(material->metallic_map, RenderDevice::TextureSemantic::METALLIC);
	this->bind_texture(material->roughness_map, RenderDevice::TextureSemantic::ROUGHNESS);
	this->bind_texture(material->emission_map, RenderDevice::TextureSemantic::EMISSION);
	this->bind_texture(material->normal_map, RenderDevice::TextureSemantic::NORMAL);
	this->bind_texture(material->occlusion_map, RenderDevice::TextureSemantic::OCCLUSION);
}

void Omnific::VulkanRenderDevice::unbind_material() 
{
	this->unbind_texture(RenderDevice::TextureSemantic::ALBEDO);
	this->unbind_texture(RenderDevice::TextureSemantic::METALLIC);
	this->unbind_texture(RenderDevice::TextureSemantic::ROUGHNESS);
	this->unbind_texture(RenderDevice::TextureSemantic::EMISSION);
	this->unbind_texture(RenderDevice::TextureSemantic::NORMAL);
	this->unbind_texture(RenderDevice::TextureSemantic::OCCLUSION);
}

void Omnific::VulkanRenderDevice::use_shader(std::shared_ptr<Shader> shader) 
{

}

void Omnific::VulkanRenderDevice::use_default_2d_shader()
{

}

void Omnific::VulkanRenderDevice::use_default_3d_shader()
{

}

void Omnific::VulkanRenderDevice::set_int_uniform(std::string name, int value)
{

}

void Omnific::VulkanRenderDevice::set_bool_uniform(std::string name, bool value)
{

}

void Omnific::VulkanRenderDevice::set_float_uniform(std::string name, float value)
{

}

void Omnific::VulkanRenderDevice::set_vec2_uniform(std::string name, glm::vec2 value)
{

}

void Omnific::VulkanRenderDevice::set_vec3_uniform(std::string name, glm::vec3 value)
{

}

void Omnific::VulkanRenderDevice::set_vec4_uniform(std::string name, glm::vec4 value)
{

}

void Omnific::VulkanRenderDevice::set_mat4_uniform(std::string name, glm::mat4 value)
{

}

void Omnific::VulkanRenderDevice::set_int_array_uniform(std::string name, std::vector<int> values)
{

}

void Omnific::VulkanRenderDevice::set_bool_array_uniform(std::string name, std::vector<bool> values)
{

}

void Omnific::VulkanRenderDevice::set_float_array_uniform(std::string name, std::vector<float> values)
{

}

void Omnific::VulkanRenderDevice::set_vec2_array_uniform(std::string name, std::vector<glm::vec2> values)
{

}

void Omnific::VulkanRenderDevice::set_vec3_array_uniform(std::string name, std::vector<glm::vec3> values)
{

}

void Omnific::VulkanRenderDevice::set_vec4_array_uniform(std::string name, std::vector<glm::vec4> values)
{

}
