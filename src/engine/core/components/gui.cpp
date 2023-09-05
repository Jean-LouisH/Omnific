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

#include "gui.hpp"
#include <core/singletons/os/os.hpp>
#include <core/singletons/uid_generator.hpp>

void Omnia::GUI::deserialize(YAML::Node yamlNode)
{
	for (YAML::const_iterator it3 = yamlNode.begin(); it3 != yamlNode.end(); ++it3)
	{
		RenderableComponent::deserialize(yamlNode);

		if (it3->first.as<std::string>() == "GUIPanelTabGroup")
		{
			std::shared_ptr<GUIPanelTabGroup> guiPanelTabGroup(new GUIPanelTabGroup());

			for (YAML::const_iterator it4 = it3->second.begin(); it4 != it3->second.end(); ++it4)
			{
				if (it4->first.as<std::string>() == "name")
				{
					guiPanelTabGroup->name = it4->second.as<std::string>();
				}
				else if (it4->first.as<std::string>() == "position")
				{
					guiPanelTabGroup->position.x = it4->second[0].as<double>();
					guiPanelTabGroup->position.y = it4->second[1].as<double>();
				}
				else if (it4->first.as<std::string>() == "dimensions")
				{
					guiPanelTabGroup->dimensions.x = it4->second[0].as<double>();
					guiPanelTabGroup->dimensions.y = it4->second[1].as<double>();
				}
				else if (it4->first.as<std::string>() == "colour")
				{
					guiPanelTabGroup->backgroundColour = std::shared_ptr<Colour>(new Colour(it4->second.as<std::string>()));
				}
				else if (it4->first.as<std::string>() == "image")
				{
					;
				}
				else if (it4->first.as<std::string>() == "GUIPanel")
				{
					std::shared_ptr<GUIPanel> guiPanel(new GUIPanel());
					std::shared_ptr<GUIWidget> guiWidget;

					for (YAML::const_iterator it5 = it4->second.begin(); it5 != it4->second.end(); ++it5)
					{
						if (it5->first.as<std::string>() == "name")
						{
							guiPanel->name = it5->second.as<std::string>();
						}
						else if (it5->first.as<std::string>() == "position")
						{
							guiPanel->position.x = it5->second[0].as<double>();
							guiPanel->position.y = it5->second[1].as<double>();
						}
						else if (it5->first.as<std::string>() == "dimensions")
						{
							guiPanel->dimensions.x = it5->second[0].as<double>();
							guiPanel->dimensions.y = it5->second[1].as<double>();
						}
						else if (it5->first.as<std::string>() == "colour")
						{
							guiPanel->backgroundColour = std::shared_ptr<Colour>(new Colour(it5->second.as<std::string>()));
						}
						else if (it5->first.as<std::string>() == "image")
						{
							;
						}
						else if (it5->first.as<std::string>() == "GUIMenuBar")
						{
							std::shared_ptr<GUIMenuBar> guiMenuBar(new GUIMenuBar());

							for (YAML::const_iterator it6 = it5->second.begin(); it6 != it5->second.end(); ++it6)
							{
								if (it6->first.as<std::string>() == "")
								{

								}
							}

							guiWidget = std::dynamic_pointer_cast<GUIWidget>(guiMenuBar);
						}
						else if (it5->first.as<std::string>() == "GUIContextMenu")
						{
							std::shared_ptr<GUIContextMenu> guiContextMenu(new GUIContextMenu());

							for (YAML::const_iterator it6 = it5->second.begin(); it6 != it5->second.end(); ++it6)
							{
								if (it6->first.as<std::string>() == "")
								{

								}
							}

							guiWidget = std::dynamic_pointer_cast<GUIWidget>(guiContextMenu);
						}
						else if (it5->first.as<std::string>() == "GUIColour")
						{
							std::shared_ptr<GUIColour> guiColour(new GUIColour());

							for (YAML::const_iterator it6 = it5->second.begin(); it6 != it5->second.end(); ++it6)
							{
								if (it6->first.as<std::string>() == "colour")
								{
									guiColour->backgroundColour = std::shared_ptr<Colour>(new Colour(it6->second.as<std::string>()));
								}
							}

							guiColour->updateImage();
							guiWidget = std::dynamic_pointer_cast<GUIWidget>(guiColour);
						}
						else if (it5->first.as<std::string>() == "GUIButton")
						{
							std::shared_ptr<GUIButton> guiButton(new GUIButton());

							guiButton->isClickable = true;
							guiButton->isHighlightable = true;

							for (YAML::const_iterator it6 = it5->second.begin(); it6 != it5->second.end(); ++it6)
							{
								if (it6->first.as<std::string>() == "is_x_stretched_to_panel")
								{
									guiButton->isXStretchedToPanel = it6->second.as<bool>();
								}
								else if (it6->first.as<std::string>() == "is_y_stretched_to_panel")
								{
									guiButton->isYStretchedToPanel = it6->second.as<bool>();
								}
								else if (it6->first.as<std::string>() == "default_dimensions")
								{
									guiButton->dimensions.x = it6->second[0].as<double>();
									guiButton->dimensions.y = it6->second[1].as<double>();
								}
								else if (it6->first.as<std::string>() == "position")
								{
									guiButton->position.x = it6->second[0].as<double>();
									guiButton->position.y = it6->second[1].as<double>();
								}
								else if (it6->first.as<std::string>() == "text")
								{
									guiButton->guiText->text = it6->second.as<std::string>();
								}
								else if (it6->first.as<std::string>() == "font")
								{
									std::shared_ptr<Omnia::Font> font = OS::getFileAccess().loadAssetByType<Font>(it6->second[0].as<std::string>());
									*font = Font(font->getName(), it6->second[1].as<int>());
									guiButton->guiText->font = font;
									guiButton->guiText->size = it6->second[1].as<int>();
								}
								else if (it6->first.as<std::string>() == "text_colour")
								{
									guiButton->guiText->colour = std::shared_ptr<Colour>(new Colour(it6->second.as<std::string>()));
								}
								else if (it6->first.as<std::string>() == "colour")
								{
									guiButton->backgroundColour = std::shared_ptr<Colour>(new Colour(it6->second.as<std::string>()));
								}
								else if (it6->first.as<std::string>() == "image")
								{
									;
								}
							}

							guiButton->updateImage();
							guiWidget = std::dynamic_pointer_cast<GUIWidget>(guiButton);
						}
						else if (it5->first.as<std::string>() == "GUIToggleButton")
						{
							std::shared_ptr<GUIToggleButton> guiToggleButton(new GUIToggleButton());

							for (YAML::const_iterator it6 = it5->second.begin(); it6 != it5->second.end(); ++it6)
							{
								if (it6->first.as<std::string>() == "")
								{

								}
							}

							guiWidget = std::dynamic_pointer_cast<GUIWidget>(guiToggleButton);
						}
						else if (it5->first.as<std::string>() == "GUIList")
						{
							std::shared_ptr<GUIList> guiList(new GUIList());

							for (YAML::const_iterator it6 = it5->second.begin(); it6 != it5->second.end(); ++it6)
							{
								if (it6->first.as<std::string>() == "")
								{

								}
							}

							guiWidget = std::dynamic_pointer_cast<GUIWidget>(guiList);
						}
						else if (it5->first.as<std::string>() == "GUITree")
						{
							std::shared_ptr<GUITree> guiTree(new GUITree());

							for (YAML::const_iterator it6 = it5->second.begin(); it6 != it5->second.end(); ++it6)
							{
								if (it6->first.as<std::string>() == "")
								{

								}
							}

							guiWidget = std::dynamic_pointer_cast<GUIWidget>(guiTree);
						}
						else if (it5->first.as<std::string>() == "GUIText")
						{
							std::shared_ptr<GUIText> guiText(new GUIText());

							for (YAML::const_iterator it6 = it5->second.begin(); it6 != it5->second.end(); ++it6)
							{
								if (it6->first.as<std::string>() == "is_clickable")
								{
									guiText->isClickable = it6->second.as<bool>();
								}
								if (it6->first.as<std::string>() == "is_highlightable")
								{
									guiText->isHighlightable = it6->second.as<bool>();
								}
								if (it6->first.as<std::string>() == "is_x_stretched_to_panel")
								{
									guiText->isXStretchedToPanel = it6->second.as<bool>();
								}
								else if (it6->first.as<std::string>() == "is_y_stretched_to_panel")
								{
									guiText->isYStretchedToPanel = it6->second.as<bool>();
								}
								else if (it6->first.as<std::string>() == "position")
								{
									guiText->position.x = it6->second[0].as<double>();
									guiText->position.y = it6->second[1].as<double>();
								}
								else if (it6->first.as<std::string>() == "text")
								{
									guiText->text = it6->second.as<std::string>();
								}
								else if (it6->first.as<std::string>() == "font")
								{
									std::shared_ptr<Omnia::Font> font = OS::getFileAccess().loadAssetByType<Font>(it6->second[0].as<std::string>());
									*font = Font(font->getName(), it6->second[1].as<int>());
									guiText->font = font;
									guiText->size = it6->second[1].as<int>();
								}
								else if (it6->first.as<std::string>() == "colour")
								{
									guiText->colour = std::shared_ptr<Colour>(new Colour(it6->second.as<std::string>()));
								}
								else if (it6->first.as<std::string>() == "wrap_length")
								{
									guiText->wrapLength = it6->second.as<int>();
								}
							}

							guiText->updateImage();
							guiWidget = std::dynamic_pointer_cast<GUIWidget>(guiText);
						}
						else if (it5->first.as<std::string>() == "GUITiles")
						{
							std::shared_ptr<GUITiles> guiTiles(new GUITiles());

							for (YAML::const_iterator it6 = it5->second.begin(); it6 != it5->second.end(); ++it6)
							{
								if (it6->first.as<std::string>() == "")
								{

								}
							}

							guiWidget = std::dynamic_pointer_cast<GUIWidget>(guiTiles);
						}
						else if (it5->first.as<std::string>() == "GUIListBox")
						{
							std::shared_ptr<GUIList> guiListBox(new GUIList());

							for (YAML::const_iterator it6 = it5->second.begin(); it6 != it5->second.end(); ++it6)
							{
								if (it6->first.as<std::string>() == "")
								{

								}
							}

							guiWidget = std::dynamic_pointer_cast<GUIWidget>(guiListBox);
						}
						else if (it5->first.as<std::string>() == "GUISpinner")
						{
							std::shared_ptr<GUISpinner> guiSpinner(new GUISpinner());

							for (YAML::const_iterator it6 = it5->second.begin(); it6 != it5->second.end(); ++it6)
							{
								if (it6->first.as<std::string>() == "")
								{

								}
							}

							guiWidget = std::dynamic_pointer_cast<GUIWidget>(guiSpinner);
						}
						else if (it5->first.as<std::string>() == "GUIDropDownList")
						{
							std::shared_ptr<GUIDropDownList> guiDropDownList(new GUIDropDownList());

							for (YAML::const_iterator it6 = it5->second.begin(); it6 != it5->second.end(); ++it6)
							{
								if (it6->first.as<std::string>() == "")
								{

								}
							}

							guiWidget = std::dynamic_pointer_cast<GUIWidget>(guiDropDownList);
						}
						else if (it5->first.as<std::string>() == "GUISlider")
						{
							std::shared_ptr<GUISlider> guiSlider(new GUISlider());

							for (YAML::const_iterator it6 = it5->second.begin(); it6 != it5->second.end(); ++it6)
							{
								if (it6->first.as<std::string>() == "")
								{

								}
							}

							guiWidget = std::dynamic_pointer_cast<GUIWidget>(guiSlider);
						}
						else if (it5->first.as<std::string>() == "GUILine")
						{
							std::shared_ptr<GUILine> guiLine(new GUILine());

							for (YAML::const_iterator it6 = it5->second.begin(); it6 != it5->second.end(); ++it6)
							{
								if (it6->first.as<std::string>() == "")
								{

								}
							}

							guiWidget = std::dynamic_pointer_cast<GUIWidget>(guiLine);
						}
						else if (it5->first.as<std::string>() == "GUITreeView")
						{
							std::shared_ptr<GUITreeView> guiTreeView(new GUITreeView());

							for (YAML::const_iterator it6 = it5->second.begin(); it6 != it5->second.end(); ++it6)
							{

							}

							guiWidget = std::dynamic_pointer_cast<GUIWidget>(guiTreeView);
						}

						if (guiWidget != nullptr)
							guiPanel->widgets.emplace(UIDGenerator::getNewUID(), guiWidget);
					}

					if (guiPanelTabGroup->activeGuiPanelName == "")
						guiPanelTabGroup->activeGuiPanelName = guiPanel->name;

					guiPanel->updateImage();
					guiPanelTabGroup->guiPanels.emplace(guiPanel->name, guiPanel);
				}
			}

			guiPanelTabGroup->updateImage();
			this->guiPanelTabGroups.emplace(guiPanelTabGroup->name, guiPanelTabGroup);
		}
		else if (it3->first.as<std::string>() == "follow_target_entity_name")
		{
			this->followTargetEntityName = it3->second.as<std::string>();
			this->isFollowingEntity = true;
		}
		else if (it3->first.as<std::string>() == "follow_offset")
		{
			this->followOffset.x = it3->second[0].as<double>();
			this->followOffset.y = it3->second[1].as<double>();
		}
	}

	this->updateImage();
}

