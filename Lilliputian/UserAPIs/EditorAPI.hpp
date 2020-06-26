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
		Scene* mainScene;
		Vector<Scene>* scenes;
		uint64_t entityIDCount;
	public:
		EditorAPI(Scene* mainScene2D, Vector<Scene>* scenes);
		Scene createNewScene();
		Script createNewScript();
		Entity2D createNewEntity2D();
		void setMainScene(Scene scene);
		void addScene(Scene scene);
	};
}

