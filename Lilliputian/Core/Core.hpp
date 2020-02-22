#pragma once

#include <vector>
#include "../Game/Game.hpp"
#include "../Engines/AudioEngine.hpp"
#include "../Engines/RenderingEngine.hpp"

namespace Lilliputian
{
	using SceneDefinerCallback = void(*)(Lilliputian::Editor*);

	class Core
	{
	private:
		std::vector<SceneDefinerCallback> sceneDefinerCallbacks;
		Game* game;
		AudioEngine* audioEngine;
		RenderingEngine* renderingEngine;

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

