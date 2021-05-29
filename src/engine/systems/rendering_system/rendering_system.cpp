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

Lilliputian::RenderingSystem::~RenderingSystem()
{
	SDL_DestroyRenderer(this->sdlRenderer);
	IMG_Quit();
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

void Lilliputian::RenderingSystem::process(Scene& scene)
{
	Vector<SDL::Rendering2D::Sprite2D> outputsprite2Ds;
	SDL::Rendering2D::Camera2D outputCamera2D;
	outputCamera2D.isStreaming = false;

	Vector<SceneTree2D> sceneTree2Ds = scene.getSceneTree2Ds();
	int sceneTree2DCount = sceneTree2Ds.size();

	for (int i = 0; i < sceneTree2DCount; i++)
	{
		SceneTree2D sceneTree2D = sceneTree2Ds.at(i);
		Vector<ComponentVariant> componentVariants = sceneTree2D.getComponentVariants();

		for (int j = 0; j < componentVariants.size(); j++)
		{
			SDL::Rendering2D::Sprite2D outputSprite2D;
			ComponentVariant componentVariant = componentVariants.at(j);
			Camera2D* camera2D;
			Sprite* sprite;
			AnimatedSprite* animatedSprite;
			UITextLabel* uiTextLabel;
			SDL::Rendering2D::Texture outputTexture;
			Image image;

			if (componentVariant.isRenderable())
			{
				Transform2D transform2D = sceneTree2D.getEntityTransform(componentVariant.getEntityID());
				Vector2 position_px = transform2D.position_px;
				Vector2 scale = transform2D.scale;
				SDL_Texture* sdlTexture;

				outputSprite2D.transform.position_px.x = position_px.x;
				outputSprite2D.transform.position_px.y = position_px.y;
				outputSprite2D.transform.scale.x = scale.x;
				outputSprite2D.transform.scale.y = scale.y;
				outputSprite2D.transform.rotation_rad = transform2D.rotation_rad;

				image = componentVariant.getImage();

				outputTexture.pixels.width = image.getWidth();
				outputTexture.pixels.height = image.getHeight();

				if (!this->textureCache.containsKey(image.getID()))
				{
					outputTexture.data = SDL_CreateTextureFromSurface(this->sdlRenderer, image.getSDLSurface());
					this->textureCache.emplace(image.getID(), outputTexture.data);
				}
				else
				{
					outputTexture.data = this->textureCache.at(image.getID());
				}

				outputSprite2D.textureFrames.push_back(outputTexture);
				outputSprite2D.alpha = image.getAlpha();
				outputsprite2Ds.push_back(outputSprite2D);

			}
			else if (componentVariant.getType() == ComponentVariant::Type::CAMERA_2D &&
				 componentVariant.getID() == sceneTree2D.getCurrentCameraIndex())
			{
				Transform2D transform2D = sceneTree2D.getEntityTransform(componentVariant.getEntityID());
				Vector2 position_px = transform2D.position_px;
				Vector2 scale = transform2D.scale;
				camera2D = componentVariant.getCamera2D();
				Rectangle viewport_px = camera2D->getViewportDimensions();

				outputCamera2D.isStreaming = true;
				outputCamera2D.transform.position_px.x = position_px.x;
				outputCamera2D.transform.position_px.y = position_px.y;
				outputCamera2D.transform.scale.x = scale.x;
				outputCamera2D.transform.scale.y = scale.y;
				outputCamera2D.transform.rotation_rad = transform2D.rotation_rad;
				outputCamera2D.viewport_px.width = viewport_px.width;
				outputCamera2D.viewport_px.height = viewport_px.height;
			}
		}
	}

	if (outputCamera2D.isStreaming)
	{
		SDL::Rendering2D::buildRenderablesFromSprites(
			&this->sdlRenderables,
			outputsprite2Ds,
			outputCamera2D,
			this->sdlWindow);
	}

	this->clearBuffers();
	this->render();
	this->textureCache.collectGarbage();
}