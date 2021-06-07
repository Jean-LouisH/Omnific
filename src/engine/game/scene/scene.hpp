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
#include "game/scripting/virtual_machine/script_call_batch.hpp"
#include "asset_cache.hpp"
#include "scene_tree_2d.hpp"

namespace Lilliputian
{
	class Scene
	{
	public:
		Scene();

		void addSceneTree2D(SceneTree2D sceneTree2D);
		void removeSceneTree2D(SceneTreeID sceneTreeID);

		Vector<ScriptCallBatch> getAllOnStartCallBatches();
		Vector<ScriptCallBatch> getAllOnInputCallBatches();
		Vector<ScriptCallBatch> getAllOnFrameCallBatches();
		Vector<ScriptCallBatch> getAllOnComputeCallBatches();
		Vector<ScriptCallBatch> getAllOnLateCallBatches();
		Vector<ScriptCallBatch> getAllOnFinalBatches();

		AssetCache& getAssetCache();
		SceneTree2D& getSceneTree(SceneTreeID sceneTreeID);
		SceneTree2D& getLastSceneTree2D();
		Vector<SceneTree2D>& getSceneTree2Ds();

		void unload();
	private:
		/*SceneTrees are stored in vectors for sequential access
		in engine systems.*/
		Vector<SceneTree2D> sceneTree2Ds;
		AssetCache assetCache;
	};
}