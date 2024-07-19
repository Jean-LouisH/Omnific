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

#include <systems/cpp_scripting_system/cpp_script_instance.hpp>

namespace OmnificEditor
{
	class SplashScreenTransition : public Omnific::CPPScriptInstance
	{
	public:
		/* Custom classes should derive their own type string
		and update the base type name.*/
		SplashScreenTransition()
		{
			this->type = TYPE_STRING;
		};
		static constexpr const char* TYPE_STRING = "SplashScreenTransition";

		/* Custom classes should have their own clone method 
		for the CPPScriptAssembly instance derivation. */
		virtual CPPScriptInstance* instance() override
		{
			SplashScreenTransition* clone = new SplashScreenTransition(*this);
			return clone;
		}

		/* Add all the methods from CPPScript to override. */
		virtual void onStart() override;
		virtual void onLogic() override;

		/* custom public methods. */

	private:
		int countdown_value = 5.0;
		const char* next_scene = "project_list";

		/* custom private methods. */
	};
}