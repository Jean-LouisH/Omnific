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
	//int thread_count = Platform::get_logical_core_count();
	//ThreadPool* instance = ThreadPool::get_instance();

	//for (int i = 0; i < (thread_count); i++)
	//	instance->threads.push_back(new std::thread(&ThreadPool::run_worker_thread, instance));

	//instance->allowable_thread_count = thread_count;
}

void Omnific::ThreadPool::set_allowable_thread_count(int thread_count)
{
	ThreadPool* instance = ThreadPool::get_instance();
	if (thread_count > 0 && thread_count <= instance->threads.size())
		instance->allowable_thread_count = thread_count;
}

void Omnific::ThreadPool::finalize()
{
	ThreadPool* instance = ThreadPool::get_instance();
	instance->is_finished = true;

	for (std::thread* thread : instance->threads)
	{
		if (thread != nullptr)
		{
			thread->join();
			delete thread;
		}
	}
}

void Omnific::ThreadPool::run_worker_thread()
{
	ThreadPool* instance = ThreadPool::get_instance();
	while (!instance->is_finished)
	{
		/* Placeholder */
		Platform::sleep_this_thread_for(1000);
	}
}

Omnific::ThreadPool* Omnific::ThreadPool::get_instance()
{
	if (instance == nullptr)
		instance = new ThreadPool();
	return instance;
}