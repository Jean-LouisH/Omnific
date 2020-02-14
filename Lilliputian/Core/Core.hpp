#pragma once

#include "../Editor/Editor.hpp"
#include <vector>

namespace Lilliputian
{
	using DefinerCallback = void(*)(Lilliputian::Editor*);

	class Core
	{
	private:
		std::vector<DefinerCallback> sceneDefinerCallbacks;
		Editor* editor;

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
		void addSceneDefiner(DefinerCallback sceneInitializerCallback);
	};
}

