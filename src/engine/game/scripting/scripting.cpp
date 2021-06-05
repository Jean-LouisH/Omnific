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
	this->vm = new VirtualMachine();
}

void Lilliputian::Scripting::loadCurrentSceneScriptModules(Scene scene)
{
	this->vm->loadCurrentSceneScriptModules(scene);
}

void Lilliputian::Scripting::executeOnStartMethods(Scene& scene)
{
	this->vm->executeOnStartMethods(scene.getAllOnStartCallBatches());
}

void Lilliputian::Scripting::executeOnInputMethods(Scene& scene)
{
	this->vm->executeOnInputMethods(scene.getAllOnInputCallBatches());
}

void Lilliputian::Scripting::executeOnFrameMethods(Scene& scene)
{
	this->vm->executeOnFrameMethods(scene.getAllOnFrameCallBatches());
}

void Lilliputian::Scripting::executeOnComputeMethods(Scene& scene)
{
	this->vm->executeOnComputeMethods(scene.getAllOnComputeCallBatches());
}

void Lilliputian::Scripting::executeOnLateMethods(Scene& scene)
{
	this->vm->executeOnLateMethods(scene.getAllOnLateCallBatches());
}

void Lilliputian::Scripting::executeOnFinalMethods(Scene& scene)
{
	this->vm->executeOnFinalMethods(scene.getAllOnFinalBatches());
}

void Lilliputian::Scripting::setSceneStorage(SceneStorage* sceneStorage)
{
	ScriptingAPIs::setSceneStorage(sceneStorage);
}