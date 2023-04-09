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

#include "opengl_rendering_backend.hpp"
#include "opengl_built_in_shaders.hpp"
#include <core/singletons/os/os.hpp>
#include <string>
#include <SDL_video.h>

#include <core/components/model.hpp>

void Omnia::OpenGLRenderingBackend::initialize()
{
	Window& window = OS::getWindow();
	window.initializeWindowContext("opengl");

	if ((!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)))
	{
		OS::getLogger().write("GLAD failed to initialize.");
	}
	else
	{
		this->builtInShaderProgram2D = std::shared_ptr<OpenGLShaderProgram>(new OpenGLShaderProgram(std::shared_ptr<Shader>(new Shader(
			OpenGLBuiltInShaders::Vertex::dimension_2, 
			OpenGLBuiltInShaders::Fragment::dimension_2, 
			false,
			false))));

		this->builtInShaderProgram3D = std::shared_ptr<OpenGLShaderProgram>(new OpenGLShaderProgram(std::shared_ptr<Shader>(new Shader(
			OpenGLBuiltInShaders::Vertex::dimension_3,
			OpenGLBuiltInShaders::Fragment::dimension_3,
			false,
			false))));

		glm::vec2 windowSize = window.getWindowSize();
		this->setViewport(windowSize.x, windowSize.y);
		OS::getLogger().write((std::string)("Rendering Backend initialized with ") +
			"OpenGL " + (char*)glGetString(GL_VERSION));
	}
}

