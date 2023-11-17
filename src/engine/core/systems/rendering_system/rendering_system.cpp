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
#include <core/assets/shader.hpp>
#include <core/assets/image.hpp>
#include <core/singletons/os/os.hpp>
#include <core/components/camera.hpp>
#include <core/components/viewport.hpp>
#include <core/components/transform.hpp>
#include <core/components/gui.hpp>
#include <core/components/model.hpp>
#include <core/singletons/configuration.hpp>

Omnia::RenderingSystem::RenderingSystem()
{
	SDL_InitSubSystem(SDL_INIT_VIDEO);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	OS::createWindow("",
		640,
		480,
		false,
		this->getRenderingBackendName());

	this->dummyLight = std::shared_ptr<Light>(new Light());
	this->dummyLightTransform = std::shared_ptr<Transform>(new Transform());
	this->openglBackend = std::shared_ptr<OpenGLRenderingBackend>(new OpenGLRenderingBackend());
	this->type = TYPE_STRING;
}

Omnia::RenderingSystem::~RenderingSystem()
{
	this->finalize();
}

void Omnia::RenderingSystem::initialize()
{
	Image image = Image(
		OS::getFileAccess().getDataDirectoryPath() + Configuration::getInstance()->metadata.iconFilepath);
	OS::getWindow().changeIcon(
		image.getData(),
		image.getWidth(),
		image.getHeight(),
		image.getDepth(),
		image.getPitch());

	this->openglBackend->initialize();
	this->isInitialized = true;
}

