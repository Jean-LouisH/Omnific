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

		/*Temporary implementation for loading just text. */
		if (it3->first.as<std::string>() == "GUIPanel")
		{
			GUIPanel guiPanel;

			for (YAML::const_iterator it4 = it3->second.begin(); it4 != it3->second.end(); ++it4)
			{
				if (it4->first.as<std::string>() == "GUIWidget")
				{
					GUIWidget guiWidget;

					for (YAML::const_iterator it5 = it4->second.begin(); it5 != it4->second.end(); ++it5)
					{
						if (it5->first.as<std::string>() == "GUIText")
						{
							GUIText guiText;

							for (YAML::const_iterator it6 = it5->second.begin(); it6 != it5->second.end(); ++it6)
							{
								if (it6->first.as<std::string>() == "text")
								{
									guiText.setText(it6->second.as<std::string>());
								}
								else if (it6->first.as<std::string>() == "font")
								{
									//it6->second[1].as<int>()));

									std::shared_ptr<Omnia::Font> font = OS::getFileAccess().loadAssetByType<Font>(it6->second[0].as<std::string>());
									guiText.setFont(font, it6->second[1].as<int>());
								}
								else if (it6->first.as<std::string>() == "colour")
								{
									guiText.setColour(
										it6->second[0].as<int>(),
										it6->second[1].as<int>(),
										it6->second[2].as<int>(),
										it6->second[3].as<int>()
									);
								}
							}

							guiWidget.guiText = guiText;
						}

					}

					guiPanel.widgets.emplace("Widget (ID:" + std::to_string(UIDGenerator::getNewUID()) + ")", guiWidget);

				}

			}

			this->guiPanels.emplace("Panel (ID:" + std::to_string(UIDGenerator::getNewUID()) + ")", guiPanel);
		}
	}

	this->updateImage();
}

void Omnia::GUIText::setText(std::string text)
{
	if (text != this->text)
	{
		this->text = text;
		this->generateImage();
	}
}

void Omnia::GUIText::setFont(std::shared_ptr<Omnia::Font> font, uint16_t size_px)
{
	this->font = font;
	this->size = size_px;
	this->generateImage();
}

void Omnia::GUIText::setFontSize(uint16_t size_px)
{
	this->size = size_px;
	this->generateImage();
}

void Omnia::GUIText::setColour(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
{
	this->colour = Colour(red, green, blue, alpha);
	this->generateImage();
}

void Omnia::GUIText::generateImage()
{
	if (this->font != nullptr)
	{
		if (this->font->getSDLTTFFont() != nullptr)
		{
			this->image = std::shared_ptr<Image>(new Image(this->text, this->font, this->colour, this->wrapLength));
		}
	}
}

void Omnia::GUI::updateImage()
{
	/* Temporary for a single GUIText*/
	if (this->guiPanels.size() == 1)
	{
		for (auto guiPanel : this->guiPanels)
		{
			if (guiPanel.second.widgets.size() == 1)
			{
				for (auto widget : guiPanel.second.widgets)
				{
					this->image = widget.second.guiText.image;
				}
			}
		}
	}

	this->setDimensions(this->image->getWidth(), this->image->getHeight(), 0);

	/*ToDo*/

}

void Omnia::GUI::setText(std::string text)
{
	/* Basic temporary implementation for a text only GUI*/
	if (this->guiPanels.size() == 1)
	{
		for (auto guiPanel : this->guiPanels)
		{
			if (guiPanel.second.widgets.size() == 1)
			{
				for (auto widget : guiPanel.second.widgets)
				{
					widget.second.guiText.setText(text);
				}
			}
		}
	}

	this->updateImage();
}