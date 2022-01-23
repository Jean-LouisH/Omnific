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

#include "application/scene/asset.hpp"
#include "mesh.hpp"
#include "material.hpp"
#include "image.hpp"
#include "skeletal_animation.hpp"
#include "rig.hpp"

#include <string>
#include <memory>
#include <vector>

namespace Esi
{
	class Model : public Asset
	{
	public:
		Model()
		{ 
			this->type = TYPE_STRING;
		};
		Model(std::string filepath);
		static constexpr const char* TYPE_STRING = "Model";

		std::shared_ptr<Mesh> mesh;
		std::shared_ptr<Image> image;
		std::shared_ptr<Material> material;

		std::shared_ptr<Rig> rig;
		std::vector<std::shared_ptr<SkeletalAnimation>> skeletalAnimations;
	private:
	};
}