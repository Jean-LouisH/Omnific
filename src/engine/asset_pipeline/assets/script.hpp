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

#include "asset_pipeline/assets/asset.hpp"

// Headers for derived scripts
///////////////////////////////////////////////////////////
#include <script_context.hpp>

#include <asset_pipeline/assets/audio_stream.hpp>
#include <asset_pipeline/assets/font.hpp>
#include <asset_pipeline/assets/image.hpp>
#include <asset_pipeline/assets/material.hpp>
#include <asset_pipeline/assets/mesh.hpp>
#include <asset_pipeline/assets/rig.hpp>
#include <asset_pipeline/assets/shader.hpp>
#include <asset_pipeline/assets/text.hpp>

#include "scene/components/audio_listener.hpp"
#include "scene/components/audio_source.hpp"
#include "scene/components/camera.hpp"
#include "scene/components/collider.hpp"
#include "scene/components/gui.hpp"
#include "scene/components/light.hpp"
#include "scene/components/model_container.hpp"
#include "scene/components/physics_body.hpp"
#include "scene/components/property_animation.hpp"
#include "scene/components/sprite_container.hpp"
#include "scene/components/timer.hpp"
#include "scene/components/transform.hpp"
#include "scene/components/viewport.hpp"

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

        virtual Registerable* clone() override
        {
            return new Script(*this);
        }
    private:
    };
}