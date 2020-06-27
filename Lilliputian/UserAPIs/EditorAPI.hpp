#pragma once

#include "../Utilities/Collections/Vector.hpp"
#include "../Game/Scene.hpp"
#include "../Game/Script.hpp"
#include "../Game/Entity2D.hpp"
#include "ScriptingAPI.hpp"

namespace Lilliputian
{
	class EditorAPI
	{
	private:
		ScriptingAPI* scriptingAPI;
		uint8_t entrySceneIndex;
		Vector<Scene>* scenes;
		uint64_t entityIDCount;
	public:
		EditorAPI(uint8_t entrySceneIndex, Vector<Scene>* scenes);
		Scene createNewScene();
		Script createNewScript();
		Entity2D createNewEntity2D();
		void setEntryScene(Scene scene);
		void addScene(Scene scene);
	};
}

