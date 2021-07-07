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

#include "command_line_api.hpp"
#include "scripting_apis.hpp"
#include <application/scene/component_variant.hpp>

void Lilliputian::CommandLineAPI::openWindow()
{
	Entity panelEntity;
	Entity textEditEntity;

	UIPanel* uiPanel = new UIPanel();
	UITextEdit* uiTextEdit = new UITextEdit();

	//Fill data
	//...
	//...

	Scene& scene = ScriptingAPIs::getSceneAPI().getThisScene();

	ComponentVariant uiPanelComponent;
	scene.addEntity(panelEntity);
	uiPanelComponent.setTo(uiPanel);
	scene.addComponentToLastEntity(uiPanelComponent);
	this->entityIDs.push_back(panelEntity.ID);

	ComponentVariant textEditComponent;
	scene.addEntity(textEditEntity);
	textEditComponent.setTo(uiTextEdit);
	scene.addComponentToLastEntity(textEditComponent);
	this->entityIDs.push_back(textEditEntity.ID);
}

void Lilliputian::CommandLineAPI::closeWindow()
{
	Scene& scene = ScriptingAPIs::getSceneAPI().getThisScene();

	for (int i = 0; i < this->entityIDs.size(); i++)
		scene.removeEntity(this->entityIDs.at(i));

	this->entityIDs.clear();
}