#pragma once

#include <vector>
#include "../Editor/Editor.hpp"
#include "../Game/Game.hpp"
#include "../ScriptingAPI/ScriptingAPI.hpp"

namespace Lilliputian
{
	using SceneDefinerCallback = void(*)(Lilliputian::Editor*);

	class Core
	{
	private:
		std::vector<SceneDefinerCallback> sceneDefinerCallbacks;
		Editor* editor;
		Game* game;

		void initialize();
		void shutdown();
		void sleep();
		void benchmark();
		void input();
		void logic();
		void compute();
		void output();
	public:
		Core();
		~Core();

		void run();
		void addSceneDefiner(SceneDefinerCallback sceneInitializerCallback);
	};
}