void Omnia::RenderingSystem::onLate(std::shared_ptr<Scene> scene)
{
	this->onWindowResize();

	if (this->hasSceneChanged(scene))
		this->buildRenderables(scene);

	this->openglBackend->clearColourBuffer(0, 0, 0, 255);

	for (int i = 0; i < this->renderableLayerLists.size(); i++)
	{
		std::vector<RenderableLayer> renderableLayerList = this->renderableLayerLists[i];
		RenderableLayer* renderableLayerListData = renderableLayerList.data();
		size_t renderableLayerListCount = renderableLayerList.size();

		for (size_t j = 0; j < renderableLayerListCount; j++)
		{
			RenderableLayer& renderableLayer = renderableLayerListData[j];

			if (renderableLayer.camera->getIsStreaming())
			{
				EntityRenderable* entityRenderablesData = renderableLayer.entityRenderables.data();
				size_t entityRenderablesCount = renderableLayer.entityRenderables.size();
				glm::mat4 worldToViewMatrix = glm::inverse(renderableLayer.cameraTransform->getTransformMatrix());
				glm::mat4 viewToProjectionMatrix = renderableLayer.camera->getViewToProjectionMatrix();

				if (renderableLayer.is2D)
				{
					this->openglBackend->disableDepthTest();
				}
				else
				{
					this->openglBackend->clearDepthBuffer();
					this->openglBackend->enableDepthTest();
				}

				if (renderableLayer.camera->getIsWireframeMode())
					this->openglBackend->enableWireframeMode();
				else
					this->openglBackend->disableWireframeMode();

				/* Memory allocated out of the tight loop. */
				std::shared_ptr<Transform> globalTransform(new Transform());

				std::vector<std::shared_ptr<Light>> activeLights;
				std::vector<std::shared_ptr<Transform>> activeLightTransforms;

				if (renderableLayer.lights.size() > 0)
				{
					activeLights = renderableLayer.lights;
					activeLightTransforms = renderableLayer.lightTransforms;
				}
				else
				{
					activeLights.push_back(this->dummyLight);
					activeLightTransforms.push_back(this->dummyLightTransform);
				}

				size_t lightsCount = activeLights.size();

				/* Forward Rendering */
				for (size_t k = 0; k < lightsCount; k++)
				{
					std::shared_ptr<Light> light = activeLights[k];
					std::shared_ptr<Transform> lightTransform = activeLightTransforms[k];

					for (size_t l = 0; l < entityRenderablesCount; l++)
					{
						EntityRenderable& entityRenderable = entityRenderablesData[l];
						std::shared_ptr<Shader> shader = entityRenderable.overridingShader;
						std::shared_ptr<ShaderParameters> shaderParameters = entityRenderable.overridingShaderParameters;

						if (shader == nullptr && shaderParameters == nullptr)
						{
							shader = entityRenderable.renderableComponent->getShader();
							shaderParameters = entityRenderable.renderableComponent->shaderParameters;
						}

						globalTransform = entityRenderable.entityTransform->getGlobalTransform();
						glm::mat4 modelToWorldMatrix = globalTransform->getTransformMatrix();
						glm::mat4 mvp = viewToProjectionMatrix * worldToViewMatrix * modelToWorldMatrix;
						float alpha = entityRenderable.renderableComponent->getAlphaInPercentage();
						const float cullAlphaThreshold = 1.0 - 0.001;
						RenderableComponent::CullMode cullMode = entityRenderable.renderableComponent->getCullMode();

						switch (cullMode)
						{
							case RenderableComponent::CullMode::NONE:
							case RenderableComponent::CullMode::BACK: glCullFace(GL_BACK); break;
							case RenderableComponent::CullMode::FRONT: glCullFace(GL_FRONT); break;
							case RenderableComponent::CullMode::FRONT_AND_BACK: glCullFace(GL_FRONT_AND_BACK); break;
						}

						if (cullMode == RenderableComponent::CullMode::NONE)
							glDisable(GL_CULL_FACE);
						else
							glEnable(GL_CULL_FACE);

						this->openglBackend->enableBlending();

						std::shared_ptr<OpenGLVertexArray> vertexArray;
						if (entityRenderable.renderableComponent->isType(Model::TYPE_STRING))
						{
							std::shared_ptr<Mesh> mesh = std::dynamic_pointer_cast<Model>(entityRenderable.renderableComponent)->mesh;
							vertexArray = this->openglBackend->getVertexArray(mesh);
						}
						else
						{
							vertexArray = this->openglBackend->getVertexArray(entityRenderable.renderableComponent->getImage());
						}
						vertexArray->bind();

						if (entityRenderable.renderableComponent->isType(Model::TYPE_STRING))
						{
							std::shared_ptr<Material> material =
								std::dynamic_pointer_cast<Model>(entityRenderable.renderableComponent)->material;

							this->openglBackend->getTexture(material->albedo)->bind(OpenGLTexture::Unit::_0);
							this->openglBackend->getTexture(material->metallicity)->bind(OpenGLTexture::Unit::_1);
							this->openglBackend->getTexture(material->roughness)->bind(OpenGLTexture::Unit::_2);
							this->openglBackend->getTexture(material->emission)->bind(OpenGLTexture::Unit::_3);
							this->openglBackend->getTexture(material->normal)->bind(OpenGLTexture::Unit::_4);
							this->openglBackend->getTexture(material->occlusion)->bind(OpenGLTexture::Unit::_5);
						}
						else
						{
							this->openglBackend->getTexture(entityRenderable.renderableComponent->getImage())->bind(OpenGLTexture::Unit::_0);
						}

						std::shared_ptr<OpenGLShaderProgram> shaderProgram;

						if (shader != nullptr)
						{
							AssetID shaderID = shader->getID();
							std::string defaultVertexInput;
							std::string defaultFragmentInput;

							if (renderableLayer.is2D)
							{
								defaultVertexInput = this->openglBackend->getDefault2DVertexInput();
								defaultFragmentInput = this->openglBackend->getDefault2DFragmentInput();
							}
							else
							{
								defaultVertexInput = this->openglBackend->getDefault3DVertexInput();
								defaultFragmentInput = this->openglBackend->getDefault3DFragmentInput();
							}

							if (!this->openglBackend->shaderPrograms.count(shaderID))
							{
								std::shared_ptr<Shader> completeShader;

								if (shader->getVertexSource() == "" && shader->getFragmentSource() == "")
								{
									completeShader = std::shared_ptr<Shader>(new Shader(
										defaultVertexInput,
										defaultFragmentInput,
										false,
										false));
								}
								else if (shader->getVertexSource() == "" && shader->getFragmentSource() != "")
								{
									completeShader = std::shared_ptr<Shader>(new Shader(
										defaultVertexInput,
										shader->getFragmentSource(),
										false,
										false));
								}
								else if (shader->getVertexSource() != "" && shader->getFragmentSource() == "")
								{
									completeShader = std::shared_ptr<Shader>(new Shader(
										shader->getVertexSource(),
										defaultFragmentInput,
										false,
										false));
								}
								else
								{
									completeShader = shader;
								}

								this->openglBackend->shaderPrograms.emplace(
									shaderID,
									std::shared_ptr<OpenGLShaderProgram>(new OpenGLShaderProgram(completeShader)));
							}

							shaderProgram = this->openglBackend->shaderPrograms.at(shaderID);
						}
						else if (!renderableLayer.is2D)
						{
							shaderProgram = this->openglBackend->builtInShaderProgram3D;
						}
						else
						{
							shaderProgram = this->openglBackend->builtInShaderProgram2D;
						}

						shaderProgram->use();

						/* Custom uniforms. */
						for (auto const& intUniformPair : shaderParameters->intUniforms)
							shaderProgram->setInt(intUniformPair.first, intUniformPair.second);

						for (auto const& boolUniformPair : shaderParameters->boolUniforms)
							shaderProgram->setBool(boolUniformPair.first, boolUniformPair.second);

						for (auto const& floatUniformPair : shaderParameters->floatUniforms)
							shaderProgram->setFloat(floatUniformPair.first, floatUniformPair.second);

						for (auto const& vec2UniformPair : shaderParameters->vec2Uniforms)
							shaderProgram->setVec2(vec2UniformPair.first, vec2UniformPair.second);

						for (auto const& vec3UniformPair : shaderParameters->vec3Uniforms)
							shaderProgram->setVec3(vec3UniformPair.first, vec3UniformPair.second);

						for (auto const& vec4UniformPair : shaderParameters->vec4Uniforms)
							shaderProgram->setVec4(vec4UniformPair.first, vec4UniformPair.second);

						for (auto const& mat4UniformPair : shaderParameters->mat4Uniforms)
							shaderProgram->setMat4(mat4UniformPair.first, mat4UniformPair.second);

						/* Standard uniforms */
						shaderProgram->setMat4("mvp", mvp);
						shaderProgram->setMat4("modelToWorldMatrix", modelToWorldMatrix);
						shaderProgram->setMat4("worldToModelMatrix", glm::inverse(modelToWorldMatrix));
						shaderProgram->setInt("albedoTextureSampler", 0);
						shaderProgram->setInt("metallicityTextureSampler", 1);
						shaderProgram->setInt("roughnessTextureSampler", 2);
						shaderProgram->setInt("emissionTextureSampler", 3);
						shaderProgram->setInt("normalTextureSampler", 4);
						shaderProgram->setInt("occlusionTextureSampler", 5);
						shaderProgram->setFloat("alpha", alpha);
						shaderProgram->setInt("lightMode", (int)light->mode);
						shaderProgram->setVec4("lightColour", light->colour->getRGBAInVec4());
						shaderProgram->setVec4("shadowColour", light->shadowColour->getRGBAInVec4());
						shaderProgram->setFloat("lightIntensity", light->intensity);
						shaderProgram->setFloat("lightAttenuation", light->attenuation);
						shaderProgram->setFloat("lightRange", light->range);
						shaderProgram->setBool("isShadowEnabled", light->isShadowEnabled);
						shaderProgram->setVec3("lightTranslation", lightTransform->translation);
						shaderProgram->setVec3("lightRotation", lightTransform->rotation);
						shaderProgram->setVec2("cameraViewport", renderableLayer.camera->getViewport());
						shaderProgram->setVec3("cameraTranslation", renderableLayer.cameraTransform->translation);
						shaderProgram->setVec3("cameraRotation", renderableLayer.cameraTransform->rotation);
						shaderProgram->setVec3("entityTranslation", entityRenderable.entityTransform->translation);
						shaderProgram->setVec3("entityRotation", entityRenderable.entityTransform->rotation);
						shaderProgram->setVec3("entityScale", entityRenderable.entityTransform->scale);

						if (vertexArray->getIndexCount() > 0)
							glDrawElements(GL_TRIANGLES, (GLsizei)vertexArray->getIndexCount(), GL_UNSIGNED_INT, 0);
						else
							glDrawArrays(GL_TRIANGLES, 0, (GLsizei)vertexArray->getVertexCount());

						vertexArray->unbind();
						this->openglBackend->disableBlending();
					}
				}
			}
		}
	}

	this->openglBackend->collectGarbage();
	this->openglBackend->swapBuffers();
}