void Omnia::OpenGLRenderingBackend::clearColourBuffer(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
{
	glClearColor(
		(float)(red) / 255.0, 
		(float)(green) / 255.0, 
		(float)(blue) / 255.0, 
		(float)(alpha) / 255.0);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Omnia::OpenGLRenderingBackend::clearDepthBuffer()
{
	glClear(GL_DEPTH_BUFFER_BIT);
}

void Omnia::OpenGLRenderingBackend::enableDepthTest()
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
}

void Omnia::OpenGLRenderingBackend::disableDepthTest()
{
	glDisable(GL_DEPTH_TEST);
}

void Omnia::OpenGLRenderingBackend::enableWireframeMode()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void Omnia::OpenGLRenderingBackend::disableWireframeMode()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Omnia::OpenGLRenderingBackend::enableBlending()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Omnia::OpenGLRenderingBackend::disableBlending()
{
	glDisable(GL_BLEND);
}

void Omnia::OpenGLRenderingBackend::submit(std::map<SceneLayerID, std::vector<SceneLayerRenderable>> sceneLayerRenderableLists)
{

	for (auto it = sceneLayerRenderableLists.begin(); it != sceneLayerRenderableLists.end(); it++)
	{

		std::vector<SceneLayerRenderable> sceneLayerRenderableList = it->second;
		SceneLayerRenderable* sceneLayerRenderableListData = sceneLayerRenderableList.data();
		size_t sceneLayerRenderableListCount = sceneLayerRenderableList.size();

		for (size_t i = 0; i < sceneLayerRenderableListCount; i++)
		{

			SceneLayerRenderable& sceneLayerRenderable = sceneLayerRenderableListData[i];

			if (sceneLayerRenderable.camera->getIsStreaming())
			{
				EntityRenderable* entityRenderablesData = sceneLayerRenderable.entityRenderables.data();
				size_t entityRenderablesCount = sceneLayerRenderable.entityRenderables.size();
				glm::mat4 worldToViewMatrix = glm::inverse(sceneLayerRenderable.cameraTransform->getTransformMatrix());
				glm::mat4 viewToProjectionMatrix = sceneLayerRenderable.camera->getViewToProjectionMatrix();

				if (sceneLayerRenderable.is2D)
				{
					this->disableDepthTest();
				}
				else
				{
					this->clearDepthBuffer();
					this->enableDepthTest();
				}

				if (sceneLayerRenderable.camera->getIsWireframeMode())
					this->enableWireframeMode();
				else
					this->disableWireframeMode();

				/* Memory allocated out of the tight loop. */
				std::shared_ptr<Transform> globalTransform(new Transform());

				std::vector<std::shared_ptr<Light>> activeLights;
				std::vector<std::shared_ptr<Transform>> activeLightTransforms;

				if (sceneLayerRenderable.lights.size() > 0)
				{
					activeLights = sceneLayerRenderable.lights;
					activeLightTransforms = sceneLayerRenderable.lightTransforms;
				}
				else
				{
					activeLights.push_back(this->dummyLight);
					activeLightTransforms.push_back(this->dummyLightTransform);
					
				}

				size_t lightsCount = activeLights.size();

				/* Forward Rendering */
				for (size_t j = 0; j < lightsCount; j++)
				{
					std::shared_ptr<Light> light = activeLights[j];
					std::shared_ptr<Transform> lightTransform = activeLightTransforms[j];

					for (size_t k = 0; k < entityRenderablesCount; k++)
					{
						EntityRenderable& entityRenderable = entityRenderablesData[k];
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

						if (alpha < cullAlphaThreshold || cullMode == RenderableComponent::CullMode::NONE)
						{
							glDisable(GL_CULL_FACE);
							if (alpha < cullAlphaThreshold)
								this->enableBlending();
						}
						else
						{
							glEnable(GL_CULL_FACE);
							this->disableBlending();
						}

						std::shared_ptr<OpenGLVertexArray> vertexArray = this->getVertexArray(entityRenderable.renderableComponent);
						vertexArray->bind();

						if (entityRenderable.renderableComponent->isType(Model::TYPE_STRING))
						{
							std::shared_ptr<Material> material =
								std::dynamic_pointer_cast<Model>(entityRenderable.renderableComponent)->material;

							this->getTexture(material->albedo)->bind(OpenGLTexture::Unit::_0);
							this->getTexture(material->metallicity)->bind(OpenGLTexture::Unit::_1);
							this->getTexture(material->roughness)->bind(OpenGLTexture::Unit::_2);
							this->getTexture(material->emission)->bind(OpenGLTexture::Unit::_3);
							this->getTexture(material->normal)->bind(OpenGLTexture::Unit::_4);
							this->getTexture(material->occlusion)->bind(OpenGLTexture::Unit::_5);
						}
						else
						{
							this->getTexture(entityRenderable.renderableComponent->getImage())->bind(OpenGLTexture::Unit::_0);
						}

						std::shared_ptr<OpenGLShaderProgram> shaderProgram;

						if (shader != nullptr)
						{
							AssetID shaderID = shader->getID();

							if (!this->shaderPrograms.count(shaderID))
							{
								std::shared_ptr<Shader> completeShader;

								if (shader->getVertexSource() == "" && shader->getFragmentSource() == "")
								{
									completeShader = std::shared_ptr<Shader>(new Shader(
										OpenGLBuiltInShaders::Vertex::dimension_3,
										OpenGLBuiltInShaders::Fragment::dimension_3,
										false,
										false));
								}
								else if (shader->getVertexSource() == "" && shader->getFragmentSource() != "")
								{
									completeShader = std::shared_ptr<Shader>(new Shader(
										OpenGLBuiltInShaders::Vertex::dimension_3,
										shader->getFragmentSource(),
										false,
										false));
								}
								else if (shader->getVertexSource() != "" && shader->getFragmentSource() == "")
								{
									completeShader = std::shared_ptr<Shader>(new Shader(
										shader->getVertexSource(),
										OpenGLBuiltInShaders::Fragment::dimension_3,
										false,
										false));
								}
								else
								{
									completeShader = shader;
								}

								this->shaderPrograms.emplace(
									shaderID,
									std::shared_ptr<OpenGLShaderProgram>(new OpenGLShaderProgram(completeShader)));
							}

							shaderProgram = this->shaderPrograms.at(shaderID);
						}
						else if (!sceneLayerRenderable.is2D)
						{
							shaderProgram = this->builtInShaderProgram3D;
						}
						else
						{
							shaderProgram = this->builtInShaderProgram2D;
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
						shaderProgram->setVec2("cameraViewport", sceneLayerRenderable.camera->getViewport());
						shaderProgram->setVec3("cameraTranslation", sceneLayerRenderable.cameraTransform->translation);
						shaderProgram->setVec3("cameraRotation", sceneLayerRenderable.cameraTransform->rotation);
						shaderProgram->setVec3("entityTranslation", entityRenderable.entityTransform->translation);
						shaderProgram->setVec3("entityRotation", entityRenderable.entityTransform->rotation);

						if (vertexArray->getIndexCount() > 0)
							glDrawElements(GL_TRIANGLES, (GLsizei)vertexArray->getIndexCount(), GL_UNSIGNED_INT, 0);
						else
							glDrawArrays(GL_TRIANGLES, 0, (GLsizei)vertexArray->getVertexCount());

						vertexArray->unbind();
					}
				}
			}
		}
	}

	this->collectGarbage();
}

void Omnia::OpenGLRenderingBackend::setViewport(uint32_t width, uint32_t height)
{
	glViewport(0, 0, width, height);
}

void Omnia::OpenGLRenderingBackend::swapBuffers()
{
	SDL_GL_SwapWindow(OS::getWindow().getSDLWindow());
}

std::string Omnia::OpenGLRenderingBackend::getRenderingBackendName()
{
	return "opengl";
}

std::shared_ptr<Omnia::OpenGLTexture> Omnia::OpenGLRenderingBackend::getTexture(std::shared_ptr<Image> image)
{
	std::shared_ptr<OpenGLTexture> texture;

	if (image != nullptr)
	{
		AssetID assetID = image->getID();

		if (this->textures.count(assetID) == 0)
		{
			this->textures.emplace(assetID, std::shared_ptr<OpenGLTexture>(new OpenGLTexture(image)));
			this->missedFrameCounts.emplace(assetID, 0);
		}
		else
		{
			this->missedFrameCounts.at(assetID) = 0;
		}

		texture = this->textures.at(assetID);
	}
	else
	{
		texture = std::shared_ptr<OpenGLTexture>(new OpenGLTexture());
	}

	return texture;
}

std::shared_ptr<Omnia::OpenGLVertexArray> Omnia::OpenGLRenderingBackend::getVertexArray(std::shared_ptr<RenderableComponent> renderableComponent)
{
	AssetID assetID;

	if (renderableComponent->isType(Model::TYPE_STRING))
	{
		std::shared_ptr<Mesh> mesh = std::dynamic_pointer_cast<Model>(renderableComponent)->mesh;
		if (mesh != nullptr)
		{
			assetID = mesh->getID();
		}
	}
	else
	{
		std::shared_ptr<Image> image = renderableComponent->getImage();
		if (image != nullptr)
		{
			assetID = image->getID();
		}
	}

	if (this->vertexArrays.count(assetID) == 0)
	{
		this->vertexArrays.emplace(assetID, std::shared_ptr<OpenGLVertexArray>(new OpenGLVertexArray(renderableComponent)));
		this->missedFrameCounts.emplace(assetID, 0);
	}
	else
	{
		this->missedFrameCounts.at(assetID) = 0;
	}
	return this->vertexArrays.at(assetID);
}

void Omnia::OpenGLRenderingBackend::collectGarbage()
{
	std::vector<AssetID> assetsToDelete;

	for (auto it = this->missedFrameCounts.begin(); it != this->missedFrameCounts.end(); it++)
	{
		if (this->missedFrameCounts.at(it->first) > this->allowableMissedFrames)
		{
			if (this->textures.count(it->first) > 0)
				this->textures.erase(it->first);
			if (this->vertexArrays.count(it->first) > 0)
				this->vertexArrays.erase(it->first);

			assetsToDelete.push_back(it->first);
		}
	}

	for (size_t i = 0; i < assetsToDelete.size(); i++)
		this->missedFrameCounts.erase(assetsToDelete.at(i));

	for (auto it = this->missedFrameCounts.begin(); it != this->missedFrameCounts.end(); it++)
		this->missedFrameCounts.at(it->first)++;
}