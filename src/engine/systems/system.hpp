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
#include <memory>
#include <foundations/registerable.hpp>

namespace Omnific
{
    /* Base class for objects that process Scenes and
    their Component properties for updates or output.*/
    class System : public Registerable
    {
    public:
    	enum class LoopEventIndex
		{
			ON_INPUT,
			ON_ENTITY_START,
			ON_EARLY_UPDATE,
			ON_UPDATE,
			ON_FIXED_UPDATE,
			ON_LATE_UPDATE,
			ON_ENTITY_FINISH,
			ON_OUTPUT
		};

        static constexpr const char* TYPE_STRING = "System";
        System()
        {
            this->type = TYPE_STRING;
            loop_event_indices.emplace("on_input", LoopEventIndex::ON_INPUT);
            loop_event_indices.emplace("on_entity_start", LoopEventIndex::ON_ENTITY_START);
            loop_event_indices.emplace("on_early_update", LoopEventIndex::ON_EARLY_UPDATE);
            loop_event_indices.emplace("on_update", LoopEventIndex::ON_UPDATE);
            loop_event_indices.emplace("on_fixed_update", LoopEventIndex::ON_FIXED_UPDATE);
            loop_event_indices.emplace("on_late_update", LoopEventIndex::ON_LATE_UPDATE);
            loop_event_indices.emplace("on_entity_finish", LoopEventIndex::ON_ENTITY_FINISH);
            loop_event_indices.emplace("on_output", LoopEventIndex::ON_OUTPUT);
        };
        ~System();

        virtual void initialize();
        virtual void on_entity_start();
        virtual void on_input();
        virtual void on_early_update();
        virtual void on_update();
        virtual void on_fixed_update();
        virtual void on_late_update();
        virtual void on_output();
        virtual void on_entity_finish();
        virtual void finalize();
        bool get_is_initialized();

    protected:
    	std::unordered_map<std::string, LoopEventIndex> loop_event_indices;
        bool is_initialized = false;

        bool has_scene_changed();
        bool have_components_changed();
    private:
        SceneID active_scene_id = 0;

    };
}