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

#include "asset_pipeline/assets/image.hpp"
#include "scene/components/renderable_component.hpp"
#include "scene/components/component.hpp"

#include "asset_pipeline/assets/model.hpp"
#include <memory>
#include <vector>
#include <unordered_map>
#include <string>

namespace Omnia
{
	class OMNIA_ENGINE_API ModelContainer : public RenderableComponent
	{
	public:
		ModelContainer()
		{
			this->type = TYPE_STRING;
		};
		static constexpr const char* TYPE_STRING = "ModelContainer";

		virtual Registerable* copy() override
		{
			return new ModelContainer(*this);
		}
		virtual void deserialize(YAML::Node yamlNode);
		void addModel(std::shared_ptr<Model> model);
		void addCube();
		void addTexturedCube(std::shared_ptr<Image> image);
		void changeToModel(uint64_t index);
		void changeToModel(std::string name);
		std::shared_ptr<Model> getCurrentModel();
	private:
		std::vector<std::shared_ptr<Model>> models;
		std::unordered_map<std::string, size_t> modelIndicesByName;
		uint64_t currentModelIndex = 0;
	};
}