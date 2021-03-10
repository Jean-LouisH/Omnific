#pragma once

#include "component.hpp"
#include "game/scene/assets/texture.hpp"
#include "utilities/string.hpp"
#include "game/scene/assets/font.hpp"

namespace Lilliputian
{
	class UITextLabel : public Component
	{
	public:
		void setText(String text);
		void setFont(Font font);
	private:
		String text;
		Font font;
		Texture texture;

		void generateTexture();
	};
}