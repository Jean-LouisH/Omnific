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


void Lilliputian::SceneAPI::initialize(SceneForest& scene)
{
	this->scene = &scene;
}

void Lilliputian::SceneAPI::bindEntity(SceneTreeID sceneTreeID, EntityID entityID)
{
	this->boundSceneTreeID = sceneTreeID;
	this->boundEntityID = entityID;
}

Lilliputian::UITextLabel* Lilliputian::SceneAPI::getUITextLabel()
{
	UITextLabel* uiTextLabel;

	SceneTree2D sceneTree2D = this->scene->getSceneTree2Ds().at(this->boundSceneTreeID);
	Entity2D entity2D = sceneTree2D.getEntity2D(this->boundEntityID);
	uiTextLabel = sceneTree2D.getComponentVariants().at(entity2D.components.at(ComponentVariant::Type::UI_TEXT_LABEL)).getUITextLabel();

	return uiTextLabel;
}