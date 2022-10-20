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

#include "scripting_languages/python/python.hpp"
#include "scripting_languages/cpp/cpp.hpp"
#include "core/scene/scene.hpp"
#include <core/singletons/scene_storage.hpp>
#include "core/utilities/aliases.hpp"
#include "scripting_languages/scripting_language.hpp"
#include "core/system.hpp"
#include <memory>
#include <vector>

namespace Omnia
{
	class ScriptingSystem : public System
	{
	public:
		ScriptingSystem()
		{
			this->scriptingLanguages.emplace(Python::TYPE_STRING, std::static_pointer_cast<ScriptingLanguage>(std::shared_ptr<Python>(new Python())));
			this->scriptingLanguages.emplace(CPP::TYPE_STRING, std::static_pointer_cast<ScriptingLanguage>(std::shared_ptr<CPP>(new CPP())));
			this->type = TYPE_STRING;
			this->threadType = ThreadType::UPDATE;
		};

		static constexpr const char* TYPE_STRING = "ScriptingSystem";

		virtual Registerable* instance() override
		{
			return new ScriptingSystem(*this);
		}

		virtual void initialize() override;
		virtual void finalize() override;
		virtual void onStart(std::shared_ptr<Scene> scene) override;
		virtual void onInput(std::shared_ptr<Scene> scene) override;
		virtual void onEarly(std::shared_ptr<Scene> scene);
		virtual void onLogic(std::shared_ptr<Scene> scene) override;
		virtual void onCompute(std::shared_ptr<Scene> scene) override;
		virtual void onLate(std::shared_ptr<Scene> scene) override;
		virtual void onFinish(std::shared_ptr<Scene> scene) override;

		void executeCommand(std::string command);
		void loadScriptModules(std::shared_ptr<Scene> scene);
	private:
		std::unordered_map<std::string, std::shared_ptr<ScriptingLanguage>> scriptingLanguages;
	};
}

