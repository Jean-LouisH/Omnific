#pragma once

#include "../../Utilities/Aliases.hpp"
#include "../../Utilities/Collections/Vector.hpp"
#include "../../Utilities/Constants.hpp"
#include "Assets/Texture.hpp"

namespace Lilliputian
{
	class AnimatedSprite2D
	{
	private:
		enum FlipStates
		{
			FLIP_HORIZONTAL = 1 << 0,
			FLIP_VERTICAL = 1 << 1
		};

		Vector<Texture> textureFrames;
		uint8_t flip = 0;
	public:
		EntityID entityID = NO_ENTITY;

		uint8_t frameIndex = 0;
		uint8_t alpha = 255;

		void addTextureFrame(Texture texture);
		void flipHorizontally();
		void flipVertically();
		bool getVerticalFlip();
		bool getHorizontalFlip();
		Vector<Texture> getTextureFrames();
	};
}