void Omnia::GUIColour::updateImage()
{
	this->image = std::shared_ptr<Image>(new Image(this->backgroundColour, this->dimensions.x, this->dimensions.y));
}

void Omnia::GUIButton::updateImage()
{
	/* The border of the button wraps around the text it contains by an offset. */
	this->guiText->updateImage();
	this->dimensions = this->guiText->dimensions + this->buttonSpaceFromText;
	this->image = std::shared_ptr<Image>(new Image(this->backgroundColour, this->dimensions.x, this->dimensions.y));
}

void Omnia::GUIPanel::updateImage()
{
	glm::vec2 maximumWidgetSidePositions = glm::vec2(0.0);

	for (auto widget : this->widgets)
	{
		glm::vec2 guiWidgetSidePositions = widget.second->position + widget.second->dimensions;
		if (guiWidgetSidePositions.x > maximumWidgetSidePositions.x)
			maximumWidgetSidePositions.x = guiWidgetSidePositions.x;
		if (guiWidgetSidePositions.y > maximumWidgetSidePositions.y)
			maximumWidgetSidePositions.y = guiWidgetSidePositions.y;
	}

	/* If Widgets take up more space than the Panel dimensions, then 
	   create the scrollbars for the sides of the panel. */
	if (maximumWidgetSidePositions.x > this->dimensions.x)
	{

	}

	if (maximumWidgetSidePositions.y > this->dimensions.y)
	{

	}

	this->image = std::shared_ptr<Image>(new Image(this->backgroundColour, this->dimensions.x, this->dimensions.y));
}

