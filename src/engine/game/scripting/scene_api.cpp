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
	bool result = false;

	ComponentVariant::Type type = this->convertStringToType(typeString);

	Vector<SceneTree2D>& sceneTree2Ds = this->scene->getSceneTree2Ds();
	for (int i = 0; i < sceneTree2Ds.size(); i++)
		if (sceneTree2Ds.at(i).getID() == this->boundSceneTreeID)
			if (sceneTree2Ds.at(i).getEntity2D(this->boundEntityID).components.count(type) > 0)
				return true;

	return result;
}

void Lilliputian::SceneAPI::changeToScene(String sceneFilename)
{
	if (this->sceneSerializer->doesSceneExist(sceneFilename))
	{
		this->scene->unload();
		*this->scene = this->sceneSerializer->loadFromFile(sceneFilename);
	}
}

void Lilliputian::SceneAPI::setSceneSerializer(SceneSerializer* sceneSerializer)
{
	this->sceneSerializer = sceneSerializer;
}

Lilliputian::Entity2D& Lilliputian::SceneAPI::getThisEntity2D()
{
	return this->getThisSceneTree2D().getEntity2D(this->boundEntityID);
}

Lilliputian::SceneTree2D& Lilliputian::SceneAPI::getThisSceneTree2D()
{
	return this->scene->getSceneTree2Ds().at(this->boundSceneTreeID);
}

Lilliputian::Scene& Lilliputian::SceneAPI::getScene()
{
	return *this->scene;
}

Lilliputian::ComponentVariant& Lilliputian::SceneAPI::getComponentVariant(ComponentVariant::Type type)
{
	ComponentVariant* componentVariant = nullptr;
	Vector<SceneTree2D>& sceneTree2Ds = this->scene->getSceneTree2Ds();
	SceneTree2D* sceneTree2D = nullptr;

	for (int i = 0; i < sceneTree2Ds.size(); i++)
		if (sceneTree2Ds.at(i).getID() == this->boundSceneTreeID)
			sceneTree2D = &sceneTree2Ds.at(i);
	
	Entity2D& entity2D = sceneTree2D->getEntity2D(this->boundEntityID);
	Vector<ComponentVariant>& componentVariants = sceneTree2D->getComponentVariants();

	for (int i = 0; i < componentVariants.size(); i++)
		if (componentVariants.at(i).getID() == entity2D.components.at(type))
			componentVariant = &componentVariants.at(i);

	return *componentVariant;
}

Lilliputian::UITextLabel& Lilliputian::SceneAPI::getUITextLabel()
{
	UITextLabel* uiTextLabel = nullptr;
	String typeString = "ui_text_label";
	Vector<SceneTree2D>& sceneTree2Ds = this->scene->getSceneTree2Ds();

	if (this->hasComponent(typeString))
		for (int i = 0; i < sceneTree2Ds.size(); i++)
			if (sceneTree2Ds.at(i).getID() == this->boundSceneTreeID)
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