void Omnia::RenderingSystem::finalize()
{
	if (this->isInitialized)
		SDL_QuitSubSystem(SDL_INIT_VIDEO);

	this->isInitialized = false;
}

void Omnia::RenderingSystem::onWindowResize()
{
	glm::vec2 windowSize = OS::getWindow().getWindowSize();

	if (lastDetectedWindowSize.x != windowSize.x ||
		lastDetectedWindowSize.y != windowSize.y)
	{
		int renderableLayerListSize = this->renderableLayerLists.size();

		for (int i = 0; renderableLayerListSize; i++)
		{
			std::vector<RenderableLayer> renderableLayers = this->renderableLayerLists.at(renderableLayerListSize - 1);
			int renderableLayerSize = renderableLayers.size();
			if (renderableLayerSize > 0)
			{
				RenderableLayer& renderableLayer = renderableLayers.at(renderableLayerSize - 1);

				if (renderableLayer.entityRenderables.size() > 0)
				{
					EntityRenderable& entityRenderable = renderableLayer.entityRenderables.at(0);
					if (entityRenderable.renderableComponent->isType(GUI::TYPE_STRING))
					{
						renderableLayer.camera->setViewportWidth(windowSize.x);
						renderableLayer.camera->setViewportHeight(windowSize.y);
					}
				}
			}
		}

		this->openglBackend->setViewport(windowSize.x, windowSize.y);
		lastDetectedWindowSize = windowSize;
	}
}

