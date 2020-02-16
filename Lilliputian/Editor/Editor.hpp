#pragma once

#include <vector>
#include "../Game/Scene.hpp"
#include "../Game/Script.hpp"
#include "../Game/Entity2D.hpp"

namespace Lilliputian
{
	class Editor
	{
	private:
		ScriptingAPI* scriptingAPI;
		Scene* mainScene;
		std::vector<Scene>* scenes;
	public:
		Editor(Scene* mainScene2D, std::vector<Scene>* scenes);
		Scene createNewScene();
		Script createNewScript();
		Entity2D createNewEntity2D();
		void setMainScene(Scene scene);
		void addScene(Scene scene);
	};
}

