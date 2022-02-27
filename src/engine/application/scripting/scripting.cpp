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

Omnific::Scripting::Scripting()
{
	this->pythonVM = std::unique_ptr<PythonVM>(new PythonVM());
	this->cppNative = std::unique_ptr<CPPNative>(new CPPNative());
}

void Omnific::Scripting::onModifiedScriptInstance(Scene scene)
{
	this->pythonVM->onModifiedScriptInstance(scene);
	this->cppNative->onModifiedScriptInstance();
}

void Omnific::Scripting::executeOnStartMethods(Scene& scene)
{
	std::unordered_map<SceneTreeID, SceneTree>& sceneTrees = scene.getSceneTrees();

	for (auto it = sceneTrees.begin(); it != sceneTrees.end(); it++)
		this->pythonVM->executeOnStartMethods(it->second.generateCallBatches(CallType::START));

}

void Omnific::Scripting::executeOnInputMethods(Scene& scene)
{
	std::unordered_map<SceneTreeID, SceneTree>& sceneTrees = scene.getSceneTrees();

	for (auto it = sceneTrees.begin(); it != sceneTrees.end(); it++)
		this->pythonVM->executeOnInputMethods(it->second.generateCallBatches(CallType::UPDATE));
}

void Omnific::Scripting::executeOnFrameMethods(Scene& scene)
{
	std::unordered_map<SceneTreeID, SceneTree>& sceneTrees = scene.getSceneTrees();

	for (auto it = sceneTrees.begin(); it != sceneTrees.end(); it++)
		this->pythonVM->executeOnFrameMethods(it->second.generateCallBatches(CallType::UPDATE));
	this->cppNative->executeOnFrameMethods();
}

void Omnific::Scripting::executeOnComputeMethods(Scene& scene)
{
	std::unordered_map<SceneTreeID, SceneTree>& sceneTrees = scene.getSceneTrees();

	for (auto it = sceneTrees.begin(); it != sceneTrees.end(); it++)
		this->pythonVM->executeOnComputeMethods(it->second.generateCallBatches(CallType::UPDATE));
}

void Omnific::Scripting::executeOnOutputMethods(Scene& scene)
{
	std::unordered_map<SceneTreeID, SceneTree>& sceneTrees = scene.getSceneTrees();

	for (auto it = sceneTrees.begin(); it != sceneTrees.end(); it++)
		this->pythonVM->executeOnOutputMethods(it->second.generateCallBatches(CallType::UPDATE));
}

void Omnific::Scripting::executeOnFinishMethods(Scene& scene)
{
	std::unordered_map<SceneTreeID, SceneTree>& sceneTrees = scene.getSceneTrees();

	for (auto it = sceneTrees.begin(); it != sceneTrees.end(); it++)
		this->pythonVM->executeOnFinishMethods(it->second.generateCallBatches(CallType::FINISH));
}

void Omnific::Scripting::setSceneStorage(SceneStorage* sceneStorage)
{
	ScriptingAPIs::setSceneStorage(sceneStorage);
}