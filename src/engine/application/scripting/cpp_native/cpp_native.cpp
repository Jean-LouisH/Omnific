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

#include "cpp_native.hpp"
#include "os/os.hpp"

typedef void(ScriptMethod)(void);

Esi::CPPNative::~CPPNative()
{
	OS::getDLLAccess().closeDynamicLibrary(this->dynamicLibraryHandle);
}

void Esi::CPPNative::onModifiedScriptInstance()
{
	this->dynamicLibraryHandle = OS::getDLLAccess().openDynamicLibrary(this->nativeAssemblyFilename);
}

void Esi::CPPNative::executeOnStartMethods()
{
	this->executeMethods("executeOnStartMethods");
}

void Esi::CPPNative::executeOnInputMethods()
{
	this->executeMethods("executeOnInputMethods");
}

void Esi::CPPNative::executeOnFrameMethods()
{
	this->executeMethods("executeOnFrameMethods");
}

void Esi::CPPNative::executeOnComputeMethods()
{
	this->executeMethods("executeOnComputeMethods");
}

void Esi::CPPNative::executeOnOutputMethods()
{
	this->executeMethods("executeOnOutputMethods");
}

void Esi::CPPNative::executeOnFinishMethods()
{
	this->executeMethods("executeOnFinishMethods");
}

void Esi::CPPNative::executeMethods(std::string methodName)
{
	if (this->dynamicLibraryHandle != nullptr)
	{
		ScriptMethod* function = 
			(ScriptMethod*)OS::getDLLAccess().getDyamicLibraryProcedure(this->dynamicLibraryHandle, methodName.c_str());

		if (function != nullptr)
			function();
	}
}