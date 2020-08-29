#include "RenderingSDL.hpp"
#include "SDLRenderable.hpp"

void SDL::Rendering2D::buildRenderablesFromSprites(
	std::vector<SDLRenderable>* SDLRenderables,
	std::vector<Sprite2D> sprite2Ds,
	Camera2D camera2D,
	SDL_Window* window)
{
	SDLRenderables->clear();
	int windowWidth_px = 0;
	int windowHeight_px = 0;
	SDL_GetWindowSize(window, &windowWidth_px, &windowHeight_px);

	Sprite2D* sprites = sprite2Ds.data();
	int spriteCount = sprite2Ds.size();
	for (int i = 0; i < spriteCount; i++)
	{
		Sprite2D* sprite = &sprites[i];

		float spriteWidth = sprite->textureFrames.at(sprite->frameIndex).pixels.width;
		float spriteHeight = sprite->textureFrames.at(sprite->frameIndex).pixels.height;
		float spriteWidthScaled = spriteWidth * sprite->transform.scale.x;
		float spriteHeightScaled = spriteHeight * sprite->transform.scale.y;
		float cameraWidth = camera2D.viewport_px.width;
		float cameraHeight = camera2D.viewport_px.height;
		float cameraX = camera2D.transform.position_px.x;
		float cameraY = camera2D.transform.position_px.y;
		float spriteX = sprite->transform.position_px.x;
		float spriteY = sprite->transform.position_px.y;
		float rotationRelativeToCamera =
			(sprite->transform.rotation_rad - camera2D.transform.rotation_rad) * 180.0;

		SDLRenderable renderable;
		renderable.texture = sprite->textureFrames.at(sprite->frameIndex).data;
		
		renderable.textureRect.x = 0;
		renderable.textureRect.y = 0;
		renderable.textureRect.w = spriteWidth;
		renderable.textureRect.h = spriteHeight;
		
		renderable.renderingRect.x = (int)((((spriteX - (spriteWidthScaled / 2.0) - cameraX) / cameraWidth) + 0.5) * (float)windowWidth_px);
		renderable.renderingRect.y = (int)((((cameraY - spriteY - (spriteHeightScaled / 2.0)) / cameraHeight) + 0.5) * (float)windowHeight_px);
		renderable.renderingRect.w = (int)(spriteWidthScaled / cameraWidth * (float)windowWidth_px);
		renderable.renderingRect.h = (int)(spriteHeightScaled / cameraHeight * (float)windowHeight_px);
		
		renderable.rotation = rotationRelativeToCamera;
		renderable.flip = sprite->flip;
		
		SDLRenderables->push_back(renderable);
	}
}