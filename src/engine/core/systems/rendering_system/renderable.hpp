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

#include <vector>
#include <core/components/transform.hpp>
#include <core/components/model.hpp>

#include <core/components/camera.hpp>
#include <core/assets/image.hpp>
#include <core/assets/material.hpp>
#include <core/assets/mesh.hpp>
#include <core/assets/shader.hpp>
#include <glm/glm.hpp>
#include <memory>

namespace Omnia
{
	/* Caches memory locations for values 
	   relevant to rendering a single Entity. */
	class Renderable
	{
	public:
		std::shared_ptr<Transform> transform;
		std::shared_ptr<Model> model;
		std::shared_ptr<Shader> overriding_shader;
		std::shared_ptr<ShaderParameters> overriding_shader_parameters;
	private:
	};
}

