#include "scene_serializer.hpp"
#include <yaml-cpp/yaml.h>
#include "utilities/string.hpp"
#include "utilities/collections/set.hpp"
#include "scene/scene_forest.hpp"
#include "scene/components/component_variant.hpp"

Lilliputian::SceneSerializer::SceneSerializer(String dataDirectory)
{
	this->dataDirectory = dataDirectory;
}

Lilliputian::SceneForest Lilliputian::SceneSerializer::loadFromFile(String filepath)
{
	SceneForest scene;

	if (true)
	{
		scene = this->loadFromTextFile(filepath);
	}

	return scene;

}

Lilliputian::SceneForest Lilliputian::SceneSerializer::loadFromTextFile(String filepath)
{
	SceneForest scene;
	const String fullFilepath = this->dataDirectory + filepath;

	try
	{
		YAML::Node yamlNode = YAML::LoadFile(fullFilepath);

		for (YAML::const_iterator it0 = yamlNode.begin(); it0 != yamlNode.end(); ++it0)
		{
			if (it0->first.as<std::string>() == "SceneTree2D")
			{
				scene.incrementSceneTree2D();

				for (YAML::const_iterator it1 = it0->second.begin(); it1 != it0->second.end(); ++it1)
				{
					if (it1->first.as<std::string>() == "Entity2D")
					{
						Entity2D entity2D;
						scene.addEntity2D(entity2D);

						for (YAML::const_iterator it2 = it1->second.begin(); it2 != it1->second.end(); ++it2)
						{
							ComponentVariant componentVariant;

							//Entity attributes
							if (it2->first.as<std::string>() == "name")
							{
								scene.addNameToLastEntity(it2->second.as<std::string>());
							}
							else if (it2->first.as<std::string>() == "parent")
							{
								scene.addParentToLastEntityByName(it2->second.as<std::string>());
							}
							else if (it2->first.as<std::string>() == "position_px")
							{
								Vector2 position_px;
								position_px.x = it2->second[0].as<double>();
								position_px.y = it2->second[1].as<double>();
								scene.addPositionToEntity2D(position_px);
							}
							else if (it2->first.as<std::string>() == "rotation_rad")
							{
								scene.addRotationToEntity2D(it2->second.as<double>());
							}
							else if (it2->first.as<std::string>() == "scale")
							{
								Vector2 scale;
								scale.x = it2->second[0].as<double>();
								scale.y = it2->second[1].as<double>();
								scene.addScaleToEntity2D(scale);
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

								componentVariant.type = ComponentVariant::Type::COMPONENT_TYPE_AI_BEHAVIOUR_TREE;
								componentVariant.aiBehaviourTree = aiBehaviourTree;
								scene.addComponentToLastEntity(componentVariant);
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

								componentVariant.type = ComponentVariant::Type::COMPONENT_TYPE_CAMERA_2D;
								componentVariant.camera2D = camera2D;
								scene.addComponentToLastEntity(componentVariant);
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

								componentVariant.type = ComponentVariant::Type::COMPONENT_TYPE_COUNTDOWN_TIMER;
								componentVariant.countdownTimer = countdownTimer;
								scene.addComponentToLastEntity(componentVariant);
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
									if (it3->first.as<std::string>() == "texture")
									{
										Image image((dataDirectory + it3->second.as<std::string>()).c_str());
										Texture texture = scene.assetCache().loadTexture(image);
										sprite->setTexture(texture);
									}
								}

								componentVariant.type = ComponentVariant::Type::COMPONENT_TYPE_SPRITE;
								componentVariant.sprite = sprite;
								scene.addComponentToLastEntity(componentVariant);
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
							else if (it2->first.as<std::string>() == "StreamedAudioSource2D")
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
							else if (it2->first.as<std::string>() == "UIRichTextLabel")
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