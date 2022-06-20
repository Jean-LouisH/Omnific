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
#include "utilities/rectangle.hpp"
#include <unordered_map>
#include "utilities/aliases.hpp"
#include <string>
#include "scene_tree.hpp"

namespace Omnia
{
	class Scene
	{
		friend class UpdateSystem;
		friend class SceneSerializer;
	public:
		Scene();

		std::shared_ptr<SceneTree> getSceneTree(SceneTreeID sceneTree);
		std::shared_ptr<SceneTree> getSceneTreeByName(std::string name);
		std::shared_ptr<SceneTree> getLastSceneTree();
		std::unordered_map<SceneTreeID, std::shared_ptr<SceneTree>>& getSceneTrees();

		SceneID getID();
	private:
		SceneID id = 0;
		std::unordered_map<SceneTreeID, std::shared_ptr<SceneTree>> sceneTrees;
		SceneTreeID lastSceneTreeID = 0;

		void addSceneTree(std::shared_ptr<SceneTree> sceneTree);
		void addEmptySceneTree();
		void removeSceneTree(SceneTreeID sceneTreeID);
	};
}