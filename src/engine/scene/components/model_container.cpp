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

#include "model_container.hpp"
#include <asset_pipeline/asset_pipeline.hpp>
#include <scene_serializer.hpp>

void Omnia::ModelContainer::deserializeProperties(YAML::Node yamlNode)
{
	for (YAML::const_iterator it3 = yamlNode.begin(); it3 != yamlNode.end(); ++it3)
		if (it3->first.as<std::string>() == "models")
			for (int i = 0; i < it3->second.size(); i++)
				if (it3->second[i].as<std::string>() == "Model::cube")
					this->addCube();
				else
					this->addModel(AssetPipeline::load<Model>(SceneSerializer::getDataDirectory() + it3->second[i].as<std::string>()));
}

void Omnia::ModelContainer::addModel(std::shared_ptr<Model> model)
{
	this->models.push_back(model);
	this->currentModelIndex = this->models.size() - 1;
	this->modelIndicesByName.emplace(model->getName(), this->models.size() - 1);
}

void Omnia::ModelContainer::addCube()
{
	std::shared_ptr<Model> cubeModel = std::shared_ptr<Model>(new Model("Model::cube"));
	this->addModel(cubeModel);
}

void Omnia::ModelContainer::addTexturedCube(std::shared_ptr<Image> image)
{
	std::shared_ptr<Model> texturedCubeModel = std::shared_ptr<Model>(new Model("Model::cube", image));
	this->addModel(texturedCubeModel);
}

void Omnia::ModelContainer::changeToModel(uint64_t index)
{
	if (index < this->models.size() && index >= 0)
		this->currentModelIndex = index;
}

void Omnia::ModelContainer::changeToModel(std::string name)
{
	uint64_t index = -1;

	if (this->modelIndicesByName.count(name))
		index = this->modelIndicesByName.at(name);

	this->changeToModel(index);
}

std::shared_ptr<Omnia::Model> Omnia::ModelContainer::getCurrentModel()
{
	std::shared_ptr<Model> model;

	if (this->models.size() > 0)
		model = this->models.at(currentModelIndex);

	return model;
}