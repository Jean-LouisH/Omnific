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

#pragma once

#include "scene/scene.hpp"
#include "systems/system.hpp"

namespace Omnia
{
	/* Processes Components related to time and classical mechanics physics simulations. */
	class PhysicsSystem : public System
	{
	public:
		PhysicsSystem()
		{
			this->type = TYPE_STRING;
		};

		~PhysicsSystem();
		static constexpr const char* TYPE_STRING = "PhysicsSystem";

		virtual Registerable* instance() override
		{
			return new PhysicsSystem(*this);
		}

		virtual void initialize() override;
		virtual void on_compute(std::shared_ptr<Scene> scene) override;
		virtual void finalize() override;
	private:

		void update_timers(std::shared_ptr<SceneLayer> scene);
		void displace(std::shared_ptr<SceneLayer> scene);
		void gravitate(std::shared_ptr<SceneLayer> scene);
		void decelerate(std::shared_ptr<SceneLayer> scene);
		void detect_collisions(std::shared_ptr<SceneLayer> scene);
		void handle_collisions(std::shared_ptr<SceneLayer> scene);
	};
}

