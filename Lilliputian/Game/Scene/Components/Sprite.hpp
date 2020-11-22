#pragma once

#include "../../../Utilities/Aliases.hpp"
#include "../../../Utilities/Collections/Vector.hpp"
#include "../../../Utilities/Collections/Map.hpp"
#include "../../../Utilities/Constants.hpp"
#include "../../../Utilities/String.hpp"
#include "../Assets/Texture.hpp"
#include "Component.hpp"

namespace Lilliputian
{
	class Sprite : public Component
	{
	public:
		void setAlpha(uint8_t value);
		uint8_t getAlpha();
		void hide();
		void show();

		void flipVertically();
		void flipHorizontally();

		Texture getTexture();
	private:
		Texture texture;
		uint8_t alpha = 255;
		bool isFlippedVertically = false;
		bool isFlippedHorizontally = false;
	};
}