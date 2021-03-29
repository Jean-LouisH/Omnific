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

#include "utilities/collections/vector.hpp"
#include "utilities/collections/stack.hpp"
#include "utilities/collections/queue.hpp"
#include "utilities/collections/map.hpp"
#include "components/component_variant.hpp"
#include "assets/asset_cache.hpp"
#include "scene_tree_2d.hpp"

namespace Lilliputian
{
	class SceneForest
	{
	public:
		void incrementSceneTree2D();

		void addEntity2D(Entity2D entity2D);
		void addEmptyEntity2D(Entity2D entity2D);
		void addNameToLastEntity(String name);
		void addParentToLastEntityByName(String name);
		void addComponent(EntityID entityID, ComponentVariant componentVariant);
		void addComponentToLastEntity(ComponentVariant componentVariant);

		void executeOnStartMethods();
		void executeOnInputMethods();
		void executeOnFrameMethods();
		void executeOnComputeMethods(uint32_t msPerComputeUpdate);
		void executeOnLateMethods();
		void executeOnFinalMethods();

		EntityID getPreviousEntityID();
		AssetCache& getAssetCache();
		Vector<Stack<SceneTree2D>>& getSceneTree2DStacks();
	private:
		uint64_t entityIDCount = 0;
		Map<String, EntityID> entityNameRegistry;
		Vector<Stack<SceneTree2D>> sceneTree2DStacks;
		AssetCache assetCache;

		Stack<SceneTree2D>* getLastSceneTree2DStack();
	};
}