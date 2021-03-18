#pragma once

#include "utilities/aliases.hpp"
#include "utilities/collections/vector.hpp"
#include "utilities/collections/map.hpp"
#include "utilities/constants.hpp"
#include "utilities/string.hpp"
#include "game/scene/assets/image.hpp"
#include "component.hpp"

namespace Lilliputian
{
	class Sprite : public Component
	{
	public:
		void setTexture(Image texture);
		void setAlpha(uint8_t value);
		uint8_t getAlpha();
		void hide();
		void show();

		void flipVertically();
		void flipHorizontally();

		Image& getTexture();
	private:
		Image texture;
		uint8_t alpha = 255;
		bool isFlippedVertically = false;
		bool isFlippedHorizontally = false;
	};
}