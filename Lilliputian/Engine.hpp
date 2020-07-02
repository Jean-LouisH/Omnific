#pragma once

#include "Utilities/Collections/Vector.hpp"
#include "Utilities/String.hpp"
#include "Game/Game.hpp"
#include "Game/ScriptRegistry.hpp"
#include "Engines/AIEngine/AIEngine.hpp"
#include "Engines/AnimationEngine/AnimationEngine.hpp"
#include "Engines/AudioEngine/AudioEngine.hpp"
#include "Engines/HapticEngine/HapticEngine.hpp"
#include "Engines/PhysicsEngine/PhysicsEngine.hpp"
#include "Engines/RenderingEngine/RenderingEngine.hpp"
#include "OSWindow.hpp"
#include "Platform.hpp"
#include "EngineState.hpp"
#include "Profiler.hpp"
#include "FileSystem.hpp"

namespace Lilliputian
{
	using ScriptCompilerCallback = void(*)(Lilliputian::ScriptRegistry*);

	class Engine
	{
	private:
		Vector<ScriptCompilerCallback> scriptCompilerCallbacks;
		Game* game;
		AIEngine* aiEngine;
		AnimationEngine* animationEngine;
		AudioEngine* audioEngine;
		HapticEngine* hapticEngine;
		PhysicsEngine* physicsEngine;
		RenderingEngine* renderingEngine;
		OSWindow* osWindow;
		Platform* platform;
		Profiler* profiler;
		FileSystem* fileSystem;
		uint64_t frameCount;
		uint16_t FPS;
		EngineState state;
		uint32_t msPerComputeUpdate;
		uint32_t targetFPS;
		String gameTitle;
		uint16_t windowHeight;
		uint16_t windowWidth;
		String entrySceneName;
		bool isStartingFullscreen;

		void setGameTitle(const char* gameTitle);
		void setEntryScene(const char* entrySceneName);
		void setWindowDimensions(uint16_t width, uint16_t height);

		void initialize();
		void input();
		void logic();
		void compute();
		void output();
		void sleep();
		void shutdown();
	public:
		Engine(
			int argc,
			char* argv[], 
			const char* gameTitle, 
			uint16_t windowWidth, 
			uint16_t windowHeight, 
			const char* entrySceneName);
		~Engine();

		void run();
		void setMillisecondsPerComputeUpdate(uint32_t msPerComputeUpdate);
		void setTargetFPS(uint32_t targetFPS);
		void setFullscreenStart(bool value);
		void addScriptCompiler(ScriptCompilerCallback scriptCompilerCallback);
	};
}

