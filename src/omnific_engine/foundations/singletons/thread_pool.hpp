// MIT License
// 
// Copyright (c) 2020 Jean-Louis Haywood
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

#include <thread>
#include <vector>
#include <string>
#include <stdint.h>
#include <memory>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <engine_api.hpp>

namespace Omnific
{
	class OMNIFIC_ENGINE_API ThreadPool
	{
	public:
		static void initialize();
		static void enqueue_task(std::function<void()> task);
		static void wait_for_all_tasks();
		static void finalize();
	private:
		std::vector<std::thread> threads;
		std::queue<std::function<void()>> tasks;
		std::mutex queue_mutex;
		std::condition_variable task_condition;
		std::condition_variable all_tasks_completed_condition;
		unsigned int busy_thread_count;
		bool is_shutting_down = false;

		static void run_worker_thread();

		static ThreadPool* instance;
		static ThreadPool* get_instance();
	};
}