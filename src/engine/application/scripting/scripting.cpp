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
#include "scripting_apis/scripting_apis.hpp"

Esi::Scripting::Scripting()
{
	this->pythonVM = std::unique_ptr<PythonVM>(new PythonVM());
	this->cppNative = std::unique_ptr<CPPNative>(new CPPNative());
}

void Esi::Scripting::onModifiedScriptInstance(Scene scene)
{
	this->pythonVM->onModifiedScriptInstance(scene);
	this->cppNative->onModifiedScriptInstance();
}

void Esi::Scripting::executeOnStartMethods(Scene& scene)
{
	this->pythonVM->executeOnStartMethods(scene.generateCallBatches(CallType::START));
}

void Esi::Scripting::executeOnInputMethods(Scene& scene)
{
	this->pythonVM->executeOnInputMethods(scene.generateCallBatches(CallType::UPDATE));
}

void Esi::Scripting::executeOnFrameMethods(Scene& scene)
{
	this->pythonVM->executeOnFrameMethods(scene.generateCallBatches(CallType::UPDATE));
	this->cppNative->executeOnFrameMethods();
}

void Esi::Scripting::executeOnComputeMethods(Scene& scene)
{
	this->pythonVM->executeOnComputeMethods(scene.generateCallBatches(CallType::UPDATE));
}

void Esi::Scripting::executeOnOutputMethods(Scene& scene)
{
	this->pythonVM->executeOnOutputMethods(scene.generateCallBatches(CallType::UPDATE));
}

void Esi::Scripting::executeOnFinishMethods(Scene& scene)
{
	this->pythonVM->executeOnFinishMethods(scene.generateCallBatches(CallType::FINISH));
}

void Esi::Scripting::setSceneStorage(SceneStorage* sceneStorage)
{
	ScriptingAPIs::setSceneStorage(sceneStorage);
}