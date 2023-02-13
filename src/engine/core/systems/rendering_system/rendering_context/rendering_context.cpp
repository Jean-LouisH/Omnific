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

#include "rendering_context.hpp"
#include "built_in_shaders.hpp"
#include <core/singletons/os/os.hpp>
#include <string>
#include <SDL_video.h>

#include <core/components/model.hpp>

void Omnia::RenderingContext::initialize()
{
	Window& window = OS::getWindow();
	this->glContext = SDL_GL_CreateContext(window.getSDLWindow());

	if (!gladLoadGL())
	{
		OS::getLogger().write("GLAD failed to initialize.");
	}
	else
	{
		this->builtInShaderProgram2D = std::shared_ptr<ShaderProgram>(new ShaderProgram(std::shared_ptr<Shader>(new Shader(
			BuiltInShaders::Vertex::dimension_2, 
			BuiltInShaders::Fragment::dimension_2, 
			false,
			false))));

		this->builtInShaderProgram3D = std::shared_ptr<ShaderProgram>(new ShaderProgram(std::shared_ptr<Shader>(new Shader(
			BuiltInShaders::Vertex::dimension_3,
			BuiltInShaders::Fragment::dimension_3,
			false,
			false))));

		Rectangle windowDimensions = window.getWindowSize();
		this->setViewport(windowDimensions.width, windowDimensions.height);
		OS::getLogger().write((std::string)("Rendering Context initialized with ") +
			"OpenGL " + (char*)glGetString(GL_VERSION));
	}
}

