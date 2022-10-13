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

#include "core/asset.hpp"

// Headers for derived scripts
///////////////////////////////////////////////////////////
#include <core/singletons/entity_context.hpp>

#include <core/singletons/scene_storage.hpp>

#include <customization/classes/assets/audio_stream.hpp>
#include <customization/classes/assets/font.hpp>
#include <customization/classes/assets/image.hpp>
#include <customization/classes/assets/material.hpp>
#include <customization/classes/assets/mesh.hpp>
#include <customization/classes/assets/rig.hpp>
#include <customization/classes/assets/shader.hpp>
#include <customization/classes/assets/text.hpp>

#include "customization/classes/components/audio_listener.hpp"
#include "customization/classes/components/audio_source.hpp"
#include "customization/classes/components/camera.hpp"
#include "customization/classes/components/collider.hpp"
#include "customization/classes/components/gui.hpp"
#include "customization/classes/components/light.hpp"
#include "customization/classes/components/model.hpp"
#include "customization/classes/components/physics_body.hpp"
#include "customization/classes/components/property_animation.hpp"
#include "customization/classes/components/sprite.hpp"
#include "customization/classes/components/timer.hpp"
#include "customization/classes/components/transform.hpp"
#include "customization/classes/components/viewport.hpp"

///////////////////////////////////////////////////////////

namespace Omnia
{
    class OMNIA_ENGINE_API Script : public Asset
    {
    public:
        Script()
        {
            this->type = TYPE_STRING;
        };
        static constexpr const char* TYPE_STRING = "Script";

        virtual Registerable* instance() override
        {
            Script* clone = new Script(*this);
            clone->id = UIDGenerator::getNewUID();
            return clone;
        }
    private:
    };
}