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
#include <foundations/transform.hpp>
#include <scene/components/camera.hpp>
#include <scene/components/light.hpp>
#include <memory>
#include "renderable.hpp"

namespace Omnific
{
	/* Caches memory locations for values
	   relevant to rendering from a given Camera. */
	class RenderableLayer
	{
	public:
		bool is_2d = false;
		std::shared_ptr<Camera> camera;
		std::shared_ptr<Transform> camera_transform;
		std::vector<std::shared_ptr<Light>> lights;
		std::vector<std::shared_ptr<Transform>> light_transforms;
		std::vector<Renderable> renderables;
	private:
	};
}

