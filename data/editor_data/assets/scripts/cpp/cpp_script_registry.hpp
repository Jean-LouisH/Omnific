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

#include <asset_pipeline/assets/scripts/cpp_script.hpp>
#include <unordered_map>
#include <memory>

/// Include custom class headers below
////////////////////////////////////////////////////

#include "splash_screen_transition.hpp"


/////////////////////////////////////////////////////

namespace Omnia
{
	class CPPScriptRegistry
	{
	public:
		static void initialize()
		{
			CPPScriptRegistry* registry = getInstance();

			// Add custom scripts here.
			////////////////////////////////////////////

			registry->add(new SplashScreenTransition());


			////////////////////////////////////////////
		}

		static void loadScriptInstances();
		static std::unordered_map<std::string, std::shared_ptr<CPPScript>> getScriptInstances();
		static void finalize();

		template <class T>
		static void add(T* t)
		{
			getInstance()->cppScriptDefinitions.emplace(T::TYPE_STRING, std::static_pointer_cast<CPPScript>(std::shared_ptr<T>(t)));
		}
	private:
		static CPPScriptRegistry* instance;
		std::unordered_map<std::string, std::shared_ptr<CPPScript>> cppScriptDefinitions;
		std::unordered_map<std::string, std::shared_ptr<CPPScript>> cppScriptInstances;
		static CPPScriptRegistry* getInstance();
	};
}