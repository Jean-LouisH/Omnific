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

#include "scene_api.hpp"


void Lilliputian::SceneAPI::bindScene(Scene* scene)
{
	this->scene = scene;
}

void Lilliputian::SceneAPI::bindEntity(SceneTreeID sceneTreeID, EntityID entityID)
{
	this->boundSceneTreeID = sceneTreeID;
	this->boundEntityID = entityID;
}

bool Lilliputian::SceneAPI::hasComponent(String typeString)
{
	ComponentVariant::Type type = this->convertStringToType(typeString);

	SceneTree2D& sceneTree2D = this->scene->getSceneTree2Ds().at(this->boundSceneTreeID);
	return sceneTree2D.getEntity2D(this->boundEntityID).components.count(type) > 0;
}

void Lilliputian::SceneAPI::changeToScene(String sceneFilename)
{
	if (this->sceneSerializer->doesSceneExist(sceneFilename))
	{
		this->scene->cleanup();
		*this->scene = this->sceneSerializer->loadFromFile(sceneFilename);
	}
}

void Lilliputian::SceneAPI::setSceneSerializer(SceneSerializer* sceneSerializer)
{
	this->sceneSerializer = sceneSerializer;
}

Lilliputian::ComponentVariant& Lilliputian::SceneAPI::getComponentVariant(ComponentVariant::Type type)
{
	SceneTree2D& sceneTree2D = this->scene->getSceneTree2Ds().at(this->boundSceneTreeID);
	Entity2D& entity2D = sceneTree2D.getEntity2D(this->boundEntityID);
	return sceneTree2D.getComponentVariants().at(entity2D.components.at(type));
}

Lilliputian::UITextLabel& Lilliputian::SceneAPI::getUITextLabel()
{
	UITextLabel* uiTextLabel = nullptr;
	String typeString = "ui_text_label";

	if (this->hasComponent(typeString))
		uiTextLabel = this->getComponentVariant(this->convertStringToType(typeString)).getUITextLabel();

	return *uiTextLabel;
}

Lilliputian::ComponentVariant::Type Lilliputian::SceneAPI::convertStringToType(String typeString)
{
	ComponentVariant::Type type = ComponentVariant::Type::NONE;

	if (typeString == "ui_text_label")
		type = ComponentVariant::Type::UI_TEXT_LABEL;

	return type;
}