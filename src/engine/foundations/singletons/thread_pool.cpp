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

#include "thread_pool.hpp"
#include <foundations/singletons/platform/platform.hpp>
#include <chrono>

Omnific::ThreadPool* Omnific::ThreadPool::instance = nullptr;

void Omnific::ThreadPool::initialize()
{
	int thread_count = Platform::get_logical_core_count() - 1;
	ThreadPool* instance = ThreadPool::get_instance();

	for (int i = 0; i < (thread_count); ++i)
		instance->threads.push_back(std::thread(&ThreadPool::run_worker_thread));

	instance->busy_thread_count = 0;
}

void Omnific::ThreadPool::enqueue_task(std::function<void()> task)
{
	ThreadPool* instance = ThreadPool::get_instance();
	{
		std::unique_lock<std::mutex> lock(instance->queue_mutex);
		instance->tasks.emplace(std::move(task));
	}
	instance->task_condition.notify_one();
}

void Omnific::ThreadPool::wait_for_all_tasks()
{
	ThreadPool* instance = ThreadPool::get_instance();
    std::unique_lock<std::mutex> lock(instance->queue_mutex);
    instance->all_tasks_completed_condition.wait(lock, [instance](){ 
		return instance->tasks.empty() && (instance->busy_thread_count == 0);});
}

void Omnific::ThreadPool::finalize()
{
	ThreadPool* instance = ThreadPool::get_instance();

	{
		std::unique_lock<std::mutex> lock(instance->queue_mutex);
		instance->is_shutting_down = true;
	}

	instance->task_condition.notify_all();

	for (std::thread& thread : instance->threads)
	{
		thread.join();
	}
}

void Omnific::ThreadPool::run_worker_thread()
{
	ThreadPool* instance = ThreadPool::get_instance();
	while (true)
	{
		std::unique_lock<std::mutex> lock(instance->queue_mutex);
		std::function<void()> task;

		instance->task_condition.wait(
			lock, 
			[instance]{return !instance->tasks.empty() || instance->is_shutting_down;});

		if (!instance->tasks.empty())
		{
			instance->busy_thread_count++;
			task = std::move(instance->tasks.front());
			instance->tasks.pop();
			lock.unlock();
			task();
			lock.lock();
			instance->busy_thread_count--;
			instance->all_tasks_completed_condition.notify_one();
		}
		else if (instance->is_shutting_down)
		{
			break;
		}
	}
}

Omnific::ThreadPool* Omnific::ThreadPool::get_instance()
{
	if (instance == nullptr)
		instance = new ThreadPool();
	return instance;
}