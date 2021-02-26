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
		void addComponent(EntityID entityID, ComponentVariant componentVariant);
		void addComponentToLastEntity(ComponentVariant componentVariant);

		EntityID getPreviousEntityID();

		void executeFrameLogic();
	private:
		uint64_t entityIDCount = 0;
		Map<String, EntityID> entityNameRegistry;
		Vector<Stack<SceneTree2D>> sceneTree2DStacks;
		AssetCache assetCache;

		Stack<SceneTree2D>* getLastSceneTree2DStack();
	};
}