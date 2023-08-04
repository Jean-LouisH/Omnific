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

#include "core/system.hpp"

Omnia::System::~System()
{
	this->isInitialized = false;
}

void Omnia::System::initialize()
{

}

void Omnia::System::initializeOutput()
{

}

void Omnia::System::onStart(std::shared_ptr<Scene> scene)
{

}

void Omnia::System::onInput(std::shared_ptr<Scene> scene)
{

}

void Omnia::System::onEarly(std::shared_ptr<Scene> scene)
{

}

void Omnia::System::onLogic(std::shared_ptr<Scene> scene)
{

}

void Omnia::System::onCompute(std::shared_ptr<Scene> scene)
{

}

void Omnia::System::onLate(std::shared_ptr<Scene> scene)
{

}

void Omnia::System::onOutput(std::shared_ptr<Scene> scene)
{

}

void Omnia::System::onFinish(std::shared_ptr<Scene> scene)
{

}

void Omnia::System::finalizeOutput()
{

}

void Omnia::System::finalize()
{

}

bool Omnia::System::getIsInitialized()
{
	return this->isInitialized;
}

bool Omnia::System::getIsOutputInitialized()
{
	return this->isOutputInitialized;
}

bool Omnia::System::hasSceneChanged(std::shared_ptr<Scene> scene)
{
	bool activeSceneChanged = this->activeSceneID != scene->getID();

	if (activeSceneChanged)
		this->activeSceneID = scene->getID();

	return (scene->queryEventCount(OMNIA_EVENT_COMPONENT_ADDED) ||
		scene->queryEventCount(OMNIA_EVENT_COMPONENT_REMOVED) ||
		activeSceneChanged);
}

bool Omnia::System::hasSceneChangedForOutput(std::shared_ptr<Scene> scene)
{
	bool activeSceneChanged = this->activeSceneID != scene->getID();

	if (activeSceneChanged)
		this->activeSceneID = scene->getID();

	return (scene->queryOutputEventCount(OMNIA_EVENT_COMPONENT_ADDED) ||
		scene->queryOutputEventCount(OMNIA_EVENT_COMPONENT_REMOVED) ||
		activeSceneChanged);
}