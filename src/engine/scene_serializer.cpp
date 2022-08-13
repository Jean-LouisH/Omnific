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

#include "scene_serializer.hpp"
#include <yaml-cpp/yaml.h>
#include <string>
#include <set>
#include "scene/scene.hpp"
#include "scene/component.hpp"
#include <os/os.hpp>
#include "scene/asset_cache.hpp"

Omnia::SceneSerializer::SceneSerializer(std::string dataDirectory)
{
	this->dataDirectory = dataDirectory;
}

bool Omnia::SceneSerializer::doesSceneExist(std::string filepath)
{
	return OS::getFileAccess().exists(this->dataDirectory + filepath);
}

void Omnia::SceneSerializer::serialize(std::string filepath, std::shared_ptr<Scene> scene)
{

}

std::shared_ptr<Omnia::Scene> Omnia::SceneSerializer::deserialize(std::string filepath)
{
	return this->deserialize(filepath, "");
}

std::shared_ptr<Omnia::Scene> Omnia::SceneSerializer::deserialize(std::string filepath, std::string name)
{
	std::shared_ptr<Scene> scene = std::shared_ptr<Scene>(new Scene());
	const std::string fullFilepath = this->dataDirectory + filepath;

	try
	{
		YAML::Node yamlNode = YAML::LoadFile(fullFilepath);

		for (YAML::const_iterator it0 = yamlNode.begin(); it0 != yamlNode.end(); ++it0)
		{
			if (it0->first.as<std::string>() == "SceneTree")
			{
				std::shared_ptr<SceneTree> sceneTree(new SceneTree());
				bool loadThisSceneTree = true;

				/* If the name is an empty string, load all, otherwise search for the name */
				if (name != "")
				{
					loadThisSceneTree = false;

					for (YAML::const_iterator it1 = it0->second.begin(); it1 != it0->second.end(); ++it1)
					{
						if (it1->first.as<std::string>() == "name")
						{
							loadThisSceneTree = it1->second.as<std::string>() == name;
							break;
						}
					}
				}

				if (loadThisSceneTree)
				{
					for (YAML::const_iterator it1 = it0->second.begin(); it1 != it0->second.end(); ++it1)
					{
						if (it1->first.as<std::string>() == "name")
						{
							sceneTree->name = it1->second.as<std::string>();
						}
						else if (it1->first.as<std::string>() == "spatial_dimension")
						{
							int value = it1->second.as<int>();

							if (value == 2)
								sceneTree->is2D = true;
							else if (value == 3)
								sceneTree->is2D = false;
						}
						else if (it1->first.as<std::string>() == "Entity")
						{
							std::shared_ptr<Entity> entity(new Entity());
							sceneTree->addEntity(entity);

							for (YAML::const_iterator it2 = it1->second.begin(); it2 != it1->second.end(); ++it2)
							{
								//Entity attributes
								if (it2->first.as<std::string>() == "name")
								{
									sceneTree->setEntityName(sceneTree->getLastEntity()->getID(), it2->second.as<std::string>());
								}
								else if (it2->first.as<std::string>() == "parent")
								{
									sceneTree->getLastEntity()->parentID = sceneTree->getEntityByName(it2->second.as<std::string>())->getID();
								}
								//Components
								else if (it2->first.as<std::string>() == AudioListener::TYPE_STRING)
								{
									for (YAML::const_iterator it3 = it2->second.begin(); it3 != it2->second.end(); ++it3)
									{
										if (it3->first.as<std::string>() == "default")
										{

										}
										else if (it3->first.as<std::string>() == "")
										{

										}
									}
								}
								else if (it2->first.as<std::string>() == AudioSource::TYPE_STRING)
								{
									for (YAML::const_iterator it3 = it2->second.begin(); it3 != it2->second.end(); ++it3)
									{
										if (it3->first.as<std::string>() == "default")
										{

										}
										else if (it3->first.as<std::string>() == "")
										{

										}
									}
								}
								else if (it2->first.as<std::string>() == Camera::TYPE_STRING)
								{
									std::shared_ptr<Camera> camera(new Camera());

									camera->setViewportHeight(480);
									camera->setIsStreaming(true);

									for (YAML::const_iterator it3 = it2->second.begin(); it3 != it2->second.end(); ++it3)
									{
										if (it3->first.as<std::string>() == "viewport_px")
										{
											camera->setViewportHeight(it3->second[1].as<double>());
											camera->setViewportWidth(it3->second[0].as<double>());
										}
										else if (it3->first.as<std::string>() == "limits_px")
										{
											camera->setLimits(
												it3->second[0].as<double>(),
												it3->second[1].as<double>(),
												it3->second[2].as<double>(),
												it3->second[3].as<double>()
											);
										}
										else if (it3->first.as<std::string>() == "keepAspect")
										{
											camera->setKeepAspect(it3->second.as<bool>());
										}
										else if (it3->first.as<std::string>() == "isStreaming")
										{
											camera->setIsStreaming(it3->second.as<bool>());
										}
									}

									std::shared_ptr<Component> component = std::static_pointer_cast<Component>(camera);
									sceneTree->addComponentToLastEntity(component);
								}
								else if (it2->first.as<std::string>() == PhysicsBody::TYPE_STRING)
								{
									std::shared_ptr<PhysicsBody> characterBody(new PhysicsBody());

									for (YAML::const_iterator it3 = it2->second.begin(); it3 != it2->second.end(); ++it3)
									{
										if (it3->first.as<std::string>() == "default")
										{

										}
									}

									std::shared_ptr<Component> component = std::static_pointer_cast<Component>(characterBody);
									sceneTree->addComponentToLastEntity(component);
								}
								else if (it2->first.as<std::string>() == Collider::TYPE_STRING)
								{
									std::shared_ptr<Collider> collider(new Collider());

									for (YAML::const_iterator it3 = it2->second.begin(); it3 != it2->second.end(); ++it3)
									{
										if (it3->first.as<std::string>() == "default")
										{

										}
										else if (it3->first.as<std::string>() == "dimensions")
										{
											collider->setDimensions(
												it3->second[0].as<double>(),
												it3->second[1].as<double>(),
												it3->second[2].as<double>());
										}
										else if (it3->first.as<std::string>() == "is_trigger")
										{
											collider->isTrigger = it3->second.as<bool>();
										}
									}

									std::shared_ptr<Component> component = std::static_pointer_cast<Component>(collider);
									sceneTree->addComponentToLastEntity(component);
								}
								else if (it2->first.as<std::string>() == Timer::TYPE_STRING)
								{
									std::shared_ptr<Timer> countdownTimer(new Timer());

									for (YAML::const_iterator it3 = it2->second.begin(); it3 != it2->second.end(); ++it3)
									{
										if (it3->first.as<std::string>() == "")
										{

										}
									}

									std::shared_ptr<Component> component = std::static_pointer_cast<Component>(countdownTimer);
									sceneTree->addComponentToLastEntity(component);
								}
								else if (it2->first.as<std::string>() == ModelContainer::TYPE_STRING)
								{
									std::shared_ptr<ModelContainer> modelContainer(new ModelContainer());

									for (YAML::const_iterator it3 = it2->second.begin(); it3 != it2->second.end(); ++it3)
									{
										if (it3->first.as<std::string>() == "models")
										{
											for (int i = 0; i < it3->second.size(); i++)
											{
												if (it3->second[i].as<std::string>() == "Model::cube")
												{
													modelContainer->addCube();
												}
												else
												{
													std::shared_ptr<Omnia::Model> model(new Model(this->dataDirectory + it3->second[i].as<std::string>()));
													std::shared_ptr<Asset> asset = std::static_pointer_cast<Asset>(model);
													AssetCache::store(asset);
													modelContainer->addModel(model);
												}
											}
										}
									}

									std::shared_ptr<Component> component = std::static_pointer_cast<Component>(modelContainer);
									sceneTree->addComponentToLastEntity(component);
								}
								else if (it2->first.as<std::string>() == PropertyAnimation::TYPE_STRING)
								{
									for (YAML::const_iterator it3 = it2->second.begin(); it3 != it2->second.end(); ++it3)
									{
										if (it3->first.as<std::string>() == "default")
										{

										}
										else if (it3->first.as<std::string>() == "")
										{

										}
									}
								}
								else if (it2->first.as<std::string>() == PhysicsBody::TYPE_STRING)
								{
									for (YAML::const_iterator it3 = it2->second.begin(); it3 != it2->second.end(); ++it3)
									{
										if (it3->first.as<std::string>() == "default")
										{

										}
										else if (it3->first.as<std::string>() == "")
										{

										}
									}
								}
								else if (it2->first.as<std::string>() == ScriptCollection::TYPE_STRING)
								{
									std::shared_ptr<ScriptCollection> scriptCollection(new ScriptCollection());

										for (YAML::const_iterator it3 = it2->second.begin(); it3 != it2->second.end(); ++it3)
										{
											if (it3->first.as<std::string>() == "python")
											{
												for (int i = 0; i < it3->second.size(); i++)
												{
													std::shared_ptr<Omnia::PythonScript> pythonScript(new PythonScript(it3->second[i].as<std::string>()));
													AssetCache::store(std::static_pointer_cast<Asset>(pythonScript));
													scriptCollection->scripts.push_back(pythonScript);
												}
											}
											else if (it3->first.as<std::string>() == "cpp")
											{
												for (int i = 0; i < it3->second.size(); i++)
												{
													std::shared_ptr<Omnia::CPPScript> cppScript(new CPPScript(it3->second[i].as<std::string>()));
													AssetCache::store(std::static_pointer_cast<Asset>(cppScript));
													scriptCollection->scripts.push_back(cppScript);
												}
											}
										}

									sceneTree->addComponentToLastEntity(std::static_pointer_cast<Component>(scriptCollection));
								}
								else if (it2->first.as<std::string>() == SpriteContainer::TYPE_STRING)
								{
									std::shared_ptr<SpriteContainer> sprite(new SpriteContainer());

									for (YAML::const_iterator it3 = it2->second.begin(); it3 != it2->second.end(); ++it3)
									{
										if (it3->first.as<std::string>() == "image")
										{
											if (it3->second.as<std::string>() == "Image::default")
											{
												sprite->addImage(std::shared_ptr<Image>(new Image(it3->second.as<std::string>())));
											}
											else
											{
												std::shared_ptr<Omnia::Image> image(new Image(this->dataDirectory + it3->second.as<std::string>()));
												std::shared_ptr<Asset> asset = std::static_pointer_cast<Asset>(image);
												AssetCache::store(asset);
												sprite->addImage(image);
											}
										}
										else if (it3->first.as<std::string>() == "dimensions")
										{
											sprite->setDimensions(
												it3->second[0].as<double>(),
												it3->second[1].as<double>(),
												it3->second[2].as<double>());
										}
										else if (it3->first.as<std::string>() == "alpha")
										{
											sprite->setAlpha((uint8_t)(it3->second.as<double>() * 255.0));
										}
									}

									std::shared_ptr<Component> component = std::static_pointer_cast<Component>(sprite);
									sceneTree->addComponentToLastEntity(component);
								}
								else if (it2->first.as<std::string>() == Transform::TYPE_STRING)
								{
									std::shared_ptr<Transform> transform(new Transform());

									for (YAML::const_iterator it3 = it2->second.begin(); it3 != it2->second.end(); ++it3)
									{
										if (it3->first.as<std::string>() == "translation")
										{
											transform->translation.x = it3->second[0].as<double>();
											transform->translation.y = it3->second[1].as<double>();
											transform->translation.z = it3->second[2].as<double>();
										}
										else if (it3->first.as<std::string>() == "rotation")
										{
											transform->rotation.x = it3->second[0].as<double>();
											transform->rotation.y = it3->second[1].as<double>();
											transform->rotation.z = it3->second[2].as<double>();
										}
										else if (it3->first.as<std::string>() == "scale")
										{
											transform->scale.x = it3->second[0].as<double>();
											transform->scale.y = it3->second[1].as<double>();
											transform->scale.z = it3->second[2].as<double>();
										}
									}

									std::shared_ptr<Component> component = std::static_pointer_cast<Component>(transform);
									sceneTree->addComponentToLastEntity(component);
								}
								else if (it2->first.as<std::string>() == GUI::TYPE_STRING)
								{
									std::shared_ptr<GUI> gui(new GUI());

									for (YAML::const_iterator it3 = it2->second.begin(); it3 != it2->second.end(); ++it3)
									{
										//if (it3->first.as<std::string>() == "text")
										//{
										//	gui->setText(it3->second.as<std::string>());
										//}
										//else if (it3->first.as<std::string>() == "font")
										//{
										//	std::shared_ptr<Omnia::Font> font(new Font(
										//		this->dataDirectory + it3->second[0].as<std::string>(),
										//		it3->second[1].as<int>()));
										//	std::shared_ptr<Asset> asset = std::static_pointer_cast<Asset>(font);
										//	AssetCache::store(asset);
										//	gui->setFont(font, it3->second[1].as<int>());
										//}
										//else if (it3->first.as<std::string>() == "colour")
										//{
										//	gui->setColour(
										//		it3->second[0].as<int>(),
										//		it3->second[1].as<int>(),
										//		it3->second[2].as<int>(),
										//		it3->second[3].as<int>()
										//	);
										//}
									}

									std::shared_ptr<Component> component = std::static_pointer_cast<Component>(gui);
									sceneTree->addComponentToLastEntity(component);
								}
								else if (it2->first.as<std::string>() == Viewport::TYPE_STRING)
								{
									std::shared_ptr<Viewport> uiViewport(new Viewport());

									for (YAML::const_iterator it3 = it2->second.begin(); it3 != it2->second.end(); ++it3)
									{
										if (it3->first.as<std::string>() == "camera_entity")
										{
											uiViewport->setCameraEntity(sceneTree->getEntityByName(it3->second.as<std::string>())->getID());
										}
									}

									std::shared_ptr<Component> component = std::static_pointer_cast<Component>(uiViewport);
									sceneTree->addComponentToLastEntity(component);
								}
							}
						}
						/* Recursively load another SceneTree into this one if the filename
						   is not the same. */
						else if (it1->first.as<std::string>() == "SubSceneTree")
						{
							EntityID parentID = 0;

							for (YAML::const_iterator it2 = it1->second.begin(); it2 != it1->second.end(); ++it2)
							{
								if (it2->first.as<std::string>() == "parent")
								{
									parentID = sceneTree->getEntityByName(it2->second.as<std::string>())->getID();
								}
								else if (it2->first.as<std::string>() == "name")
								{
									std::string subSceneFilepath = it1->second[0].as<std::string>();
									std::string subSceneTreeName = it1->second[1].as<std::string>();

									if (subSceneFilepath != filepath)
									{
										std::shared_ptr<SceneTree> subSceneTree = this->deserialize(subSceneFilepath, subSceneTreeName)->getLastSceneTree();

										/* Only load the SceneTree if it is the same spatial dimension. */
										if (subSceneTree->is2D == sceneTree->is2D)
										{
											/* Transfer Entities and their Components */
											std::shared_ptr<Entity> newRootEntity;
											newRootEntity->setName(subSceneFilepath);
											newRootEntity->parentID = parentID;
											sceneTree->addEntity(newRootEntity);
											std::unordered_map<EntityID, std::shared_ptr<Entity>>& subSceneEntities = subSceneTree->getEntities();

											for (auto it = subSceneEntities.begin(); it != subSceneEntities.end(); it++)
											{
												std::shared_ptr<Entity> subSceneEntity = it->second;

												if (subSceneEntity->parentID == 0)
													subSceneEntity->parentID = newRootEntity->getID();

												sceneTree->addEntity(subSceneEntity);

												std::unordered_map<std::string, ComponentID> subSceneEntityComponentIDs = subSceneEntity->componentIDs;

												for (auto it2 = subSceneEntityComponentIDs.begin(); it2 != subSceneEntityComponentIDs.end(); it2++)
													sceneTree->addComponentToLastEntity(subSceneTree->getComponent(it2->second));
											}
										}
									}
								}
							}
						}
					}
				}

				scene->addSceneTree(sceneTree);
			}
		}
	}
	catch (int e)
	{

	}

	return scene;
}