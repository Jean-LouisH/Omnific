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

#include "scripting.hpp"
#include "scripting_apis.hpp"

Lilliputian::Scripting::Scripting()
{
	this->vm = new VirtualMachine(&this->scripts);
}

void Lilliputian::Scripting::loadCurrentSceneScriptModules()
{
	this->vm->loadCurrentSceneScriptModules();
}

void Lilliputian::Scripting::executeOnStartMethods(Scene& scene)
{
	this->vm->executeOnStartMethods(scene.generateOnStartCallBatches());
}

void Lilliputian::Scripting::executeOnInputMethods(Scene& scene)
{
	this->vm->executeOnInputMethods(scene.generateOnInputCallBatches());
}

void Lilliputian::Scripting::executeOnFrameMethods(Scene& scene)
{
	this->vm->executeOnFrameMethods(scene.generateOnFrameCallBatches());
}

void Lilliputian::Scripting::executeOnComputeMethods(Scene& scene)
{
	this->vm->executeOnComputeMethods(scene.generateOnComputeCallBatches());
}

void Lilliputian::Scripting::executeOnLateMethods(Scene& scene)
{
	this->vm->executeOnLateMethods(scene.generateOnLateCallBatches());
}

void Lilliputian::Scripting::executeOnFinalMethods(Scene& scene)
{
	this->vm->executeOnFinalMethods(scene.generateOnFinalBatches());
}

void Lilliputian::Scripting::bindScene(Scene* scene)
{
	ScriptingAPIs::bindScene(scene);
}

Lilliputian::Vector<Lilliputian::String>* Lilliputian::Scripting::getScripts()
{
	return &this->scripts;
}