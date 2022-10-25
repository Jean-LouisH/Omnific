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

#include "model.hpp"
#include <core/scene/scene.hpp>
#include <core/singletons/os/os.hpp>


void Omnia::Model::deserialize(YAML::Node yamlNode)
{
	this->material = std::shared_ptr<Material>(new Material());

	for (YAML::const_iterator it3 = yamlNode.begin(); it3 != yamlNode.end(); ++it3)
	{
		if (it3->first.as<std::string>() == "mesh")
		{
			this->mesh = std::shared_ptr<Mesh>(new Mesh(it3->second.as<std::string>()));
		}
		else if (it3->first.as<std::string>() == "albedo")
		{
			this->material->albedo = std::shared_ptr<Image>(new Image(it3->second.as<std::string>()));
		}
	}

	if (this->mesh == nullptr)
	{
		this->mesh = std::shared_ptr<Mesh>(new Mesh("Mesh::cube"));
	}

	if (this->material->albedo == nullptr)
	{
		this->material->albedo = std::shared_ptr<Image>(new Image("Image::default"));
	}
}

void Omnia::Model::setToCube()
{
	this->mesh = std::shared_ptr<Mesh>(new Mesh("Mesh::cube"));
	this->material = std::shared_ptr<Material>(new Material());
	this->material->albedo = std::shared_ptr<Image>(new Image("Image::default"));
}

void Omnia::Model::setToTexturedCube(std::shared_ptr<Material> material)
{
	this->mesh = std::shared_ptr<Mesh>(new Mesh("Mesh::cube"));
	this->material = material;
}

void Omnia::Model::setMesh(std::shared_ptr<Mesh> mesh)
{
	this->mesh = mesh;
}

void Omnia::Model::setMaterial(std::shared_ptr<Material> material)
{
	this->material = material;
}

void Omnia::Model::setRig(std::shared_ptr<Rig> rig)
{
	this->rig = rig;
}

void Omnia::Model::addSkeletalAnimation(std::shared_ptr<SkeletalAnimation> skeletalAnimation)
{
	this->skeletalAnimations.push_back(skeletalAnimation);
}

std::shared_ptr<Omnia::Mesh> Omnia::Model::getMesh()
{
	return this->mesh;
}

std::shared_ptr<Omnia::Material> Omnia::Model::getMaterial()
{
	return this->material;
}

std::shared_ptr<Omnia::Rig> Omnia::Model::getRig()
{
	return this->rig;
}

std::vector<std::shared_ptr<Omnia::SkeletalAnimation>> Omnia::Model::getSkeletalAnimations()
{
	return this->skeletalAnimations;
}

/*

#include "model.hpp"
#include <os/os.hpp>

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <tiny_gltf.h>

Omnia::Model::Model(std::string filepath)
{
	this->load(filepath, std::shared_ptr<Image>(new Image("Image::default")));
}

Omnia::Model::Model(std::string filepath, std::shared_ptr<Image> image)
{
	this->load(filepath, image);
}

void Omnia::Model::load(std::string filepath, std::shared_ptr<Image> image)
{
	if (filepath == "Model::cube")
	{
		this->mesh = std::shared_ptr<Mesh>(new Mesh("Mesh::cube"));
		this->image = image;
	}
	else
	{
		tinygltf::Model model;
		tinygltf::TinyGLTF tinyGLTF;
		std::string err;
		std::string warn;

		//bool ret = tinyGLTF.LoadASCIIFromFile(&model, &err, &warn, filepath);
		bool ret = tinyGLTF.LoadBinaryFromFile(&model, &err, &warn, filepath);

		if (!warn.empty()) {
			printf("Warn: %s\n", warn.c_str());
		}

		if (!err.empty()) {
			printf("Err: %s\n", err.c_str());
		}

		if (!ret) {
			printf("Failed to parse glTF\n");
		}

this->image = std::shared_ptr<Image>(new Image("Image::default"));

if (model.meshes.size() == 1)
{
	std::vector<float> positions = this->readGLTFPrimitiveAttribute(model, "POSITION");
	std::vector<float> textureCoords = this->readGLTFPrimitiveAttribute(model, "TEXCOORD_0");
	std::vector<float> normals = this->readGLTFPrimitiveAttribute(model, "NORMAL");
	std::vector<uint32_t> indices = this->readGLTFPrimitiveIndices(model);

	this->mesh = std::shared_ptr<Mesh>(new Mesh(positions, textureCoords, normals, indices));

	if (model.images.size() == 1)
	{
		tinygltf::Image image = model.images.at(0);
		this->image = std::shared_ptr<Image>(
			new Image((uint8_t*)image.image.data(), image.width, image.height, image.component));
	}
}
	}
}

std::vector<uint8_t> Omnia::Model::readGLTFBuffer(std::vector<unsigned char> bufferData, tinygltf::BufferView bufferView)
{
	std::vector<uint8_t> bytes(
		bufferData.begin() + bufferView.byteOffset,
		bufferData.begin() + bufferView.byteOffset + bufferView.byteLength);

	return bytes;
}

std::vector<float> Omnia::Model::readGLTFPrimitiveAttribute(tinygltf::Model model, std::string attributeName)
{
	tinygltf::Primitive primitive = model.meshes.at(0).primitives.at(0);
	std::vector<unsigned char> buffer = model.buffers.at(0).data;
	std::vector<uint8_t> bytes = this->readGLTFBuffer(buffer, model.bufferViews.at(primitive.attributes.at(attributeName)));
	std::vector<float> attribute;
	float* floatByteData = (float*)bytes.data();
	size_t floatByteSize = bytes.size() / sizeof(float);

	for (size_t i = 0; i < floatByteSize; i++)
		attribute.push_back(floatByteData[i]);

	return attribute;

}

std::vector<uint32_t> Omnia::Model::readGLTFPrimitiveIndices(tinygltf::Model model)
{
	tinygltf::Primitive primitive = model.meshes.at(0).primitives.at(0);
	std::vector<unsigned char> buffer = model.buffers.at(0).data;
	std::vector<uint8_t> indexBytes = this->readGLTFBuffer(buffer, model.bufferViews.at(primitive.indices));
	std::vector<uint32_t> indices;

	uint16_t* shortIndexByteData = (uint16_t*)indexBytes.data();
	size_t shortIndexByteSize = indexBytes.size() / sizeof(uint16_t);

	for (size_t i = 0; i < shortIndexByteSize; i++)
		indices.push_back((uint32_t)shortIndexByteData[i]);

	return indices;
}
*/