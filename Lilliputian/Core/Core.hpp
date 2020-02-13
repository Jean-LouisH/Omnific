#pragma once

namespace Lilliputian
{
	class Core
	{
	private:
		void sleep();
		void benchmark();
		void input();
		void logic();
		void compute();
		void output();
	public:
		Core();
		~Core();
		void initialize();
		void run();
		void shutdown();
	};
}

