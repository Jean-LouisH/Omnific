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

#include "shared_library_access.hpp"
#include <core/singletons/os/os.hpp>
#include <sstream>
#include <iomanip>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <dlfcn.h>
#include <errno.h>
#endif

void* Omnia::SharedLibraryAccess::open(std::string filename)
{
#ifdef _WIN32
	return LoadLibraryA((filename + ".dll").c_str());
#else
	return dlopen((filename + ".so").c_str(), RTLD_NOW);
#endif
	OS::getLogger().write("Opened shared library: " + filename);
}

void Omnia::SharedLibraryAccess::close(void* dynamicLibraryHandle)
{
	if (dynamicLibraryHandle != nullptr)
	{
#ifdef _WIN32
		FreeLibrary((HMODULE)dynamicLibraryHandle);
#else
		dlclose(dynamicLibraryHandle);
#endif
		std::stringstream ss;
		ss << std::hex << (uint64_t)dynamicLibraryHandle;
		OS::getLogger().write("Close shared library at address: 0x" + ss.str());
	}
}

void* Omnia::SharedLibraryAccess::getProcedure(void* dynamicLibraryHandle, std::string procedureName)
{
	if (procedureName == "")
	{
		return nullptr;
	}

#ifdef _WIN32
	return GetProcAddress((HMODULE)dynamicLibraryHandle, procedureName.c_str());
#else
	return dlsym(dynamicLibraryHandle, procedureName.c_str());
#endif
}