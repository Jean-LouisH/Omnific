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

#include "command_line.hpp"

Lilliputian::CommandLine::CommandLine(
	Map<String, Script>* scripts,
	Vector<SceneForest>* loadedScenes,
	SceneSerializer* sceneSerializer,
	OS* os,
	Profiler* profiler
)
{
	this->scripts = scripts;
	this->loadedScenes = loadedScenes;
	this->sceneSerializer = sceneSerializer;
	this->os = os;
	this->profiler = profiler;
}

void Lilliputian::CommandLine::execute(String command)
{
	if (command == "shutdown" || command == "exit")
		this->os->getHid().forceShutdownRequest();
}