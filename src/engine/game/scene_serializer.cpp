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
#include "utilities/string.hpp"
#include <set>
#include "scene/scene.hpp"
#include "scene/components/component_variant.hpp"
#include <os/os.hpp>

Lilliputian::SceneSerializer::SceneSerializer(String dataDirectory)
{
	this->dataDirectory = dataDirectory;
}

Lilliputian::Scene Lilliputian::SceneSerializer::loadFromFile(String filepath)
{
	Scene scene;

	if (true)
	{
		scene = this->loadFromTextFile(filepath);
	}

	return scene;

}

bool Lilliputian::SceneSerializer::doesSceneExist(String filepath)
{
	return OS::getFileAccess().exists(this->dataDirectory + filepath);
}

Lilliputian::Scene Lilliputian::SceneSerializer::loadFromTextFile(String filepath)
{
	Scene scene;
	const String fullFilepath = this->dataDirectory + filepath;

	try
	{
		YAML::Node yamlNode = YAML::LoadFile(fullFilepath);

		for (YAML::const_iterator it0 = yamlNode.begin(); it0 != yamlNode.end(); ++it0)
		{
			if (it0->first.as<std::string>() == "SceneTree2D")
			{
				SceneTree2D sceneTree2D;

				for (YAML::const_iterator it1 = it0->second.begin(); it1 != it0->second.end(); ++it1)
				{
					if (it1->first.as<std::string>() == "Entity2D")
					{
						Entity2D entity2D;
						sceneTree2D.addEntity2D(entity2D);

						for (YAML::const_iterator it2 = it1->second.begin(); it2 != it1->second.end(); ++it2)
						{
							ComponentVariant componentVariant;

							//Entity attributes
							if (it2->first.as<std::string>() == "name")
							{
								sceneTree2D.getLastEntity2D().name = it2->second.as<std::string>();
							}
							else if (it2->first.as<std::string>() == "parent")
							{
								sceneTree2D.getLastEntity2D().parentID = sceneTree2D.getEntity2DByName(it2->second.as<std::string>()).ID;
							}
							//Components
							else if (it2->first.as<std::string>() == "AIBehaviourTree")
							{
								AIBehaviourTree* aiBehaviourTree = new AIBehaviourTree();

								for (YAML::const_iterator it3 = it2->second.begin(); it3 != it2->second.end(); ++it3)
								{
									if (it3->first.as<std::string>() == "default")
									{

									}
									else if (it3->first.as<std::string>() == "")
									{

									}
								}

								componentVariant.setToComponent(aiBehaviourTree);
								sceneTree2D.addComponentToLastEntity(componentVariant);
							}
							else if (it2->first.as<std::string>() == "AISightPerception")
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
							else if (it2->first.as<std::string>() == "AnimatedSprite")
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
							else if (it2->first.as<std::string>() == "AudioListener2D")
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
							else if (it2->first.as<std::string>() == "AudioStreamSource2D")
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
							else if (it2->first.as<std::string>() == "Camera2D")
							{
								Camera2D* camera2D = new Camera2D();

								camera2D->setViewportHeight(480);
								camera2D->setIsStreaming(true);

								for (YAML::const_iterator it3 = it2->second.begin(); it3 != it2->second.end(); ++it3)
								{
									if (it3->first.as<std::string>() == "viewport_px")
									{
										camera2D->setViewportHeight(it3->second[1].as<double>());
										camera2D->setViewportWidth(it3->second[0].as<double>());
									}
									else if (it3->first.as<std::string>() == "limits_px")
									{
										camera2D->setLimits(
											it3->second[0].as<double>(),
											it3->second[1].as<double>(),
											it3->second[2].as<double>(),
											it3->second[3].as<double>()
										);
									}
									else if (it3->first.as<std::string>() == "keepAspect")
									{
										camera2D->setKeepAspect(it3->second.as<bool>());
									}
									else if (it3->first.as<std::string>() == "isStreaming")
									{
										camera2D->setIsStreaming(it3->second.as<bool>());
									}
								}

								componentVariant.setToComponent(camera2D);
								sceneTree2D.addComponentToLastEntity(componentVariant);
							}
							else if (it2->first.as<std::string>() == "CircleCollider2D")
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
							else if (it2->first.as<std::string>() == "ConstantDirectionalForce2D")
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
							else if (it2->first.as<std::string>() == "ConstantPointForce2D")
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
							else if (it2->first.as<std::string>() == "CountdownTimer")
							{

								CountdownTimer* countdownTimer = new CountdownTimer();

								for (YAML::const_iterator it3 = it2->second.begin(); it3 != it2->second.end(); ++it3)
								{
									if (it3->first.as<std::string>() == "")
									{

									}
								}

								componentVariant.setToComponent(countdownTimer);
								sceneTree2D.addComponentToLastEntity(componentVariant);
							}
							else if (it2->first.as<std::string>() == "FixedTransform2D")
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
							else if (it2->first.as<std::string>() == "NavigationMeshAgent2D")
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
							else if (it2->first.as<std::string>() == "NavigationMeshBoxObstacle2D")
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
							else if (it2->first.as<std::string>() == "NavigationPath2D")
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
							else if (it2->first.as<std::string>() == "NeuralNetwork")
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
							else if (it2->first.as<std::string>() == "PhysicsConstraint2D")
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
							else if (it2->first.as<std::string>() == "PhysicsThruster2D")
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
							else if (it2->first.as<std::string>() == "PropertyAnimation")
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
							else if (it2->first.as<std::string>() == "RectangularCollider2D")
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
							else if (it2->first.as<std::string>() == "RectangularMesh2D")
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
							else if (it2->first.as<std::string>() == "RectangularTriggerArea2D")
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
							else if (it2->first.as<std::string>() == "RegularPolygonalMesh2D")
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
							else if (it2->first.as<std::string>() == "RigidBody2D")
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
							else if (it2->first.as<std::string>() == "Sprite")
							{
								Sprite* sprite = new Sprite();

								for (YAML::const_iterator it3 = it2->second.begin(); it3 != it2->second.end(); ++it3)
								{
									if (it3->first.as<std::string>() == "image")
									{
										Image image = scene.getAssetCache().loadImage((this->dataDirectory + it3->second.as<std::string>()).c_str());
										sprite->setImage(image);
									}
								}

								componentVariant.setToComponent(sprite);
								sceneTree2D.addComponentToLastEntity(componentVariant);
							}
							else if (it2->first.as<std::string>() == "StaticFluid2D")
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
							else if (it2->first.as<std::string>() == "Transform2D")
							{
								Transform2D* transform2D = new Transform2D();

								for (YAML::const_iterator it3 = it2->second.begin(); it3 != it2->second.end(); ++it3)
								{
									if (it3->first.as<std::string>() == "position_px")
									{
										transform2D->position_px.x = it3->second[0].as<double>();
										transform2D->position_px.y = it3->second[1].as<double>();
									}
									else if (it3->first.as<std::string>() == "rotation_rad")
									{
										transform2D->rotation_rad = it3->second.as<double>();
									}
									else if (it3->first.as<std::string>() == "scale")
									{
										transform2D->scale.x = it3->second[0].as<double>();
										transform2D->scale.y = it3->second[1].as<double>();
									}
								}

								componentVariant.setToComponent(transform2D);
								sceneTree2D.addComponentToLastEntity(componentVariant);
							}
							else if (it2->first.as<std::string>() == "UIButton")
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
							else if (it2->first.as<std::string>() == "UIColouredRectangle")
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
							else if (it2->first.as<std::string>() == "UIGraphEdit")
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
							else if (it2->first.as<std::string>() == "UIGraphNode")
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
							else if (it2->first.as<std::string>() == "UIHorizontalScrollbar")
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
							else if (it2->first.as<std::string>() == "UIHorizontalSeparator")
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
							else if (it2->first.as<std::string>() == "UIHorizontalSlider")
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
							else if (it2->first.as<std::string>() == "UIHoverCard")
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
							else if (it2->first.as<std::string>() == "UIItemList")
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
							else if (it2->first.as<std::string>() == "UILinkButton")
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
							else if (it2->first.as<std::string>() == "UIPanel")
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
							else if (it2->first.as<std::string>() == "UIProgressBar")
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
							else if (it2->first.as<std::string>() == "UISpinBox")
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
							else if (it2->first.as<std::string>() == "UITab")
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
							else if (it2->first.as<std::string>() == "UITextEdit")
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
							else if (it2->first.as<std::string>() == "UITextLabel")
							{
								UITextLabel* uiTextLabel = new UITextLabel();

								for (YAML::const_iterator it3 = it2->second.begin(); it3 != it2->second.end(); ++it3)
								{
									if (it3->first.as<std::string>() == "text")
									{
										uiTextLabel->setText(it3->second.as<std::string>());
									}
									else if (it3->first.as<std::string>() == "font")
									{
										Font font = scene.getAssetCache().loadFont((
											this->dataDirectory + it3->second[0].as<std::string>()).c_str(),
											it3->second[1].as<int>());

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

								componentVariant.setToComponent(uiTextLabel);
								sceneTree2D.addComponentToLastEntity(componentVariant);
							}
							else if (it2->first.as<std::string>() == "UITexturedButton")
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
							else if (it2->first.as<std::string>() == "UITexturedProgressBar")
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
							else if (it2->first.as<std::string>() == "UITexturedRectangle")
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
							else if (it2->first.as<std::string>() == "UITree")
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
							else if (it2->first.as<std::string>() == "UIVerticalScrollbar")
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
							else if (it2->first.as<std::string>() == "UIVerticalSeparator")
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
							else if (it2->first.as<std::string>() == "UIVerticalSlider")
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
							else if (it2->first.as<std::string>() == "Scripts")
							{
								for (int i = 0; i < it2->second.size(); i++)
								{
									String script = it2->second[i].as<std::string>();
									sceneTree2D.getLastEntity2D().scripts.push_back(script);
								}
							}
						}
					}
				}

				scene.addSceneTree2D(sceneTree2D);
			}
		}
	}
	catch (int e)
	{

	}

	return scene;
}