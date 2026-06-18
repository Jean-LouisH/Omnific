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

#include "pathfinding_system.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include <foundations/singletons/platform/platform.hpp>
#include <scene/components/camera.hpp>
#include <scene/components/renderable.hpp>
#include <foundations/singletons/profiler.hpp>
#include <foundations/singletons/scene_manager.hpp>
#include <scene/components/viewport.hpp>
#include <foundations/singletons/configuration.hpp>
#include <math.h>


#define PATHFINDING_SYSTEM_ON_FIXED_UPDATE_FRAME_TIME_CLOCK_NAME "pathfinding_system_on_fixed_update_frame_time"

Omnific::PathfindingSystem::~PathfindingSystem()
{
	this->finalize();
}

void Omnific::PathfindingSystem::initialize()
{
	this->is_initialized = true;
	Profiler::add_clock(PATHFINDING_SYSTEM_ON_FIXED_UPDATE_FRAME_TIME_CLOCK_NAME, {"pathfinding_system", "on_fixed_update_frame_time"});
	Platform::get_logger().write("Initialized Pathfinding System");
}

void Omnific::PathfindingSystem::on_fixed_update()
{
	std::shared_ptr<Scene> scene = SceneManager::get_active_scene();
	std::shared_ptr<Clock> frame_time_clock = Profiler::get_clock(PATHFINDING_SYSTEM_ON_FIXED_UPDATE_FRAME_TIME_CLOCK_NAME);
	frame_time_clock->set_start();



	frame_time_clock->set_end();
}

void Omnific::PathfindingSystem::finalize()
{
	this->is_initialized = false;
}