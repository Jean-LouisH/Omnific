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
#include <core/singletons/os/os.hpp>
#include <chrono>

void Omnia::ThreadPool::initialize()
{
	int thread_count = OS::get_platform().get_logical_core_count();

	for (int i = 0; i < (thread_count); i++)
		this->threads.push_back(new std::thread(&ThreadPool::run_worker_thread, this));

	this->allowable_thread_count = thread_count;
}

void Omnia::ThreadPool::set_allowable_thread_count(int thread_count)
{
	if (thread_count > 0 && thread_count <= this->threads.size())
		this->allowable_thread_count = thread_count;
}

void Omnia::ThreadPool::finalize()
{
	this->is_finished = true;

	for (std::thread* thread : this->threads)
	{
		if (thread != nullptr)
		{
			thread->join();
			delete thread;
		}
	}
}

void Omnia::ThreadPool::run_worker_thread()
{
	while (!this->is_finished)
	{
		/* Placeholder */
		OS::sleep_this_thread_for(1000);
	}
}