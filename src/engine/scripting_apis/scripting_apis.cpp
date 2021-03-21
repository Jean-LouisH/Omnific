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

#include "scripting_apis.hpp"

Lilliputian::ScriptingAPIs::ScriptingAPIs()
{
	this->fileAPI = new FileAPI();
	this->inputAPI = new InputAPI();
	this->logAPI = new LogAPI();
	this->renderAPI = new RenderAPI();
	this->sceneAPI = new SceneAPI();
	this->threadAPI = new ThreadAPI();
	this->timeAPI = new TimeAPI();
	this->windowAPI = new WindowAPI();
}

Lilliputian::FileAPI& Lilliputian::ScriptingAPIs::file() const
{
	return *this->fileAPI;
}

Lilliputian::InputAPI& Lilliputian::ScriptingAPIs::input() const
{
	return *this->inputAPI;
}

Lilliputian::LogAPI& Lilliputian::ScriptingAPIs::log() const
{
	return *this->logAPI;
}

Lilliputian::RenderAPI& Lilliputian::ScriptingAPIs::render() const
{
	return *this->renderAPI;
}

Lilliputian::SceneAPI& Lilliputian::ScriptingAPIs::scene() const
{
	return *this->sceneAPI;
}

Lilliputian::ThreadAPI& Lilliputian::ScriptingAPIs::thread() const
{
	return *this->threadAPI;
}

Lilliputian::TimeAPI& Lilliputian::ScriptingAPIs::time() const
{
	return *this->timeAPI;
}

Lilliputian::WindowAPI& Lilliputian::ScriptingAPIs::window() const
{
	return *this->windowAPI;
}