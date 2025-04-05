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

// Headers for derived scripts
///////////////////////////////////////////////////////////
#include <foundations/registerable.hpp>
#include <application_api.hpp>
#include <systems/cpp_scripting_system/cpp_entity_context.hpp>

#include <foundations/singletons/scene_storage.hpp>
#include <scene/entity.hpp>

#include <foundations/resources/audio.hpp>
#include <foundations/resources/font.hpp>
#include <foundations/resources/image.hpp>
#include <foundations/resources/mesh.hpp>
#include <foundations/resources/script.hpp>
#include <foundations/resources/shader.hpp>
#include <foundations/resources/text.hpp>

#include "scene/components/audio_listener.hpp"
#include "scene/components/audio_source.hpp"
#include "scene/components/camera.hpp"
#include "scene/components/collider.hpp"
#include "scene/components/gui.hpp"
#include "scene/components/light.hpp"
#include "scene/components/model.hpp"
#include "scene/components/physics_body.hpp"
#include "scene/components/animator.hpp"
#include "scene/components/script_collection.hpp"
#include "scene/components/sprite.hpp"
#include "scene/components/timer.hpp"
#include "scene/components/viewport.hpp"

///////////////////////////////////////////////////////////

namespace Omnific
{
    class OMNIFIC_ENGINE_API CPPScriptInstance : public Registerable
    {
    public:
        CPPScriptInstance()
        {
            this->type = TYPE_STRING;
        };
        static constexpr const char* TYPE_STRING = "CPPScriptInstance";

        virtual Registerable* instance() override
		{
			CPPScriptInstance* clone = new CPPScriptInstance(*this);
			clone->id = UIDGenerator::get_new_uid();
			return clone;
		}

        virtual void on_input();
        virtual void on_entity_start();
        virtual void on_early_update();
        virtual void on_update();
        virtual void on_fixed_update();
        virtual void on_late_update();
        virtual void on_entity_finish();
        virtual void on_output();
    private:
    };
}