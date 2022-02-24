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

Omnific::SceneSerializer::SceneSerializer(std::string dataDirectory)
{
	this->dataDirectory = dataDirectory;
}

bool Omnific::SceneSerializer::doesSceneExist(std::string filepath)
{
	return OS::getFileAccess().exists(this->dataDirectory + filepath);
}

void Omnific::SceneSerializer::serialize(std::string filepath, Scene scene)
{

}

Omnific::Scene Omnific::SceneSerializer::deserialize(std::string filepath)
{
	Scene scene;
	const std::string fullFilepath = this->dataDirectory + filepath;

	try
	{
		YAML::Node yamlNode = YAML::LoadFile(fullFilepath);

		for (YAML::const_iterator it0 = yamlNode.begin(); it0 != yamlNode.end(); ++it0)
		{
			if (it0->first.as<std::string>() == "Scene")
			{
				for (YAML::const_iterator it1 = it0->second.begin(); it1 != it0->second.end(); ++it1)
				{
					if (it1->first.as<std::string>() == "Entity")
					{
						Entity entity;
						scene.addEntity(entity);

						for (YAML::const_iterator it2 = it1->second.begin(); it2 != it1->second.end(); ++it2)
						{
							//Entity attributes
							if (it2->first.as<std::string>() == "name")
							{
								scene.getLastEntity().name = it2->second.as<std::string>();
							}
							else if (it2->first.as<std::string>() == "spatial_dimension")
							{
								int value = it2->second.as<int>();

								if (value == 2)
									scene.getLastEntity().spatialDimension = Entity::SpatialDimension::_2D;
							}
							else if (it2->first.as<std::string>() == "parent")
							{
								scene.getLastEntity().parentID = scene.getEntityByName(it2->second.as<std::string>()).id;
							}
							//Components
							else if (it2->first.as<std::string>() == BehaviourTree::TYPE_STRING)
							{
								std::shared_ptr<BehaviourTree> behaviourTree(new BehaviourTree());

								for (YAML::const_iterator it3 = it2->second.begin(); it3 != it2->second.end(); ++it3)
								{
									if (it3->first.as<std::string>() == "default")
									{

									}
									else if (it3->first.as<std::string>() == "")
									{

									}
								}

								std::shared_ptr<Component> component = std::static_pointer_cast<Component>(behaviourTree);
								scene.addComponentToLastEntity(component);
							}
							else if (it2->first.as<std::string>() == SightPerception::TYPE_STRING)
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
								scene.addComponentToLastEntity(component);
							}
							else if (it2->first.as<std::string>() == CharacterBody::TYPE_STRING)
							{
								std::shared_ptr<CharacterBody> characterBody(new CharacterBody());

								for (YAML::const_iterator it3 = it2->second.begin(); it3 != it2->second.end(); ++it3)
								{
									if (it3->first.as<std::string>() == "default")
									{

									}
								}

								std::shared_ptr<Component> component = std::static_pointer_cast<Component>(characterBody);
								scene.addComponentToLastEntity(component);
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
								}

								std::shared_ptr<Component> component = std::static_pointer_cast<Component>(collider);
								scene.addComponentToLastEntity(component);
							}
							else if (it2->first.as<std::string>() == ConstantForce::TYPE_STRING)
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
							else if (it2->first.as<std::string>() == CountdownTimer::TYPE_STRING)
							{
								std::shared_ptr<CountdownTimer> countdownTimer(new CountdownTimer());

								for (YAML::const_iterator it3 = it2->second.begin(); it3 != it2->second.end(); ++it3)
								{
									if (it3->first.as<std::string>() == "")
									{

									}
								}

								std::shared_ptr<Component> component = std::static_pointer_cast<Component>(countdownTimer);
								scene.addComponentToLastEntity(component);
							}
							else if (it2->first.as<std::string>() == ModelContainer::TYPE_STRING)
							{
								std::shared_ptr<ModelContainer> modelContainer(new ModelContainer());

								for (YAML::const_iterator it3 = it2->second.begin(); it3 != it2->second.end(); ++it3)
								{
									if (it3->first.as<std::string>() == "model")
									{
										std::shared_ptr<Omnific::Model> model(new Model(this->dataDirectory + it3->second.as<std::string>()));
										std::shared_ptr<Asset> asset = std::static_pointer_cast<Asset>(model);
										scene.getAssetCache().store(asset);
									}
								}

								std::shared_ptr<Component> component = std::static_pointer_cast<Component>(modelContainer);
								scene.addComponentToLastEntity(component);
							}
							else if (it2->first.as<std::string>() == NavigationMeshAgent::TYPE_STRING)
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
							else if (it2->first.as<std::string>() == NavigationMeshBoxObstacle::TYPE_STRING)
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
							else if (it2->first.as<std::string>() == NavigationPath::TYPE_STRING)
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
							else if (it2->first.as<std::string>() == RigidBody::TYPE_STRING)
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
							else if (it2->first.as<std::string>() == SpriteContainer::TYPE_STRING)
							{
								std::shared_ptr<SpriteContainer> sprite(new SpriteContainer());

								for (YAML::const_iterator it3 = it2->second.begin(); it3 != it2->second.end(); ++it3)
								{
									if (it3->first.as<std::string>() == "image")
									{
										std::shared_ptr<Omnific::Image> image(new Image(this->dataDirectory + it3->second.as<std::string>()));
										std::shared_ptr<Asset> asset = std::static_pointer_cast<Asset>(image);
										scene.getAssetCache().store(asset);
										sprite->addImage(image);
									}
									else if (it3->first.as<std::string>() == "dimensions")
									{
										sprite->setDimensions(
											it3->second[0].as<double>(),
											it3->second[1].as<double>(),
											it3->second[2].as<double>());
									}
								}

								std::shared_ptr<Component> component = std::static_pointer_cast<Component>(sprite);
								scene.addComponentToLastEntity(component);
							}
							else if (it2->first.as<std::string>() == StaticFluid::TYPE_STRING)
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
								scene.addComponentToLastEntity(component);
							}
							else if (it2->first.as<std::string>() == UIButton::TYPE_STRING)
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
							else if (it2->first.as<std::string>() == UIGraphEdit::TYPE_STRING)
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
							else if (it2->first.as<std::string>() == UIGraphNode::TYPE_STRING)
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
							else if (it2->first.as<std::string>() == UIScrollbar::TYPE_STRING)
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
							else if (it2->first.as<std::string>() == UISeparator::TYPE_STRING)
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
							else if (it2->first.as<std::string>() == UISlider::TYPE_STRING)
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
							else if (it2->first.as<std::string>() == UIHoverCard::TYPE_STRING)
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
							else if (it2->first.as<std::string>() == UIItemList::TYPE_STRING)
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
							else if (it2->first.as<std::string>() == UIPanel::TYPE_STRING)
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
							else if (it2->first.as<std::string>() == UIProgressBar::TYPE_STRING)
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
							else if (it2->first.as<std::string>() == UIRectangle::TYPE_STRING)
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
							else if (it2->first.as<std::string>() == UISpinBox::TYPE_STRING)
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
							else if (it2->first.as<std::string>() == UITab::TYPE_STRING)
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
							else if (it2->first.as<std::string>() == UITextEdit::TYPE_STRING)
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
							else if (it2->first.as<std::string>() == UITextLabel::TYPE_STRING)
							{
								std::shared_ptr<UITextLabel> uiTextLabel(new UITextLabel());

								for (YAML::const_iterator it3 = it2->second.begin(); it3 != it2->second.end(); ++it3)
								{
									if (it3->first.as<std::string>() == "text")
									{
										uiTextLabel->setText(it3->second.as<std::string>());
									}
									else if (it3->first.as<std::string>() == "font")
									{
										std::shared_ptr<Omnific::Font> font(new Font(
											this->dataDirectory + it3->second[0].as<std::string>(),
											it3->second[1].as<int>()));
										std::shared_ptr<Asset> asset = std::static_pointer_cast<Asset>(font);
										scene.getAssetCache().store(asset);
										uiTextLabel->setFont(font, it3->second[1].as<int>());
									}
									else if (it3->first.as<std::string>() == "colour")
									{
										uiTextLabel->setColour(
											it3->second[0].as<int>(),
											it3->second[1].as<int>(),
											it3->second[2].as<int>(),
											it3->second[3].as<int>()
										);
									}
								}

								std::shared_ptr<Component> component = std::static_pointer_cast<Component>(uiTextLabel);
								scene.addComponentToLastEntity(component);
							}
							else if (it2->first.as<std::string>() == UITree::TYPE_STRING)
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
							else if (it2->first.as<std::string>() == UIViewport::TYPE_STRING)
							{
								std::shared_ptr<UIViewport> uiViewport(new UIViewport());

								for (YAML::const_iterator it3 = it2->second.begin(); it3 != it2->second.end(); ++it3)
								{
									if (it3->first.as<std::string>() == "camera_entity")
									{
										uiViewport->setCameraEntity(scene.getEntityByName(it3->second.as<std::string>()).id);
									}
								}

								std::shared_ptr<Component> component = std::static_pointer_cast<Component>(uiViewport);
								scene.addComponentToLastEntity(component);
							}
							/*Non-components*/
							else if (it2->first.as<std::string>() == "Scripts")
							{
								for (int i = 0; i < it2->second.size(); i++)
								{
									std::string script = it2->second[i].as<std::string>();
									scene.getLastEntity().scripts.push_back(script);
								}
							}
						}
					}
				}
			}
		}
	}
	catch (int e)
	{

	}

	return scene;
}