void Omnia::RenderingContext::clearColourBuffer(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
{
	glClearColor(
		(float)(red) / 255.0, 
		(float)(green) / 255.0, 
		(float)(blue) / 255.0, 
		(float)(alpha) / 255.0);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Omnia::RenderingContext::clearDepthBuffer()
{
	glClear(GL_DEPTH_BUFFER_BIT);
}

void Omnia::RenderingContext::enableDepthTest()
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
}

void Omnia::RenderingContext::disableDepthTest()
{
	glDisable(GL_DEPTH_TEST);
}

void Omnia::RenderingContext::enableWireframeMode()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void Omnia::RenderingContext::disableWireframeMode()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Omnia::RenderingContext::enableBlending()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Omnia::RenderingContext::disableBlending()
{
	glDisable(GL_BLEND);
}

void Omnia::RenderingContext::submit(std::map<SceneTreeID, std::vector<SceneTreeRenderable>> sceneTreeRenderableLists)
{

	for (auto it = sceneTreeRenderableLists.begin(); it != sceneTreeRenderableLists.end(); it++)
	{

		std::vector<SceneTreeRenderable> sceneTreeRenderableList = it->second;
		SceneTreeRenderable* sceneTreeRenderableListData = sceneTreeRenderableList.data();
		size_t sceneTreeRenderableListCount = sceneTreeRenderableList.size();

		for (size_t i = 0; i < sceneTreeRenderableListCount; i++)
		{

			SceneTreeRenderable& sceneTreeRenderable = sceneTreeRenderableListData[i];

			if (sceneTreeRenderable.camera->getIsStreaming())
			{
				EntityRenderable* entityRenderablesData = sceneTreeRenderable.entityRenderables.data();
				size_t entityRenderablesCount = sceneTreeRenderable.entityRenderables.size();
				glm::mat4 worldToViewMatrix = glm::inverse(sceneTreeRenderable.cameraTransform->getTransformMatrix());
				glm::mat4 viewToProjectionMatrix = sceneTreeRenderable.camera->getViewToProjectionMatrix();

				if (sceneTreeRenderable.is2D)
				{
					this->disableDepthTest();
				}
				else
				{
					this->clearDepthBuffer();
					this->enableDepthTest();
				}

				if (sceneTreeRenderable.camera->getIsWireframeMode())
					this->enableWireframeMode();
				else
					this->disableWireframeMode();

				/* Memory allocated out of the tight loop. */
				std::shared_ptr<Transform> globalTransform(new Transform());

				for (size_t j = 0; j < entityRenderablesCount; j++)
				{

					EntityRenderable& entityRenderable = entityRenderablesData[j];
					std::shared_ptr<Shader> shader = entityRenderable.overridingShader;
					
					if (shader == nullptr)
						shader = entityRenderable.renderableComponent->getShader();

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

					std::shared_ptr<VertexArray> vertexArray = this->getVertexArray(entityRenderable.renderableComponent);
					vertexArray->bind();

					if (entityRenderable.renderableComponent->isType(Model::TYPE_STRING))
					{
						std::shared_ptr<Material> material = 
							std::dynamic_pointer_cast<Model>(entityRenderable.renderableComponent)->material;

						this->getTexture(material->albedo)->bind(Texture::Unit::_0);
						this->getTexture(material->metallicity)->bind(Texture::Unit::_1);
						this->getTexture(material->roughness)->bind(Texture::Unit::_2);
						this->getTexture(material->emission)->bind(Texture::Unit::_3);
						this->getTexture(material->normal)->bind(Texture::Unit::_4);
					}
					else
					{
						this->getTexture(entityRenderable.renderableComponent->getImage())->bind(Texture::Unit::_0);
					}

					std::shared_ptr<ShaderProgram> shaderProgram;

					if (shader != nullptr)
					{
						AssetID shaderID = shader->getID();

						if (!this->shaderPrograms.count(shaderID))
						{
							std::shared_ptr<Shader> completeShader; 

							if (shader->getVertexSource() == "" && shader->getFragmentSource() == "")
							{
								completeShader = std::shared_ptr<Shader>(new Shader(
									BuiltInShaders::Vertex::dimension_3,
									BuiltInShaders::Fragment::dimension_3,
									false,
									false));
							}
							else if (shader->getVertexSource() == "" && shader->getFragmentSource() != "")
							{
								completeShader = std::shared_ptr<Shader>(new Shader(
									BuiltInShaders::Vertex::dimension_3,
									shader->getFragmentSource(),
									false,
									false));
							}
							else if (shader->getVertexSource() != "" && shader->getFragmentSource() == "")
							{
								completeShader = std::shared_ptr<Shader>(new Shader(
									shader->getVertexSource(),
									BuiltInShaders::Fragment::dimension_3,
									false,
									false));
							}
							else
							{
								completeShader = shader;
							}

							this->shaderPrograms.emplace(
								shaderID,
								std::shared_ptr<ShaderProgram>(new ShaderProgram(completeShader)));
						}

						shaderProgram = this->shaderPrograms.at(shaderID);
					}
					else if (!sceneTreeRenderable.is2D)
					{
						shaderProgram = this->builtInShaderProgram3D;
					}
					else 
					{
						shaderProgram = this->builtInShaderProgram2D;
					}

					shaderProgram->use();

					/* Standard uniforms */
					shaderProgram->setMat4("mvp", mvp);
					shaderProgram->setInt("albedoTextureSampler", 0);
					shaderProgram->setInt("metallicityTextureSampler", 1);
					shaderProgram->setInt("roughnessTextureSampler", 2);
					shaderProgram->setInt("emissionTextureSampler", 3);
					shaderProgram->setInt("normalTextureSampler", 4);
					shaderProgram->setFloat("alpha", alpha);

					shaderProgram->setVec2("cameraViewport", glm::vec2());
					shaderProgram->setVec3("cameraPosition", glm::vec3());
					shaderProgram->setVec3("cameraRotation", glm::vec3());

					//ToDo: detect uniforms in Shaders and allow setters for them.

					if (vertexArray->getIndexCount() > 0)
						glDrawElements(GL_TRIANGLES, (GLsizei)vertexArray->getIndexCount(), GL_UNSIGNED_INT, 0);
					else
						glDrawArrays(GL_TRIANGLES, 0, (GLsizei)vertexArray->getVertexCount());

					vertexArray->unbind();
				}
			}
		}
	}

	this->collectGarbage();
}

void Omnia::RenderingContext::setViewport(uint32_t width, uint32_t height)
{
	glViewport(0, 0, width, height);
}

void Omnia::RenderingContext::swapBuffers()
{
	SDL_GL_SwapWindow(OS::getWindow().getSDLWindow());
}

std::string Omnia::RenderingContext::getRenderingContextName()
{
	return "opengl";
}

std::shared_ptr<Omnia::Texture> Omnia::RenderingContext::getTexture(std::shared_ptr<Image> image)
{
	std::shared_ptr<Texture> texture;

	if (image != nullptr)
	{
		AssetID assetID = image->getID();

		if (this->textures.count(assetID) == 0)
		{
			this->textures.emplace(assetID, std::shared_ptr<Texture>(new Texture(image)));
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
		texture = std::shared_ptr<Texture>(new Texture());
	}

	return texture;
}

std::shared_ptr<Omnia::VertexArray> Omnia::RenderingContext::getVertexArray(std::shared_ptr<RenderableComponent> renderableComponent)
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
		this->vertexArrays.emplace(assetID, std::shared_ptr<VertexArray>(new VertexArray(renderableComponent)));
		this->missedFrameCounts.emplace(assetID, 0);
	}
	else
	{
		this->missedFrameCounts.at(assetID) = 0;
	}
	return this->vertexArrays.at(assetID);
}

void Omnia::RenderingContext::collectGarbage()
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