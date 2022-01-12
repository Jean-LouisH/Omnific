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

#include "native.hpp"

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <dlfcn.h>
#include <errno.h>
#endif

typedef int(GetNumberProc)(void);

Esi::Native::~Native()
{
	this->closeDynamicLibrary();
}

void Esi::Native::loadModules()
{
	this->openDynamicLibrary();
}

void Esi::Native::executeOnStartMethods()
{

}

void Esi::Native::executeOnInputMethods()
{

}

void Esi::Native::executeOnFrameMethods()
{
	if (this->dynamicLibraryHandle != nullptr)
	{
		GetNumberProc* function = (GetNumberProc*)this->getDyamicLibraryProcedure("getNumber");
		int test = function();
	}
}

void Esi::Native::executeOnComputeMethods()
{

}

void Esi::Native::executeOnOutputMethods()
{

}

void Esi::Native::executeOnFinishMethods()
{

}

void Esi::Native::openDynamicLibrary()
{
#ifdef _WIN32
	this->dynamicLibraryHandle = LoadLibraryA((this->nativeAssemblyFilename + ".dll").c_str());
#else
	this->dynamicLibraryHandle = dlopen((this->nativeAssemblyFilename + ".so").c_str(), RTLD_NOW);
#endif
}

void Esi::Native::closeDynamicLibrary()
{
	if (this->dynamicLibraryHandle != nullptr)
	{
#ifdef _WIN32
		FreeLibrary((HMODULE)this->dynamicLibraryHandle);
#else
		dlclose(this->dynamicLibraryHandle);
#endif
	}
}

void* Esi::Native::getDyamicLibraryProcedure(std::string procedureName)
{
	if (procedureName == "")
	{
		return nullptr;
	}

#ifdef _WIN32
	return GetProcAddress((HMODULE)this->dynamicLibraryHandle, procedureName.c_str());
#else
	return dlsym(this->dynamicLibraryHandle, procedureName.c_str());
#endif
}