void Omnia::RenderingSystem::buildRenderables(std::shared_ptr<Scene> scene)
{
	this->renderableLayerLists.clear();

	std::unordered_map<SceneLayerID, std::shared_ptr<SceneLayer>>& sceneLayers = scene->getSceneLayers();
	std::vector<std::shared_ptr<RenderableComponent>> guiRenderableComponents;
	std::vector<std::shared_ptr<SceneLayer>> guiSceneLayers;

	for (auto it = sceneLayers.begin(); it != sceneLayers.end(); it++)
	{
		std::shared_ptr<SceneLayer> sceneLayer = it->second;
		std::vector<RenderableLayer> renderableLayerList;

		std::vector<std::shared_ptr<Viewport>> uiViewports = sceneLayer->getComponentsByType<Viewport>();
		std::vector<size_t> renderOrderIndexCache = sceneLayer->getRenderOrderIndexCache();

		/* One or more Viewport RenderableLayers for each SceneLayer. */
		for (int i = 0; i < uiViewports.size(); i++)
		{
			std::shared_ptr<Viewport> uiViewport = uiViewports.at(i);
			std::shared_ptr<Entity> cameraEntity = sceneLayer->getEntityByName(uiViewport->getCameraEntityName());
			std::shared_ptr<Camera> camera = sceneLayer->getComponentByType<Camera>(cameraEntity->getID());
			RenderableLayer renderableLayer;
			std::shared_ptr<Transform> cameraTransform = sceneLayer->getComponentByType<Transform>(camera->getEntityID());

			renderableLayer.is2D = sceneLayer->is2D;
			renderableLayer.camera = camera;
			renderableLayer.cameraTransform = cameraTransform;

			for (std::shared_ptr<Light> light : sceneLayer->getComponentsByType<Light>())
			{
				renderableLayer.lights.push_back(light);
				renderableLayer.lightTransforms.push_back(sceneLayer->getComponentByType<Transform>(light->getEntityID()));
			}

			/* Entity RenderableLayer for each Viewport*/
			for (int i = 0; i < renderOrderIndexCache.size(); i++)
			{
				EntityRenderable entityRenderable;
				std::shared_ptr<RenderableComponent> renderableComponent =
					std::dynamic_pointer_cast<RenderableComponent>(sceneLayer->getComponents().at(renderOrderIndexCache.at(i)));

				/* GUI Components are deferred to a final RenderableLayer, 
				   while other RenderableComponents are in the usual order. */
				if (renderableComponent->isType(GUI::TYPE_STRING))
				{
					guiRenderableComponents.push_back(renderableComponent);
					guiSceneLayers.push_back(sceneLayer);
				}
				else
				{
					std::shared_ptr<Entity> entity = sceneLayer->getEntity(renderableComponent->getEntityID());

					entityRenderable.entityTransform = sceneLayer->getComponentByType<Transform>(entity->getID());
					entityRenderable.renderableComponent = renderableComponent;

					std::shared_ptr<Entity> topEntity = entity;
					EntityID parentEntityID = entity->parentID;

					while (parentEntityID != 0)
					{
						topEntity = sceneLayer->getEntity(parentEntityID);
						parentEntityID = topEntity->parentID;
					}

					if (topEntity->renderableComponentID != 0)
					{
						std::shared_ptr<RenderableComponent> renderableComponent =
							std::dynamic_pointer_cast<RenderableComponent>(sceneLayer->getComponentByID(topEntity->renderableComponentID));

						std::shared_ptr<Shader> overridingShader = renderableComponent->getOverridingShader();

						if (overridingShader != nullptr)
						{
							entityRenderable.overridingShader = overridingShader;
							entityRenderable.overridingShaderParameters = renderableComponent->shaderParameters;
						}
					}

					renderableLayer.entityRenderables.push_back(entityRenderable);
				}
			}
			renderableLayerList.push_back(renderableLayer);
		}

		this->renderableLayerLists.push_back(renderableLayerList);
	}

	/* Put the deferred GUIs on a final RenderableLayer to be rendered last. */
	RenderableLayer renderableLayer;
	std::vector<RenderableLayer> renderableLayerList;
	std::shared_ptr<Camera> camera(new Camera());
	std::shared_ptr<Transform> cameraTransform(new Transform());
	glm::vec2 windowSize = OS::getWindow().getWindowSize();

	/* a virtual Camera for the GUI. */
	camera->setViewportWidth(windowSize.x);
	camera->setViewportHeight(windowSize.y);
	camera->setIsStreaming(true);
	camera->setWireframeMode(false);
	cameraTransform->translation.x = windowSize.x / 2.0;
	cameraTransform->translation.y = windowSize.y / 2.0;

	renderableLayer.is2D = true;
	renderableLayer.camera = camera;
	renderableLayer.cameraTransform = cameraTransform;

	for (int i = 0; i < guiRenderableComponents.size(); i++)
	{
		EntityRenderable entityRenderable;
		std::shared_ptr<RenderableComponent> guiRenderableComponent = guiRenderableComponents[i];
		std::shared_ptr<SceneLayer> guiSceneLayer = guiSceneLayers[i];
		std::shared_ptr<Entity> entity = guiSceneLayer->getEntity(guiRenderableComponent->getEntityID());

		entityRenderable.entityTransform = guiSceneLayer->getComponentByType<Transform>(entity->getID());
		entityRenderable.renderableComponent = guiRenderableComponent;
		renderableLayer.entityRenderables.push_back(entityRenderable);
	}

	/* There is only a single virtual Viewport, so one RenderableLayer. */
	renderableLayerList.push_back(renderableLayer);
	this->renderableLayerLists.push_back(renderableLayerList);
}

std::string Omnia::RenderingSystem::getRenderingBackendName()
{
	return this->openglBackend->getRenderingBackendName();
}