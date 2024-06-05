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

namespace Omnia
{
    /* Base class for objects that process Scenes and
    their Component properties for updates or output.*/
    class System : public Registerable
    {
    public:
        static constexpr const char* TYPE_STRING = "System";
        System()
        {
            this->type = TYPE_STRING;
        };
        ~System();

        virtual void initialize();
        virtual void on_start(std::shared_ptr<Scene> scene);
        virtual void on_input(std::shared_ptr<Scene> scene);
        virtual void on_early(std::shared_ptr<Scene> scene);
        virtual void on_logic(std::shared_ptr<Scene> scene);
        virtual void on_compute(std::shared_ptr<Scene> scene);
        virtual void on_late(std::shared_ptr<Scene> scene);
        virtual void on_finish(std::shared_ptr<Scene> scene);
        virtual void finalize();
        bool get_is_initialized();

    protected:
        bool is_initialized = false;

        bool has_scene_changed(std::shared_ptr<Scene> scene);
    private:
        SceneID active_scene_id = 0;

    };
}