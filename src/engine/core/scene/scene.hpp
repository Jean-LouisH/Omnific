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
#include <stack>
#include <memory>
#include <queue>
#include <unordered_map>
#include "core/utilities/rectangle.hpp"
#include <unordered_map>
#include "core/utilities/aliases.hpp"
#include <string>
#include "scene_layer.hpp"
#include <engine_api.hpp>

#include <core/assets/image.hpp>
#include <tiny_gltf.h>

namespace Omnia
{
	class OMNIA_ENGINE_API Scene
	{
	public:
		Scene();
		Scene(std::string filepath);
		Scene(std::string filepath, std::string name);

		void serialize(std::string filepath);
		void deserialize(std::string filepath);
		void deserialize(std::string filepath, std::string name);
		void reload();

		void addSceneLayer(std::shared_ptr<SceneLayer> sceneLayer);
		void addEmptySceneLayer();
		void removeSceneLayer(SceneLayerID sceneLayerID);

		std::string getName();
		std::shared_ptr<SceneLayer> getSceneLayer(SceneLayerID sceneLayer);
		std::shared_ptr<SceneLayer> getSceneLayerByName(std::string name);
		std::shared_ptr<SceneLayer> getLastSceneLayer();
		std::unordered_map<SceneLayerID, std::shared_ptr<SceneLayer>>& getSceneLayers();

		SceneID getID();
	private:
		SceneID id = 0;
		std::string name;
		std::unordered_map<SceneLayerID, std::shared_ptr<SceneLayer>> sceneLayers;
		SceneLayerID lastSceneLayerID = 0;

		std::shared_ptr<SceneLayer> loadGLTF(std::string filepath);
		std::vector<uint8_t> readGLTFBuffer(std::vector<unsigned char> bufferData, tinygltf::BufferView bufferView);
		std::vector<float> readGLTFPrimitiveAttribute(tinygltf::Model model, std::string attributeName, size_t index);
		std::vector<uint32_t> readGLTFPrimitiveIndices(tinygltf::Model model, size_t index);
		std::shared_ptr<Image> readGLTFImage(tinygltf::Model model, int textureIndex);
	};
}