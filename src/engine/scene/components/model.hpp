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

#include "scene/components/renderable_component.hpp"
#include "scene/components/component.hpp"

#include "scene/assets/mesh.hpp"
#include "scene/assets/material.hpp"
#include "scene/assets/skeletal_animation.hpp"
#include "scene/assets/rig.hpp"

//#include <tiny_gltf.h>

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
			this->material = std::shared_ptr<Material>(new Material());
			this->material->albedo = std::shared_ptr<Image>(new Image("Image::default"));
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

/*
namespace Omnia
{
	class OMNIA_ENGINE_API Model : public Asset
	{
	public:
		Model()
		{
			this->type = TYPE_STRING;
		};
		Model(std::string filepath);
		Model(std::string filepath, std::shared_ptr<Image> image);
		static constexpr const char* TYPE_STRING = "Model";

		std::shared_ptr<Mesh> mesh;
		std::shared_ptr<Image> image;
		std::shared_ptr<Material> material;
		std::shared_ptr<Rig> rig;
		std::vector<std::shared_ptr<SkeletalAnimation>> skeletalAnimations;

		virtual Registerable* instance() override
		{
			Model* clone = new Model(*this);
			clone->id = UIDGenerator::getNewUID();
			return clone;
		}
	private:
		void load(std::string filepath, std::shared_ptr<Image> image);
		std::vector<uint8_t> readGLTFBuffer(std::vector<unsigned char> bufferData, tinygltf::BufferView bufferView);
		std::vector<float> readGLTFPrimitiveAttribute(tinygltf::Model model, std::string attributeName);
		std::vector<uint32_t> readGLTFPrimitiveIndices(tinygltf::Model model);
	};
}

*/