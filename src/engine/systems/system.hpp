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
#include <customization/class_registry/registerable.hpp>

namespace Omnia
{
    enum class ThreadType
    {
        UPDATE,
        OUTPUT
    };

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
        virtual void onStart(std::shared_ptr<Scene> scene);
        virtual void onInput(std::shared_ptr<Scene> scene);
        virtual void onEarly(std::shared_ptr<Scene> scene);
        virtual void onLogic(std::shared_ptr<Scene> scene);
        virtual void onCompute(std::shared_ptr<Scene> scene);
        virtual void onLate(std::shared_ptr<Scene> scene);
        virtual void onFinish(std::shared_ptr<Scene> scene);
        virtual void finalize();
        bool getIsInitialized();

        bool isThreadType(ThreadType threadType);
        ThreadType getThreadType();

    protected:
        ThreadType threadType;
        bool isInitialized = false;
    private:
    };
}