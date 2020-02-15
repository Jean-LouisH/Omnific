#pragma once

#include <vector>
#include "../Game/Scene2D.hpp"
#include "../Game/Script.hpp"
#include "../Game/Entity2D.hpp"

namespace Lilliputian
{
	class Editor
	{
	private:
		ScriptingAPI* scriptingAPI;
		Scene2D mainScene2D;
		std::vector<Scene2D> scene2Ds;
	public:
		Editor();
		Scene2D createNewScene2D();
		Script createNewScript();
		Entity2D createNewEntity2D();
		void setMainScene2D(Scene2D scene2D);
	};
}

