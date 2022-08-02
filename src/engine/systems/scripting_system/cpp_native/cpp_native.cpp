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

typedef void(ScriptProcedure)(void);

Omnia::CPPNative::~CPPNative()
{
	OS::getDLLAccess().close(this->dynamicLibraryHandle);
}

void Omnia::CPPNative::initialize()
{

}

void Omnia::CPPNative::loadScriptInstances()
{
	this->dynamicLibraryHandle = OS::getDLLAccess().open(this->nativeAssemblyFilename);
	this->execute("loadScriptInstances");
}

void Omnia::CPPNative::executeOnStartMethods()
{
	this->execute("executeOnStartMethods");
}

void Omnia::CPPNative::executeOnInputMethods()
{
	this->execute("executeOnInputMethods");
}

void Omnia::CPPNative::executeOnLogicFrameMethods()
{
	this->execute("executeOnLogicFrameMethods");
}

void Omnia::CPPNative::executeOnComputeFrameMethods()
{
	this->execute("executeOnComputeFrameMethods");
}

void Omnia::CPPNative::executeOnOutputMethods()
{
	this->execute("executeOnOutputMethods");
}

void Omnia::CPPNative::executeOnFinishMethods()
{
	this->execute("executeOnFinishMethods");
}

void Omnia::CPPNative::execute(std::string methodName)
{
	if (this->dynamicLibraryHandle != nullptr)
	{
		ScriptProcedure* procedure = 
			(ScriptProcedure*)OS::getDLLAccess().getProcedure(this->dynamicLibraryHandle, methodName.c_str());

		if (procedure != nullptr)
			procedure();
	}
}

void Omnia::CPPNative::deinitialize()
{

}