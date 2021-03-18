#pragma once

#include "component.hpp"
#include "game/scene/assets/image.hpp"
#include "utilities/string.hpp"
#include "game/scene/assets/font.hpp"
#include "utilities/rectangle.hpp"
#include "utilities/colour.hpp"

namespace Lilliputian
{
	class UITextLabel : public Component
	{
	public:
		void setText(String text);
		void setFont(Font font);
		void setDimensions(float width, float height);
		void setColour(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);
	private:
		String text;
		Font font;
		Rectangle rectangle;
		Colour colour;

		Image texture;

		void generateTexture();
	};
}