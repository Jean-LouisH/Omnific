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

		/*In progress...*/

		this->image = std::shared_ptr<Image>(new Image("Image::default"));

		if (model.meshes.size() == 1)
		{
			tinygltf::Primitive primitive = model.meshes.at(0).primitives.at(0);
			std::vector<unsigned char> bufferData = model.buffers.at(0).data;
			tinygltf::BufferView positionBufferView = model.bufferViews.at(primitive.attributes.at("POSITION"));
			tinygltf::BufferView texCoord0BufferView = model.bufferViews.at(primitive.attributes.at("TEXCOORD_0"));
			tinygltf::BufferView normalsBufferView = model.bufferViews.at(primitive.attributes.at("NORMAL"));
			tinygltf::BufferView indexBufferView = model.bufferViews.at(primitive.indices);

			std::vector<uint8_t> positionBytes(
				bufferData.begin() + positionBufferView.byteOffset,
				bufferData.begin() + positionBufferView.byteOffset + positionBufferView.byteLength);
			std::vector<uint8_t> textureCoordBytes(
				bufferData.begin() + texCoord0BufferView.byteOffset,
				bufferData.begin() + texCoord0BufferView.byteOffset + texCoord0BufferView.byteLength);
			std::vector<uint8_t> indexBytes(
				bufferData.begin() + indexBufferView.byteOffset,
				bufferData.begin() + indexBufferView.byteOffset + indexBufferView.byteLength);

			std::vector<float> positions;
			std::vector<uint32_t> textureCoords;
			std::vector<uint32_t> indices;
			float* floatPositionByteData = (float*)positionBytes.data();
			float* floatTextureCoordByteData = (float*)textureCoordBytes.data();
			uint16_t* shortIndexByteData = (uint16_t*)indexBytes.data();
			size_t floatPositionByteSize = positionBytes.size() / sizeof(float);
			size_t floatTextureCoordByteSize = textureCoordBytes.size() / sizeof(float);
			size_t shortIndexByteSize = indexBytes.size() / sizeof(uint16_t);

			for (size_t i = 0; i < floatPositionByteSize; i++)
				positions.push_back(floatPositionByteData[i]);

			for (size_t i = 0; i < floatTextureCoordByteSize; i++)
				textureCoords.push_back((uint32_t)floatTextureCoordByteData[i]);

			for (size_t i = 0; i < shortIndexByteSize; i++)
				indices.push_back((uint32_t)shortIndexByteData[i]);

			this->mesh = std::shared_ptr<Mesh>(new Mesh(positions, textureCoords, indices));

			if (model.images.size() > 0)
				;
		}
	}
}