#include "rendering_system.hpp"
#include "rendering_sdl/rendering_sdl.hpp"
#include <SDL_image.h>

Lilliputian::RenderingSystem::RenderingSystem(Window& window)
{
	IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);

	this->sdlWindow = window.getSDLWindow();
	this->sdlRenderer = SDL_CreateRenderer(
		sdlWindow,
		-1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
}

void Lilliputian::RenderingSystem::clearBuffers()
{
	SDL_SetRenderDrawColor(this->sdlRenderer, 0, 0, 0, 255);
	SDL_RenderClear(this->sdlRenderer);
}

void Lilliputian::RenderingSystem::render()
{
	for (int i = 0; i < this->sdlRenderables.size(); i++)
	{	
		SDL::Rendering2D::SDLRenderable sdlRenderable = this->sdlRenderables.at(i);
		SDL_RenderCopyEx(
			this->sdlRenderer,
			sdlRenderable.texture,
			&sdlRenderable.textureRect,
			&sdlRenderable.renderingRect,
			sdlRenderable.rotation,
			NULL,
			sdlRenderable.flip
		);
	}
	SDL_RenderPresent(this->sdlRenderer);
}

void Lilliputian::RenderingSystem::process(SceneForest& scene)
{
	Vector<SDL::Rendering2D::Sprite2D> sprite2Ds;
	SDL::Rendering2D::Camera2D sdlCamera2D;

	Vector<Stack<SceneTree2D>> sceneTree2DStacks = scene.getSceneTree2DStacks();
	int sceneTree2DCount = sceneTree2DStacks.size();

	for (int i = 0; i < sceneTree2DCount; i++)
	{
		SceneTree2D sceneTree2D = sceneTree2DStacks.at(i).top();
		Vector<ComponentVariant> componentVariants = sceneTree2D.getComponentVariants();

		for (int j = 0; j < componentVariants.size(); j++)
		{
			SDL::Rendering2D::Sprite2D sdlSprite2D;
			ComponentVariant componentVariant = componentVariants.at(j);
			Camera2D* camera2D;
			Sprite* sprite;
			AnimatedSprite* animatedSprite;
			SDL::Rendering2D::Texture sdlRendererTexture;
			Texture texture;

			if (componentVariant.type == ComponentVariant::Type::COMPONENT_TYPE_SPRITE || 
				componentVariant.type == ComponentVariant::Type::COMPONENT_TYPE_ANIMATED_SPRITE)
			{
				Transform2D transform2D = sceneTree2D.getEntityTransform(componentVariant.entityID);
				Vector2 position_px = transform2D.position_px;
				Vector2 scale = transform2D.scale;
				SDL_Texture* sdlTexture;

				sdlSprite2D.transform.position_px.x = position_px.x;
				sdlSprite2D.transform.position_px.y = position_px.y;
				sdlSprite2D.transform.scale.x = scale.x;
				sdlSprite2D.transform.scale.y = scale.y;
				sdlSprite2D.transform.rotation_rad = transform2D.rotation_rad;

				if (componentVariant.type == ComponentVariant::Type::COMPONENT_TYPE_SPRITE)
				{
					sprite = componentVariant.sprite;
					texture = sprite->getTexture();
					sdlRendererTexture.pixels.width = texture.getWidth();
					sdlRendererTexture.pixels.height = texture.getHeight();

					if (this->sdlTextureCache.count(texture.getSDLSurface()) == 0)
					{
						sdlRendererTexture.data = SDL_CreateTextureFromSurface(this->sdlRenderer, texture.getSDLSurface());
						this->sdlTextureCache.emplace(texture.getSDLSurface(), sdlRendererTexture.data);
					}
					else
					{
						sdlRendererTexture.data = this->sdlTextureCache.at(texture.getSDLSurface());
					}

					sdlSprite2D.textureFrames.push_back(sdlRendererTexture);
					sdlSprite2D.alpha = sprite->getAlpha();

					sprite2Ds.push_back(sdlSprite2D);
				}
				else if (componentVariant.type == ComponentVariant::Type::COMPONENT_TYPE_ANIMATED_SPRITE)
				{
					animatedSprite = componentVariant.animatedSprite;
					texture = animatedSprite->getCurrentFrame();
					sdlRendererTexture.pixels.width = texture.getWidth();
					sdlRendererTexture.pixels.height = texture.getHeight();

					if (this->sdlTextureCache.count(texture.getSDLSurface()) == 0)
					{
						sdlRendererTexture.data = SDL_CreateTextureFromSurface(this->sdlRenderer, texture.getSDLSurface());
						this->sdlTextureCache.emplace(texture.getSDLSurface(), sdlRendererTexture.data);
					}
					else
					{
						sdlRendererTexture.data = this->sdlTextureCache.at(texture.getSDLSurface());
					}

					sdlSprite2D.textureFrames.push_back(sdlRendererTexture);
					sdlSprite2D.alpha = animatedSprite->getAlpha();

					sprite2Ds.push_back(sdlSprite2D);
				}
			}
			else if (componentVariant.type == ComponentVariant::Type::COMPONENT_TYPE_CAMERA_2D)
			{
				Transform2D transform2D = sceneTree2D.getEntityTransform(componentVariant.entityID);
				Vector2 position_px = transform2D.position_px;
				Vector2 scale = transform2D.scale;
				camera2D = componentVariant.camera2D;
				Rectangle viewport_px = camera2D->getViewportDimensions();

				sdlCamera2D.transform.position_px.x = position_px.x;
				sdlCamera2D.transform.position_px.y = position_px.y;
				sdlCamera2D.transform.scale.x = scale.x;
				sdlCamera2D.transform.scale.y = scale.y;
				sdlCamera2D.transform.rotation_rad = transform2D.rotation_rad;

				sdlCamera2D.viewport_px.width = viewport_px.width;
				sdlCamera2D.viewport_px.height = viewport_px.height;
			}
		}
	}

	SDL::Rendering2D::buildRenderablesFromSprites(
		&this->sdlRenderables,
		sprite2Ds,
		sdlCamera2D,
		this->sdlWindow);

	this->clearBuffers();
	this->render();
}