#include "scene_tree_2d.hpp"

void Lilliputian::SceneTree2D::addEntity2D(Entity2D entity2D)
{
	this->entities2D.emplace(entity2D.id, entity2D);
}

void Lilliputian::SceneTree2D::addComponent(EntityID entityID, ComponentVariant componentVariant)
{
	componentVariant.entityID = entityID;

	switch (componentVariant.type)
	{
		case Component::ComponentType::COMPONENT_TYPE_AI_BEHAVIOUR_TREE:
			this->aiBehaviourTrees.push_back(componentVariant.aiBehaviourTree);
			this->entities2D.at(entityID).addComponent(Component::ComponentType::COMPONENT_TYPE_AI_BEHAVIOUR_TREE,
				this->aiBehaviourTrees.size() - 1);
			break;
		case Component::ComponentType::COMPONENT_TYPE_AI_SIGHT_PERCEPTION:
			this->aiSightPerceptions2D.push_back(componentVariant.aiSightPerception2D);
			this->entities2D.at(entityID).addComponent(Component::ComponentType::COMPONENT_TYPE_AI_SIGHT_PERCEPTION,
				this->aiSightPerceptions2D.size() - 1);
			break;
		case Component::ComponentType::COMPONENT_TYPE_AI_SOUND_PERCEPTION:
			this->aiSoundPerceptions2D.push_back(componentVariant.aiSoundPerception2D);
			this->entities2D.at(entityID).addComponent(Component::ComponentType::COMPONENT_TYPE_AI_SOUND_PERCEPTION,
				this->aiSoundPerceptions2D.size() - 1);
			break;
		case Component::ComponentType::COMPONENT_TYPE_ANIMATED_SPRITE_2D:
			this->animatedSprites2D.push_back(componentVariant.animatedSprite2D);
			this->entities2D.at(entityID).addComponent(Component::ComponentType::COMPONENT_TYPE_ANIMATED_SPRITE_2D,
				this->animatedSprites2D.size() - 1);
			break;
		case Component::ComponentType::COMPONENT_TYPE_AUDIO_LISTENER_2D:
				this->audioListeners2D.push_back(componentVariant.audioListener2D);
				this->entities2D.at(entityID).addComponent(Component::ComponentType::COMPONENT_TYPE_AUDIO_LISTENER_2D, 
					this->audioListeners2D.size() - 1);
				break;
		case Component::ComponentType::COMPONENT_TYPE_CAMERA_2D:
			this->cameras2D.push_back(componentVariant.camera2D);
			this->entities2D.at(entityID).addComponent(Component::ComponentType::COMPONENT_TYPE_CAMERA_2D,
				this->cameras2D.size() - 1);
			break;
		case Component::ComponentType::COMPONENT_TYPE_CIRCLE_COLLIDER_2D:
			this->circleColliders2D.push_back(componentVariant.circleCollider2D);
			this->entities2D.at(entityID).addComponent(Component::ComponentType::COMPONENT_TYPE_CIRCLE_COLLIDER_2D,
				this->circleColliders2D.size() - 1);
			break;
		case Component::ComponentType::COMPONENT_TYPE_CONSTANT_DIRECTIONAL_FORCE_2D:
			this->constantDirectionalForces2D.push_back(componentVariant.constantDirectionalForce2D);
			this->entities2D.at(entityID).addComponent(Component::ComponentType::COMPONENT_TYPE_CONSTANT_DIRECTIONAL_FORCE_2D,
				this->constantDirectionalForces2D.size() - 1);
			break;
		case Component::ComponentType::COMPONENT_TYPE_CONSTANT_POINT_FORCE_2D:
			this->constantPointForces2D.push_back(componentVariant.constantPointForce2D);
			this->entities2D.at(entityID).addComponent(Component::ComponentType::COMPONENT_TYPE_CONSTANT_POINT_FORCE_2D,
				this->constantPointForces2D.size() - 1);
			break;
		case Component::ComponentType::COMPONENT_TYPE_COUNTDOWN_TIMER:
			this->countdownTimers.push_back(componentVariant.countdownTimer);
			this->entities2D.at(entityID).addComponent(Component::ComponentType::COMPONENT_TYPE_COUNTDOWN_TIMER,
				this->countdownTimers.size() - 1);
			break;
		case Component::ComponentType::COMPONENT_TYPE_FIXED_TRANSFORM_2D:
			this->fixedTransforms2D.push_back(componentVariant.fixedTransform2D);
			this->entities2D.at(entityID).addComponent(Component::ComponentType::COMPONENT_TYPE_FIXED_TRANSFORM_2D,
				this->fixedTransforms2D.size() - 1);
			break;
		case Component::ComponentType::COMPONENT_TYPE_NAVIGATION_MESH_2D:
			this->navigationMeshAgents2D.push_back(componentVariant.navigationMeshAgent2D);
			this->entities2D.at(entityID).addComponent(Component::ComponentType::COMPONENT_TYPE_NAVIGATION_MESH_2D,
				this->navigationMeshAgents2D.size() - 1);
			break;
		case Component::ComponentType::COMPONENT_TYPE_NAVIGATION_MESH_AGENT_2D:
			this->navigationMeshAgents2D.push_back(componentVariant.navigationMeshAgent2D);
			this->entities2D.at(entityID).addComponent(Component::ComponentType::COMPONENT_TYPE_NAVIGATION_MESH_AGENT_2D,
				this->navigationMeshAgents2D.size() - 1);
			break;
		case Component::ComponentType::COMPONENT_TYPE_NAVIGATION_MESH_BOX_OBSTACLE_2D:
			this->navigationMeshBoxObstacles2D.push_back(componentVariant.navigationMeshBoxObstacle2D);
			this->entities2D.at(entityID).addComponent(Component::ComponentType::COMPONENT_TYPE_NAVIGATION_MESH_BOX_OBSTACLE_2D,
				this->navigationMeshBoxObstacles2D.size() - 1);
			break;
		case Component::ComponentType::COMPONENT_TYPE_NAVIGATION_PATH_2D:
			this->navigationPaths2D.push_back(componentVariant.navigationPath2D);
			this->entities2D.at(entityID).addComponent(Component::ComponentType::COMPONENT_TYPE_NAVIGATION_PATH_2D,
				this->navigationPaths2D.size() - 1);
			break;
		case Component::ComponentType::COMPONENT_TYPE_NEURAL_NETWORK:
			this->neuralNetworks.push_back(componentVariant.neuralNetwork);
			this->entities2D.at(entityID).addComponent(Component::ComponentType::COMPONENT_TYPE_NEURAL_NETWORK,
				this->neuralNetworks.size() - 1);
			break;
		case Component::ComponentType::COMPONENT_TYPE_PHYSICS_CONSTRAINT_2D:
			this->physicsConstraints2D.push_back(componentVariant.physicsConstraint2D);
			this->entities2D.at(entityID).addComponent(Component::ComponentType::COMPONENT_TYPE_PHYSICS_CONSTRAINT_2D,
				this->physicsConstraints2D.size() - 1);
			break;
		case Component::ComponentType::COMPONENT_TYPE_PHYSICS_THRUSTER_2D:
			this->physicsThrusters2D.push_back(componentVariant.physicsThruster2D);
			this->entities2D.at(entityID).addComponent(Component::ComponentType::COMPONENT_TYPE_PHYSICS_THRUSTER_2D,
				this->physicsThrusters2D.size() - 1);
			break;
		case Component::ComponentType::COMPONENT_TYPE_PROPERTY_ANIMATION:
			this->propertyAnimations.push_back(componentVariant.propertyAnimation);
			this->entities2D.at(entityID).addComponent(Component::ComponentType::COMPONENT_TYPE_PROPERTY_ANIMATION,
				this->propertyAnimations.size() - 1);
			break;
		case Component::ComponentType::COMPONENT_TYPE_RECTANGULAR_COLIIDER_2D:
			this->rectangularColliders2D.push_back(componentVariant.rectangularCollider2D);
			this->entities2D.at(entityID).addComponent(Component::ComponentType::COMPONENT_TYPE_RECTANGULAR_COLIIDER_2D,
				this->rectangularColliders2D.size() - 1);
			break;
		case Component::ComponentType::COMPONENT_TYPE_RECTANGULAR_MESH_2D:
			this->rectangularMeshes2D.push_back(componentVariant.rectangularMesh2D);
			this->entities2D.at(entityID).addComponent(Component::ComponentType::COMPONENT_TYPE_RECTANGULAR_MESH_2D,
				this->rectangularMeshes2D.size() - 1);
			break;
		case Component::ComponentType::COMPONENT_TYPE_RECTANGULAR_TRIGGER_AREA_2D:
			this->rectangularTriggerAreas2D.push_back(componentVariant.rectangularTriggerArea2D);
			this->entities2D.at(entityID).addComponent(Component::ComponentType::COMPONENT_TYPE_RECTANGULAR_TRIGGER_AREA_2D,
				this->rectangularTriggerAreas2D.size() - 1);
			break;
		case Component::ComponentType::COMPONENT_TYPE_REGULAR_POLYGONAL_MESH_2D:
			this->regularPolygonalMeshes2D.push_back(componentVariant.regularPolygonalMesh2D);
			this->entities2D.at(entityID).addComponent(Component::ComponentType::COMPONENT_TYPE_REGULAR_POLYGONAL_MESH_2D,
				this->regularPolygonalMeshes2D.size() - 1);
			break;
		case Component::ComponentType::COMPONENT_TYPE_RIGID_BODY_2D:
			this->rigidBodies2D.push_back(componentVariant.rigidBody2D);
			this->entities2D.at(entityID).addComponent(Component::ComponentType::COMPONENT_TYPE_RIGID_BODY_2D,
				this->rigidBodies2D.size() - 1);
			break;
		case Component::ComponentType::COMPONENT_TYPE_SPRITE:
			this->sprites.push_back(componentVariant.sprite);
			this->entities2D.at(entityID).addComponent(Component::ComponentType::COMPONENT_TYPE_SPRITE,
				this->sprites.size() - 1);
			break;
		case Component::ComponentType::COMPONENT_TYPE_STATIC_FLUID_2D:
			this->staticFluids2D.push_back(componentVariant.staticFluid2D);
			this->entities2D.at(entityID).addComponent(Component::ComponentType::COMPONENT_TYPE_STATIC_FLUID_2D,
				this->staticFluids2D.size() - 1);
			break;
		case Component::ComponentType::COMPONENT_TYPE_STREAMED_AUDIO_SOURCE_2D:
			this->streamedAudioSources2D.push_back(componentVariant.streamedAudioSource2D);
			this->entities2D.at(entityID).addComponent(Component::ComponentType::COMPONENT_TYPE_STREAMED_AUDIO_SOURCE_2D,
				this->streamedAudioSources2D.size() - 1);
			break;
		case Component::ComponentType::COMPONENT_TYPE_UI_BUTTON:
			this->uiButtons.push_back(componentVariant.uiButton);
			this->entities2D.at(entityID).addComponent(Component::ComponentType::COMPONENT_TYPE_UI_BUTTON,
				this->uiButtons.size() - 1);
			break;
		case Component::ComponentType::COMPONENT_TYPE_UI_COLOURED_RECTANGLE:
			this->uiColouredRectangles.push_back(componentVariant.uiColouredRectangle);
			this->entities2D.at(entityID).addComponent(Component::ComponentType::COMPONENT_TYPE_UI_COLOURED_RECTANGLE,
				this->uiColouredRectangles.size() - 1);
			break;
		case Component::ComponentType::COMPONENT_TYPE_UI_GRAPH_EDIT:
			this->uiGraphEdits.push_back(componentVariant.uiGraphEdit);
			this->entities2D.at(entityID).addComponent(Component::ComponentType::COMPONENT_TYPE_UI_GRAPH_EDIT,
				this->uiGraphEdits.size() - 1);
			break;
		case Component::ComponentType::COMPONENT_TYPE_UI_GRAPH_NODE:
			this->uiGraphNodes.push_back(componentVariant.uiGraphNode);
			this->entities2D.at(entityID).addComponent(Component::ComponentType::COMPONENT_TYPE_UI_GRAPH_NODE,
				this->uiGraphNodes.size() - 1);
			break;
		case Component::ComponentType::COMPONENT_TYPE_UI_HORIZONTAL_SCROLLBAR:
			this->uiHorizontalScrollbars.push_back(componentVariant.uiHorizontalScrollbar);
			this->entities2D.at(entityID).addComponent(Component::ComponentType::COMPONENT_TYPE_UI_HORIZONTAL_SCROLLBAR,
				this->uiHorizontalScrollbars.size() - 1);
			break;
		case Component::ComponentType::COMPONENT_TYPE_UI_HORIZONTAL_SEPARATOR:
			this->uiHorizontalSeparators.push_back(componentVariant.uiHorizontalSeparator);
			this->entities2D.at(entityID).addComponent(Component::ComponentType::COMPONENT_TYPE_UI_HORIZONTAL_SEPARATOR,
				this->uiHorizontalSeparators.size() - 1);
			break;
		case Component::ComponentType::COMPONENT_TYPE_UI_HORIZONTAL_SLIDER:
			this->uiHorizontalSliders.push_back(componentVariant.uiHorizontalSlider);
			this->entities2D.at(entityID).addComponent(Component::ComponentType::COMPONENT_TYPE_UI_HORIZONTAL_SLIDER,
				this->uiHorizontalSliders.size() - 1);
			break;
		case Component::ComponentType::COMPONENT_TYPE_UI_HOVER_CARD:
			this->uiHoverCards.push_back(componentVariant.uiHoverCard);
			this->entities2D.at(entityID).addComponent(Component::ComponentType::COMPONENT_TYPE_UI_HOVER_CARD,
				this->uiHoverCards.size() - 1);
			break;
		case Component::ComponentType::COMPONENT_TYPE_UI_ITEM_LIST:
			this->uiItemLists.push_back(componentVariant.uiItemList);
			this->entities2D.at(entityID).addComponent(Component::ComponentType::COMPONENT_TYPE_UI_ITEM_LIST,
				this->uiItemLists.size() - 1);
			break;
		case Component::ComponentType::COMPONENT_TYPE_UI_LINK_BUTTON:
			this->uiLinkButtons.push_back(componentVariant.uiLinkButton);
			this->entities2D.at(entityID).addComponent(Component::ComponentType::COMPONENT_TYPE_UI_LINK_BUTTON,
				this->uiLinkButtons.size() - 1);
			break;
		case Component::ComponentType::COMPONENT_TYPE_UI_PANEL:
			this->uiPanels.push_back(componentVariant.uiPanel);
			this->entities2D.at(entityID).addComponent(Component::ComponentType::COMPONENT_TYPE_UI_PANEL,
				this->uiPanels.size() - 1);
			break;
		case Component::ComponentType::COMPONENT_TYPE_UI_PROGRESS_BAR:
			this->uiProgressBars.push_back(componentVariant.uiProgressBar);
			this->entities2D.at(entityID).addComponent(Component::ComponentType::COMPONENT_TYPE_UI_PROGRESS_BAR,
				this->uiProgressBars.size() - 1);
			break;
		case Component::ComponentType::COMPONENT_TYPE_UI_RICH_TEXT_LABEL:
			this->uiRichTextLabels.push_back(componentVariant.uiRichTextLabel);
			this->entities2D.at(entityID).addComponent(Component::ComponentType::COMPONENT_TYPE_UI_RICH_TEXT_LABEL,
				this->uiRichTextLabels.size() - 1);
			break;
		case Component::ComponentType::COMPONENT_TYPE_UI_SPIN_BOX:
			this->uiSpinBoxes.push_back(componentVariant.uiSpinBox);
			this->entities2D.at(entityID).addComponent(Component::ComponentType::COMPONENT_TYPE_UI_SPIN_BOX,
				this->uiSpinBoxes.size() - 1);
			break;
		case Component::ComponentType::COMPONENT_TYPE_UI_TAB:
			this->uiTabs.push_back(componentVariant.uiTab);
			this->entities2D.at(entityID).addComponent(Component::ComponentType::COMPONENT_TYPE_UI_TAB,
				this->uiTabs.size() - 1);
			break;
		case Component::ComponentType::COMPONENT_TYPE_UI_TEXT_EDIT:
			this->uiTextEdits.push_back(componentVariant.uiTextEdit);
			this->entities2D.at(entityID).addComponent(Component::ComponentType::COMPONENT_TYPE_UI_TEXT_EDIT,
				this->uiTextEdits.size() - 1);
			break;
		case Component::ComponentType::COMPONENT_TYPE_UI_TEXT_LABEL:
			this->uiTextLabels.push_back(componentVariant.uiTextLabel);
			this->entities2D.at(entityID).addComponent(Component::ComponentType::COMPONENT_TYPE_UI_TEXT_LABEL,
				this->uiTextLabels.size() - 1);
			break;
		case Component::ComponentType::COMPONENT_TYPE_UI_TEXTURED_BUTTON:
			this->uiTexturedButtons.push_back(componentVariant.uiTexturedButton);
			this->entities2D.at(entityID).addComponent(Component::ComponentType::COMPONENT_TYPE_UI_TEXTURED_BUTTON,
				this->uiTexturedButtons.size() - 1);
			break;
		case Component::ComponentType::COMPONENT_TYPE_UI_TEXTURED_PROGRESS_BAR:
			this->uiTexturedProgressBars.push_back(componentVariant.uiTexturedProgressBar);
			this->entities2D.at(entityID).addComponent(Component::ComponentType::COMPONENT_TYPE_UI_TEXTURED_PROGRESS_BAR,
				this->uiTexturedProgressBars.size() - 1);
			break;
		case Component::ComponentType::COMPONENT_TYPE_UI_TEXTURED_RECTANGLE:
			this->uiTexturedRectangles.push_back(componentVariant.uiTexturedRectangle);
			this->entities2D.at(entityID).addComponent(Component::ComponentType::COMPONENT_TYPE_UI_TEXTURED_RECTANGLE,
				this->uiTexturedRectangles.size() - 1);
			break;
		case Component::ComponentType::COMPONENT_TYPE_UI_TREE:
			this->uiTrees.push_back(componentVariant.uiTree);
			this->entities2D.at(entityID).addComponent(Component::ComponentType::COMPONENT_TYPE_UI_TREE,
				this->uiTrees.size() - 1);
			break;
		case Component::ComponentType::COMPONENT_TYPE_UI_VERTICAL_SCROLLBAR:
			this->uiVerticalScrollbars.push_back(componentVariant.uiVerticalScrollbar);
			this->entities2D.at(entityID).addComponent(Component::ComponentType::COMPONENT_TYPE_UI_VERTICAL_SCROLLBAR,
				this->uiVerticalScrollbars.size() - 1);
			break;
		case Component::ComponentType::COMPONENT_TYPE_UI_VERTICAL_SEPARATOR:
			this->uiVerticalSeparators.push_back(componentVariant.uiVerticalSeparator);
			this->entities2D.at(entityID).addComponent(Component::ComponentType::COMPONENT_TYPE_UI_VERTICAL_SEPARATOR,
				this->uiVerticalSeparators.size() - 1);
			break;
		case Component::ComponentType::COMPONENT_TYPE_UI_VERTICAL_SLIDER:
			this->uiVerticalSliders.push_back(componentVariant.uiVerticalSlider);
			this->entities2D.at(entityID).addComponent(Component::ComponentType::COMPONENT_TYPE_UI_VERTICAL_SLIDER,
				this->uiVerticalSliders.size() - 1);
			break;
	}
}

void Lilliputian::SceneTree2D::executeFrameLogic()
{
	for (int i = 0; i < this->entities2D.size(); i++)
	{

	}
}