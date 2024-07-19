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
#include <map>
#include <unordered_map>
#include <foundations/aliases.hpp>
#include <string>
#include "scene_layer.hpp"
#include <engine_api.hpp>

#include <foundations/resources/image.hpp>
#include <tiny_gltf.h>

namespace Omnific
{
	class OMNIFIC_ENGINE_API Scene
	{
	public:
		Scene();
		Scene(std::string filepath);
		Scene(std::string filepath, std::string name);

		void serialize_to(std::string filepath);
		void deserialize_from(std::string filepath);
		void deserialize_from(std::string filepath, std::string name);
		void reload();

		void add_scene_layer(std::shared_ptr<SceneLayer> scene_layer);
		void add_empty_scene_layer();
		void remove_scene_layer(SceneLayerID scene_layer_id);

		std::string get_name();
		std::shared_ptr<SceneLayer> get_scene_layer(SceneLayerID scene_layer);
		std::shared_ptr<SceneLayer> get_scene_layer_by_name(std::string name);
		std::shared_ptr<SceneLayer> get_last_scene_layer();
		std::map<SceneLayerID, std::shared_ptr<SceneLayer>>& get_scene_layers();

		SceneID get_id();
	private:
		SceneID id = 0;
		std::string name;
		std::map<SceneLayerID, std::shared_ptr<SceneLayer>> scene_layers;
		SceneLayerID last_scene_layer_id = 0;

		std::shared_ptr<SceneLayer> load_gltf(std::string filepath);
		std::vector<uint8_t> read_gltfbuffer(std::vector<unsigned char> buffer_data, tinygltf::BufferView buffer_view);
		std::vector<float> read_gltfprimitive_attribute(tinygltf::Model model, std::string attribute_name, size_t index);
		std::vector<uint32_t> read_gltfprimitive_indices(tinygltf::Model model, size_t index);
		std::shared_ptr<Image> read_gltfimage(tinygltf::Model model, int texture_index);
	};
}