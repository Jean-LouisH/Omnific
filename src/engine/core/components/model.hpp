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

#include "renderable_component.hpp"
#include "core/component.hpp"

#include "core/assets/mesh.hpp"
#include "core/assets/material.hpp"
#include "core/assets/skeletal_animation.hpp"
#include "core/assets/rig.hpp"

#include <memory>
#include <vector>
#include <unordered_map>
#include <string>

namespace Omnia
{
	class OMNIA_ENGINE_API Model : public RenderableComponent
	{
	public:
		Model()
		{
			this->type = TYPE_STRING;
		};
		static constexpr const char* TYPE_STRING = "Model";

		virtual Registerable* instance() override
		{
			Model* clone = new Model(*this);
			clone->id = UIDGenerator::getNewUID();
			return clone;
		}
		virtual void deserialize(YAML::Node yamlNode);
		void setToCube();
		void setToTexturedCube(std::shared_ptr<Material> material);

		void setMesh(std::shared_ptr<Mesh> mesh);
		void setMaterial(std::shared_ptr<Material> material);
		void setRig(std::shared_ptr<Rig> rig);
		void addSkeletalAnimation(std::shared_ptr<SkeletalAnimation> skeletalAnimation);

		std::shared_ptr<Mesh> getMesh();
		std::shared_ptr<Material> getMaterial();
		std::shared_ptr<Rig> getRig();
		std::vector<std::shared_ptr<SkeletalAnimation>> getSkeletalAnimations();
	private:
		std::shared_ptr<Mesh> mesh;
		std::shared_ptr<Material> material;
		std::shared_ptr<Rig> rig;
		std::vector<std::shared_ptr<SkeletalAnimation>> skeletalAnimations;
	};
}