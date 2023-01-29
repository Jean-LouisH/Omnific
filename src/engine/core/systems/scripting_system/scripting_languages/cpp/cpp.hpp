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

#include <string>
#include "core/systems/scripting_system/scripting_languages/scripting_language.hpp"

namespace Omnia
{
    class CPP : public ScriptingLanguage
    {
    public:
        static constexpr const char* TYPE_STRING = "CPP";
        CPP()
        {
            this->type = TYPE_STRING;
        };
        ~CPP();
        virtual void initialize() override;
        virtual void loadScriptInstances() override;
        virtual void onStart() override;
        virtual void onInput() override;
        virtual void onEarly() override;
        virtual void onLogic() override;
        virtual void onCompute() override;
        virtual void onLate() override;
        virtual void onFinish() override;
        virtual void finalize() override;

    private:
        void* dynamicLibraryHandle;
        std::string nativeAssemblyFilename = "omnia_cpp_script_assembly";

        void execute(std::string methodName);
    };
}