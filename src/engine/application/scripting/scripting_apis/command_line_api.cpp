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
#include <application/scene/component.hpp>

void Esi::CommandLineAPI::openWindow()
{
	Entity panelEntity;
	Entity textEditEntity;

	std::shared_ptr<UIPanel> uiPanel(new UIPanel());
	std::shared_ptr<UITextEdit> uiTextEdit(new UITextEdit());
	std::shared_ptr<Component> textEditComponent = std::static_pointer_cast<Component>(uiTextEdit);
	std::shared_ptr<Component> uiPanelComponent = std::static_pointer_cast<Component>(uiPanel);

	//Fill data
	//...
	//...

	Scene& scene = ScriptingAPIs::getSceneAPI().getScene();

	scene.addEntity(panelEntity);
	scene.addComponentToLastEntity(uiPanelComponent);
	this->entityIDs.push_back(panelEntity.id);

	scene.addEntity(textEditEntity);
	scene.addComponentToLastEntity(textEditComponent);
	this->entityIDs.push_back(textEditEntity.id);
}

void Esi::CommandLineAPI::closeWindow()
{
	Scene& scene = ScriptingAPIs::getSceneAPI().getScene();

	for (int i = 0; i < this->entityIDs.size(); i++)
		scene.removeEntity(this->entityIDs.at(i));

	this->entityIDs.clear();
}