void Omnia::GUIPanelTabGroup::updateImage()
{
	this->image = std::shared_ptr<Image>(new Image(this->backgroundColour, this->dimensions.x, this->dimensions.y));
}

void Omnia::GUIText::setText(std::string text)
{
	if (text != this->text)
	{
		this->text = text;
		this->updateImage();
	}
}

void Omnia::GUIText::setFont(std::shared_ptr<Omnia::Font> font, uint16_t size_px)
{
	this->font = font;
	this->size = size_px;
	this->updateImage();
}

void Omnia::GUIText::setFontSize(uint16_t size_px)
{
	this->size = size_px;
	this->updateImage();
}

void Omnia::GUIText::setColour(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
{
	this->colour = std::shared_ptr<Colour>(new Colour(red, green, blue, alpha));
	this->updateImage();
}

void Omnia::GUIText::updateImage()
{
	if (this->font != nullptr && this->colour != nullptr)
	{
		if (this->font->getSDLTTFFont() != nullptr)
		{
			this->image = std::shared_ptr<Image>(new Image(this->text, this->font, this->colour, this->wrapLength));
			this->dimensions = this->image->getDimensions();
		}
	}
}

void Omnia::GUI::updateImage()
{
	if (this->guiPanelTabGroups.size() == 1)
	{
		for (auto guiPanelTabGroup : this->guiPanelTabGroups)
		{
			std::shared_ptr<GUIPanel> guiPanel = guiPanelTabGroup.second->guiPanels.at(guiPanelTabGroup.second->activeGuiPanelName);

			/* If the entire GUI Component is defined by a single GUIWidget, just set the image. */
			if (guiPanel->widgets.size() == 1)
			{
				for (auto widget : guiPanel->widgets)
				{
					this->image = widget.second->image;
				}
			}
		}
	}
	else
	{
		/* Generate a single GUI Component image based on all the Widgets on the Panels. */
		std::shared_ptr<Image> updatedImage;
		uint8_t* rawPixels;
		uint8_t colourChannels = 4;
		glm::vec2 maximumGUITabGroupSidePositions = glm::vec2(0.0);

		for (auto guiPanelTabGroup : this->guiPanelTabGroups)
		{
			glm::vec2 guiTabGroupSidePositions = guiPanelTabGroup.second->position + guiPanelTabGroup.second->dimensions;
			if (guiTabGroupSidePositions.x > maximumGUITabGroupSidePositions.x)
				maximumGUITabGroupSidePositions.x = guiTabGroupSidePositions.x;
			if (guiTabGroupSidePositions.y > maximumGUITabGroupSidePositions.y)
				maximumGUITabGroupSidePositions.y = guiTabGroupSidePositions.y;
		}

		size_t rawPixelsSize = maximumGUITabGroupSidePositions.x * maximumGUITabGroupSidePositions.y * colourChannels;
		rawPixels = new uint8_t[rawPixelsSize];

		/* Fill the raw pixels with empty, alpha values. */
		for (int i = 0; i < rawPixelsSize; i++)
		{
			rawPixels[i] = 0;
		}

		for (auto guiPanelTabGroup : this->guiPanelTabGroups)
		{
			if (this->guiPanelTabGroups.size() > 1)
			{
				std::shared_ptr<Image> guiPanelTabGroupImage = guiPanelTabGroup.second->image;
				std::shared_ptr<GUIText> tabNameText(new GUIText());

				//tabNameText->font = font;
				//tabNameText->size = it6->second[1].as<int>();
				//tabNameText->wrapLength = it6->second.as<int>();
				//tabNameText->colour = std::shared_ptr<Colour>(new Colour(
				//	(uint8_t)it6->second[0].as<int>(),
				//	(uint8_t)it6->second[1].as<int>(),
				//	(uint8_t)it6->second[2].as<int>(),
				//	(uint8_t)it6->second[3].as<int>())
				//);
				//tabNameText->text = it6->second.as<std::string>();
				
				/* Draw the tab group to the GUI. */
				Image::normalBlend(
					rawPixels,
					glm::vec2(0.0),
					maximumGUITabGroupSidePositions,
					guiPanelTabGroupImage->getData(),
					guiPanelTabGroup.second->position,
					guiPanelTabGroup.second->dimensions
				);
			}

			std::shared_ptr<GUIPanel> activeGUIPanel = guiPanelTabGroup.second->guiPanels[guiPanelTabGroup.second->activeGuiPanelName];

			/* Draw the active GUI panel to the tab group on the GUI. */
			Image::normalBlend(
				rawPixels,
				glm::vec2(0.0),
				maximumGUITabGroupSidePositions,
				activeGUIPanel->image->getData(),
				activeGUIPanel->position + guiPanelTabGroup.second->position,
				activeGUIPanel->dimensions
			);

			for (auto widget : activeGUIPanel->widgets)
			{
				/* Draw the widget to the active GUI panel to the tab group on the GUI. */
				Image::normalBlend(
					rawPixels,
					glm::vec2(0.0),
					maximumGUITabGroupSidePositions,
					(uint8_t*)widget.second->image->getData(),
					widget.second->position + activeGUIPanel->position + guiPanelTabGroup.second->position,
					widget.second->dimensions
				);
			}
		}
	}

	this->setDimensions(this->image->getWidth(), this->image->getHeight(), 0);
}

void Omnia::GUI::setAsText(std::string text)
{

	if (this->guiPanelTabGroups.size() > 0)
	{
		if (this->guiPanelTabGroups.size() == 1)
		{
			for (auto guiPanelTabGroup : this->guiPanelTabGroups)
			{
				if (guiPanelTabGroup.second->guiPanels.size() == 1)
				{
					for (auto guiPanel : guiPanelTabGroup.second->guiPanels)
					{
						if (guiPanel.second->widgets.size() == 1)
						{
							for (auto widget : guiPanel.second->widgets)
							{
								if (widget.second->widgetType == GUIText::TYPE_STRING)
								{
									std::shared_ptr<GUIWidget> guiWidget = widget.second;
									std::shared_ptr<GUIText> guiText = std::dynamic_pointer_cast<GUIText>(guiWidget);
									guiText->setText(text);
								}
							}
						}
					}
				}
			}
		}
	}
	else
	{
		//std::shared_ptr<GUIPanel> guiPanel(new GUIPanel());
		//std::shared_ptr<GUIPanelTabGroup> guiPanelTabGroup(new GUIPanelTabGroup());
		//std::shared_ptr<GUIText> guiText(new GUIText());

		////guiText->size;
		////guiText->font;
		////guiText->colour;
		////guiText->setText(text);

		//guiPanel->name = "Text";
		//guiPanel->widgets.emplace(guiText->id, guiText);
		//this->guiPanelTabGroups.emplace(guiPanel->name, guiPanelTabGroup);
	}

	this->updateImage();
}

std::shared_ptr<Omnia::GUIWidget> Omnia::GUI::getWidget(std::string widgetName)
{
	std::shared_ptr<GUIWidget> guiWidget;

	if (this->guiWidgetRegistry.count(widgetName))
		guiWidget = this->guiWidgetRegistry.at(widgetName);

	return guiWidget;
}