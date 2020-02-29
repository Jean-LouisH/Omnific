#pragma once

#include "../Utilities/GenericCollections/Vector.hpp"
#include "../Game/Game.hpp"
#include "../Engines/AudioEngine/AudioEngine.hpp"
#include "../Engines/RenderingEngine/RenderingEngine.hpp"

namespace Lilliputian
{
	using SceneDefinerCallback = void(*)(Lilliputian::EditorAPI*);

	class Core
	{
	private:
		Vector<SceneDefinerCallback> sceneDefinerCallbacks;
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

