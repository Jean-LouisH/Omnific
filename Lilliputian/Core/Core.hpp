#pragma once

#include "../Utilities/GenericCollections/Vector.hpp"
#include "../Game/Game.hpp"
#include "../Engines/AIEngine/AIEngine.hpp"
#include "../Engines/AnimationEngine/AnimationEngine.hpp"
#include "../Engines/AudioEngine/AudioEngine.hpp"
#include "../Engines/HapticEngine/HapticEngine.hpp"
#include "../Engines/PhysicsEngine/PhysicsEngine.hpp"
#include "../Engines/RenderingEngine/RenderingEngine.hpp"

namespace Lilliputian
{
	using SceneDefinerCallback = void(*)(Lilliputian::EditorAPI*);

	class Core
	{
	private:
		Vector<SceneDefinerCallback> sceneDefinerCallbacks;
		Game* game;

		AIEngine* aiEngine;
		AnimationEngine* animationEngine;
		AudioEngine* audioEngine;
		HapticEngine* hapticEngine;
		PhysicsEngine* physicsEngine;
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

