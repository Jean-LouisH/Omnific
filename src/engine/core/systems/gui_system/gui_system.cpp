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

#include "gui_system.hpp"
#include "core/singletons/os/os.hpp"
#include <core/singletons/event_bus.hpp>
#include <SDL_ttf.h>
#include <core/components/gui.hpp>
#include <core/components/transform.hpp>
#include <core/components/viewport.hpp>

Omnia::GUISystem::~GUISystem()
{
	this->finalize();
}

void Omnia::GUISystem::initialize()
{
	this->isInitialized = true;
	OS::getLogger().write("Initialized GUI System.");
	if (TTF_Init() == -1)
		printf("TTF_Init: %s\n", TTF_GetError());
}

void Omnia::GUISystem::onEarly(std::shared_ptr<Scene> scene)
{
	Input& input = OS::getInput();
	std::unordered_map<std::string, double> numbers;
	std::unordered_map<std::string, std::string> strings;

	/* Sends an Engine loop event for a detected file drop. */
	if (input.isDropFileDetected())
	{
		numbers.emplace((std::string)"drop_file_window_id", (double)input.getDropFileWindowID());
		strings.emplace((std::string)"drop_file_path", input.getDropFilePath());
		EventBus::publish("file dropped on window", numbers, strings);
	}

	/* Informs GUIs if the mouse is hovering or clicking on them or their widgets. */
	glm::vec2 mousePosition = input.getMousePosition();

	for (auto sceneLayer : scene->getSceneLayers())
	{
		std::vector<std::shared_ptr<GUI>> guis = sceneLayer.second->getComponentsByType<GUI>();

		for (int i = 0; i < guis.size(); i++)
		{
			std::shared_ptr<GUI> gui = guis[i];
			float mouseDetectionAccuracyRange = 0.1;

			/* To enforce the GUI following a target Entity by an offset. */
			if (gui->isFollowingEntity)
			{
				std::shared_ptr<Entity> followedEntity = sceneLayer.second->getEntityByName(gui->followTargetEntityName);
				std::shared_ptr<Transform> followedEntityTransform = sceneLayer.second->getComponentByType<Transform>(followedEntity->getID());
				std::vector<std::shared_ptr<Viewport>> uiViewports = sceneLayer.second->getComponentsByType<Viewport>();
				std::shared_ptr<Transform> cameraTransform;

				for (int i = 0; i < uiViewports.size(); i++)
				{
					std::shared_ptr<Viewport> uiViewport = uiViewports[i];
					std::shared_ptr<Entity> cameraEntity = sceneLayer.second->getEntityByName(uiViewport->getCameraEntityName());
					cameraTransform = sceneLayer.second->getComponentByType<Transform>(cameraEntity->getID());
				}

				if (cameraTransform != nullptr)
				{
					/* Set the GUI position on an offset relative to the followed Entity in the Camera view. */

					if (sceneLayer.second->is2D)
					{

					}
					else
					{

					}
				}
			}

			for (auto guiPanelTabGroups : gui->guiPanelTabGroups)
			{
				std::shared_ptr<GUIPanel> activeGUIPanel = guiPanelTabGroups.second->guiPanels[guiPanelTabGroups.second->activeGuiPanelName];
				glm::vec2 guiPanelTabGroupPosition = guiPanelTabGroups.second->position;
				
				for (auto widget : activeGUIPanel->widgets)
				{
					glm::vec2 widgetGlobalPosition = gui->position + guiPanelTabGroupPosition + widget.second->position;
					widget.second->detectedInputs = { 0 };

					/* If the mouse is at least hovering over the GUI widget. */
					if (glm::length(mousePosition - widgetGlobalPosition) < mouseDetectionAccuracyRange)
					{
						widget.second->detectedInputs.isHovered = true;
						widget.second->detectedInputs.isLeftMouseButtonOnPress = input.isLeftMouseButtonOnPress();
						widget.second->detectedInputs.isLeftMouseButtonOnRelease = input.isLeftMouseButtonOnRelease();
						widget.second->detectedInputs.isLeftMouseButtonDoubleClicked = input.isLeftMouseButtonDoubleClicked();
						widget.second->detectedInputs.isMiddleMouseButtonOnPress = input.isMiddleMouseButtonOnPress();
						widget.second->detectedInputs.isMiddleMouseButtonOnRelease = input.isMiddleMouseButtonOnRelease();
						widget.second->detectedInputs.isMiddleMouseButtonDoubleClicked = input.isMiddleMouseButtonDoubleClicked();
						widget.second->detectedInputs.isRightMouseButtonOnPress = input.isRightMouseButtonOnPress();
						widget.second->detectedInputs.isRightMouseButtonOnRelease = input.isRightMouseButtonOnRelease();
						widget.second->detectedInputs.isRightMouseButtonDoubleClicked = input.isRightMouseButtonDoubleClicked();
					}
				}
			}
		}
	}
}

void Omnia::GUISystem::finalize()
{
	this->isInitialized = false;
}