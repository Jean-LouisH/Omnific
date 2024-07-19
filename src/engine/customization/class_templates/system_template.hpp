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

#include <core/system.hpp>

namespace Omnific
{
	/* Example Component class to copy and customize. */
	class SystemTemplate : public System
	{
	public:
		/* Change the TYPE_STRING to the name of the class. */
		static constexpr const char* TYPE_STRING = "SystemTemplate";

		SystemTemplate()
		{
			/* Set the Registerable type to the class TYPE_STRING. */
			this->type = TYPE_STRING;

			/* Set the thread_type to update or output. */
			this->thread_type = ThreadType::UPDATE;
			//this->thread_type = ThreadType::OUTPUT;
		};

		/* Make a copy function specifically for this class. */
		virtual Registerable* instance() override
		{
			SystemTemplate* clone = new SystemTemplate(*this);
			clone->id = UIDGenerator::get_new_uid();
			return clone;
		}

		/* Uncomment the methods needed to override. */

		//virtual void initialize();
		//virtual void on_start(std::shared_ptr<Scene> scene);
		//virtual void on_input(std::shared_ptr<Scene> scene);
		//virtual void on_early(std::shared_ptr<Scene> scene);
		//virtual void on_logic(std::shared_ptr<Scene> scene);
		//virtual void on_compute(std::shared_ptr<Scene> scene);
		//virtual void on_late(std::shared_ptr<Scene> scene);
		//virtual void on_output(std::shared_ptr<Scene> scene);
		//virtual void on_finish(std::shared_ptr<Scene> scene);
		//virtual void finalize();

	private:
	};
}

