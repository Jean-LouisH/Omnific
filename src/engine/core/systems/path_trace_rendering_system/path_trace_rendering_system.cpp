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

#include "path_trace_rendering_system.hpp"
#include <core/assets/shader.hpp>
#include <core/assets/image.hpp>

#include <core/singletons/os/os.hpp>
#include <core/components/camera.hpp>
#include <core/components/viewport.hpp>
#include <core/components/transform.hpp>

#include <core/singletons/configuration.hpp>

Omnia::PathTraceRenderingSystem::~PathTraceRenderingSystem()
{
	this->finalize();
}

void Omnia::PathTraceRenderingSystem::initialize()
{
	Image image = Image(
		OS::getFileAccess().getDataDirectoryPath() + Configuration::getInstance()->metadata.iconFilepath);
	OS::getWindow().changeIcon(
		image.getData(),
		image.getWidth(),
		image.getHeight(),
		image.getDepth(),
		image.getPitch());

	this->isInitialized = true;
	OS::getLogger().write("Initialized Path Trace Rendering System");
}

void Omnia::PathTraceRenderingSystem::onLate(std::shared_ptr<Scene> scene)
{

}

void Omnia::PathTraceRenderingSystem::finalize()
{
	if (this->isInitialized)
		SDL_QuitSubSystem(SDL_INIT_VIDEO);

	this->isInitialized = false;
}

std::string Omnia::PathTraceRenderingSystem::getRenderingBackendName()
{
	return "";
}