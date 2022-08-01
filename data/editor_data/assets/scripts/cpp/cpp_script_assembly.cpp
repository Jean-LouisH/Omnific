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

#include "cpp_script_assembly.hpp"
#include "cpp_script_registry.hpp"
#include "systems/scripting_system/script_context.hpp"
#include "os/os.hpp"

void loadScriptInstances()
{
	Omnia::OS::getLogger().write("DLL Loaded.");
	Omnia::CPPScriptRegistry::loadScriptInstances();
	
	int a = 0;
	int b = a + 2;
}

void executeOnStartMethods()
{

}

void executeOnInputMethods()
{

}

void executeOnLogicFrameMethods()
{

}

void executeOnComputeFrameMethods()
{

}

void executeOnOutputMethods()
{

}

void executeOnFinishMethods()
{

}