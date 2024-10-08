// // MIT License
// // 
// // Copyright (c) 2020 Jean-Louis Haywood
// // 
// // Permission is hereby granted, free of charge, to any person obtaining a copy
// // of this software and associated documentation files (the "Software"), to deal
// // in the Software without restriction, including without limitation the rights
// // to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// // copies of the Software, and to permit persons to whom the Software is
// // furnished to do so, subject to the following conditions:
// // 
// // The above copyright notice and this permission notice shall be included in all
// // copies or substantial portions of the Software.
// // 
// // THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// // IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// // FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// // AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// // LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// // OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// // SOFTWARE.

// #pragma once

// #include <systems/cpp_scripting_system/cpp_script_instance.hpp>
// #include <unordered_map>
// #include <memory>

// /// Include custom class headers below
// ////////////////////////////////////////////////////

// #include "custom_scripts/splash_screen_transition.hpp"


// /////////////////////////////////////////////////////

// namespace Omnific
// {
// 	class CPPScriptRegistry
// 	{
// 	public:
// 		static void initialize()
// 		{
// 			CPPScriptRegistry* registry = getInstance();

// 			// Add custom scripts here.
// 			////////////////////////////////////////////

// 			registry->add<OmnificEditor::SplashScreenTransition>();


// 			////////////////////////////////////////////
// 		}

// 		static void loadScriptInstances();
// 		static std::unordered_map<std::string, std::shared_ptr<CPPScriptInstance>> getScriptInstances();
// 		static void finalize();

// 		template <class T>
// 		static void add()
// 		{
// 			getInstance()->cppScriptDefinitions.emplace(T::TYPE_STRING, std::static_pointer_cast<CPPScriptInstance>(std::shared_ptr<T>(new T())));
// 		}
// 	private:
// 		static CPPScriptRegistry* instance;
// 		std::unordered_map<std::string, std::shared_ptr<CPPScriptInstance>> cppScriptDefinitions;
// 		std::unordered_map<std::string, std::shared_ptr<CPPScriptInstance>> cppScriptInstances;
// 		static CPPScriptRegistry* getInstance();
// 	};
// }