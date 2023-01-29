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
#include <core/singletons/entity_context.hpp>

#include <core/singletons/scene_storage.hpp>

#include <core/assets/audio_stream.hpp>
#include <core/assets/font.hpp>
#include <core/assets/image.hpp>
#include <core/assets/material.hpp>
#include <core/assets/mesh.hpp>
#include <core/assets/rig.hpp>
#include <core/assets/shader.hpp>
#include <core/assets/text.hpp>

#include "core/components/audio_listener.hpp"
#include "core/components/audio_source.hpp"
#include "core/components/camera.hpp"
#include "core/components/collider.hpp"
#include "core/components/gui.hpp"
#include "core/components/light.hpp"
#include "core/components/model.hpp"
#include "core/components/physics_body.hpp"
#include "core/components/property_animation.hpp"
#include "core/components/sprite.hpp"
#include "core/components/timer.hpp"
#include "core/components/transform.hpp"
#include "core/components/viewport.hpp"

///////////////////////////////////////////////////////////

namespace Omnia
{
    class OMNIA_ENGINE_API CPPScriptInstance
    {
    public:
        CPPScriptInstance()
        {
            this->type = TYPE_STRING;
        };
        static constexpr const char* TYPE_STRING = "CPPScriptInstance";

        virtual CPPScriptInstance* instance()
        {
            CPPScriptInstance* clone = new CPPScriptInstance(*this);
            return clone;
        }

        virtual void onStart();
        virtual void onInput();
        virtual void onEarly();
        virtual void onLogic();
        virtual void onCompute();
        virtual void onLate();
        virtual void onFinish();
    protected:
        std::string type;
    private:
    };
}