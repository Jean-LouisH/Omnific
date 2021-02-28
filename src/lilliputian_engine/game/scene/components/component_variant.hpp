#pragma once

#include "component.hpp"
#include "utilities/aliases.hpp"

#include "ai_behaviour_tree.hpp"
#include "ai_sight_perception.hpp"
#include "ai_sound_perception.hpp"
#include "animated_sprite.hpp"
#include "audio_listener_2d.hpp"
#include "camera_2d.hpp"
#include "circle_collider_2d.hpp"
#include "constant_directional_force_2d.hpp"
#include "constant_point_force_2d.hpp"
#include "countdown_timer.hpp"
#include "fixed_transform_2d.hpp"
#include "navigation_mesh_agent_2d.hpp"
#include "navigation_mesh_box_obstacle_2d.hpp"
#include "navigation_path_2d.hpp"
#include "neural_network.hpp"
#include "physics_constraint_2d.hpp"
#include "physics_thruster_2d.hpp"
#include "property_animation.hpp"
#include "rectangular_collider_2d.hpp"
#include "rectangular_mesh_2d.hpp"
#include "rectangular_trigger_area_2d.hpp"
#include "regular_polygonal_mesh_2d.hpp"
#include "rigid_body_2d.hpp"
#include "sprite.hpp"
#include "static_fluid_2d.hpp"
#include "streamed_audio_source_2d.hpp"
#include "ui_button.hpp"
#include "ui_coloured_rectangle.hpp"
#include "ui_graph_edit.hpp"
#include "ui_graph_node.hpp"
#include "ui_horizontal_scrollbar.hpp"
#include "ui_horizontal_separator.hpp"
#include "ui_horizontal_slider.hpp"
#include "ui_hover_card.hpp"
#include "ui_item_list.hpp"
#include "ui_link_button.hpp"
#include "ui_panel.hpp"
#include "ui_progress_bar.hpp"
#include "ui_rich_text_label.hpp"
#include "ui_spin_box.hpp"
#include "ui_tab.hpp"
#include "ui_text_edit.hpp"
#include "ui_text_label.hpp"
#include "ui_textured_button.hpp"
#include "ui_textured_progress_bar.hpp"
#include "ui_textured_rectangle.hpp"
#include "ui_tree.hpp"
#include "ui_vertical_scrollbar.hpp"
#include "ui_vertical_separator.hpp"
#include "ui_vertical_slider.hpp"

namespace Lilliputian
{
    struct ComponentVariant
    {
		enum Type
		{
			COMPONENT_TYPE_NONE,
			COMPONENT_TYPE_AI_BEHAVIOUR_TREE,
			COMPONENT_TYPE_AI_SIGHT_PERCEPTION,
			COMPONENT_TYPE_AI_SOUND_PERCEPTION,
			COMPONENT_TYPE_ANIMATED_SPRITE,
			COMPONENT_TYPE_AUDIO_LISTENER_2D,
			COMPONENT_TYPE_CAMERA_2D,
			COMPONENT_TYPE_CIRCLE_COLLIDER_2D,
			COMPONENT_TYPE_CONSTANT_DIRECTIONAL_FORCE_2D,
			COMPONENT_TYPE_CONSTANT_POINT_FORCE_2D,
			COMPONENT_TYPE_COUNTDOWN_TIMER,
			COMPONENT_TYPE_FIXED_TRANSFORM_2D,
			COMPONENT_TYPE_NAVIGATION_MESH_2D,
			COMPONENT_TYPE_NAVIGATION_MESH_AGENT_2D,
			COMPONENT_TYPE_NAVIGATION_MESH_BOX_OBSTACLE_2D,
			COMPONENT_TYPE_NAVIGATION_PATH_2D,
			COMPONENT_TYPE_NEURAL_NETWORK,
			COMPONENT_TYPE_PHYSICS_CONSTRAINT_2D,
			COMPONENT_TYPE_PHYSICS_THRUSTER_2D,
			COMPONENT_TYPE_PROPERTY_ANIMATION,
			COMPONENT_TYPE_RECTANGULAR_COLIIDER_2D,
			COMPONENT_TYPE_RECTANGULAR_MESH_2D,
			COMPONENT_TYPE_RECTANGULAR_TRIGGER_AREA_2D,
			COMPONENT_TYPE_REGULAR_POLYGONAL_MESH_2D,
			COMPONENT_TYPE_RIGID_BODY_2D,
			COMPONENT_TYPE_SPRITE,
			COMPONENT_TYPE_STATIC_FLUID_2D,
			COMPONENT_TYPE_STREAMED_AUDIO_SOURCE_2D,
			COMPONENT_TYPE_UI_BUTTON,
			COMPONENT_TYPE_UI_COLOURED_RECTANGLE,
			COMPONENT_TYPE_UI_GRAPH_EDIT,
			COMPONENT_TYPE_UI_GRAPH_NODE,
			COMPONENT_TYPE_UI_HORIZONTAL_SCROLLBAR,
			COMPONENT_TYPE_UI_HORIZONTAL_SEPARATOR,
			COMPONENT_TYPE_UI_HORIZONTAL_SLIDER,
			COMPONENT_TYPE_UI_HOVER_CARD,
			COMPONENT_TYPE_UI_ITEM_LIST,
			COMPONENT_TYPE_UI_LINK_BUTTON,
			COMPONENT_TYPE_UI_PANEL,
			COMPONENT_TYPE_UI_PROGRESS_BAR,
			COMPONENT_TYPE_UI_RICH_TEXT_LABEL,
			COMPONENT_TYPE_UI_SPIN_BOX,
			COMPONENT_TYPE_UI_TAB,
			COMPONENT_TYPE_UI_TEXT_EDIT,
			COMPONENT_TYPE_UI_TEXT_LABEL,
			COMPONENT_TYPE_UI_TEXTURED_BUTTON,
			COMPONENT_TYPE_UI_TEXTURED_PROGRESS_BAR,
			COMPONENT_TYPE_UI_TEXTURED_RECTANGLE,
			COMPONENT_TYPE_UI_TREE,
			COMPONENT_TYPE_UI_VERTICAL_SCROLLBAR,
			COMPONENT_TYPE_UI_VERTICAL_SEPARATOR,
			COMPONENT_TYPE_UI_VERTICAL_SLIDER
		};


        Type type = Type::COMPONENT_TYPE_NONE;
		EntityID entityID = NO_ENTITY;

		ComponentVariant()
		{

		}
		ComponentVariant(const ComponentVariant& other)
		{
			this->type = other.type;
			this->entityID = other.entityID;

			switch (other.type)
			{
				case COMPONENT_TYPE_NONE: break;
				case COMPONENT_TYPE_AI_BEHAVIOUR_TREE: this->aiBehaviourTree = new AIBehaviourTree(*other.aiBehaviourTree); break;
				case COMPONENT_TYPE_AI_SIGHT_PERCEPTION: this->aiSightPerception2D = new AISightPerception2D(*other.aiSightPerception2D); break;
				case COMPONENT_TYPE_AI_SOUND_PERCEPTION: this->aiSoundPerception2D = new AISoundPerception2D(*other.aiSoundPerception2D); break;
				case COMPONENT_TYPE_ANIMATED_SPRITE:this->animatedSprite = new AnimatedSprite(*other.animatedSprite); break;
				case COMPONENT_TYPE_AUDIO_LISTENER_2D:this->audioListener2D = new AudioListener2D(*other.audioListener2D); break;
				case COMPONENT_TYPE_CAMERA_2D: this->camera2D = new Camera2D(*other.camera2D); break;
				case COMPONENT_TYPE_CIRCLE_COLLIDER_2D:this->circleCollider2D = new CircleCollider2D(*other.circleCollider2D); break;
				case COMPONENT_TYPE_CONSTANT_DIRECTIONAL_FORCE_2D:this->constantDirectionalForce2D = new ConstantDirectionalForce2D(*other.constantDirectionalForce2D); break;
				case COMPONENT_TYPE_CONSTANT_POINT_FORCE_2D:this->constantPointForce2D = new ConstantPointForce2D(*other.constantPointForce2D); break;
				case COMPONENT_TYPE_COUNTDOWN_TIMER:this->countdownTimer = new CountdownTimer(*other.countdownTimer); break;
				case COMPONENT_TYPE_FIXED_TRANSFORM_2D:this->fixedTransform2D = new FixedTransform2D(*other.fixedTransform2D); break;
				case COMPONENT_TYPE_NAVIGATION_MESH_AGENT_2D:this->navigationMeshAgent2D = new NavigationMeshAgent2D(*other.navigationMeshAgent2D); break;
				case COMPONENT_TYPE_NAVIGATION_MESH_BOX_OBSTACLE_2D:this->navigationMeshBoxObstacle2D = new NavigationMeshBoxObstacle2D(*other.navigationMeshBoxObstacle2D); break;
				case COMPONENT_TYPE_NAVIGATION_PATH_2D:this->navigationPath2D = new NavigationPath2D(*other.navigationPath2D); break;
				case COMPONENT_TYPE_NEURAL_NETWORK:this->neuralNetwork = new NeuralNetwork(*other.neuralNetwork); break;
				case COMPONENT_TYPE_PHYSICS_CONSTRAINT_2D:this->physicsConstraint2D = new PhysicsConstraint2D(*other.physicsConstraint2D); break;
				case COMPONENT_TYPE_PHYSICS_THRUSTER_2D:this->physicsThruster2D = new PhysicsThruster2D(*other.physicsThruster2D); break;
				case COMPONENT_TYPE_PROPERTY_ANIMATION:this->propertyAnimation = new PropertyAnimation(*other.propertyAnimation); break;
				case COMPONENT_TYPE_RECTANGULAR_COLIIDER_2D:this->rectangularCollider2D = new RectangularCollider2D(*other.rectangularCollider2D); break;
				case COMPONENT_TYPE_RECTANGULAR_MESH_2D:this->rectangularMesh2D = new RectangularMesh2D(*other.rectangularMesh2D); break;
				case COMPONENT_TYPE_RECTANGULAR_TRIGGER_AREA_2D:this->rectangularTriggerArea2D = new RectangularTriggerArea2D(*other.rectangularTriggerArea2D); break;
				case COMPONENT_TYPE_REGULAR_POLYGONAL_MESH_2D:this->regularPolygonalMesh2D = new RegularPolygonalMesh2D(*other.regularPolygonalMesh2D); break;
				case COMPONENT_TYPE_RIGID_BODY_2D:this->rigidBody2D = new RigidBody2D(*other.rigidBody2D); break;
				case COMPONENT_TYPE_SPRITE:this->sprite = new Sprite(*other.sprite); break;
				case COMPONENT_TYPE_STATIC_FLUID_2D:this->staticFluid2D = new StaticFluid2D(*other.staticFluid2D); break;
				case COMPONENT_TYPE_STREAMED_AUDIO_SOURCE_2D:this->streamedAudioSource2D = new StreamedAudioSource2D(*other.streamedAudioSource2D); break;
				case COMPONENT_TYPE_UI_BUTTON:this->uiButton = new UIButton(*other.uiButton); break;
				case COMPONENT_TYPE_UI_COLOURED_RECTANGLE:this->uiColouredRectangle = new UIColouredRectangle(*other.uiColouredRectangle); break;
				case COMPONENT_TYPE_UI_GRAPH_EDIT:this->uiGraphEdit = new UIGraphEdit(*other.uiGraphEdit); break;
				case COMPONENT_TYPE_UI_GRAPH_NODE:this->uiGraphNode = new UIGraphNode(*other.uiGraphNode); break;
				case COMPONENT_TYPE_UI_HORIZONTAL_SCROLLBAR:this->uiHorizontalScrollbar = new UIHorizontalScrollbar(*other.uiHorizontalScrollbar); break;
				case COMPONENT_TYPE_UI_HORIZONTAL_SEPARATOR:this->uiHorizontalSeparator = new UIHorizontalSeparator(*other.uiHorizontalSeparator); break;
				case COMPONENT_TYPE_UI_HORIZONTAL_SLIDER:this->uiHorizontalSlider = new UIHorizontalSlider(*other.uiHorizontalSlider); break;
				case COMPONENT_TYPE_UI_HOVER_CARD:this->uiHoverCard = new UIHoverCard(*other.uiHoverCard); break;
				case COMPONENT_TYPE_UI_ITEM_LIST:this->uiItemList = new UIItemList(*other.uiItemList); break;
				case COMPONENT_TYPE_UI_LINK_BUTTON:this->uiLinkButton = new UILinkButton(*other.uiLinkButton); break;
				case COMPONENT_TYPE_UI_PANEL:this->uiPanel = new UIPanel(*other.uiPanel); break;
				case COMPONENT_TYPE_UI_PROGRESS_BAR:this->uiProgressBar = new UIProgressBar(*other.uiProgressBar); break;
				case COMPONENT_TYPE_UI_RICH_TEXT_LABEL:this->uiRichTextLabel = new UIRichTextLabel(*other.uiRichTextLabel); break;
				case COMPONENT_TYPE_UI_SPIN_BOX:this->uiSpinBox = new UISpinBox(*other.uiSpinBox); break;
				case COMPONENT_TYPE_UI_TAB:this->uiTab = new UITab(*other.uiTab); break;
				case COMPONENT_TYPE_UI_TEXT_EDIT:this->uiTextEdit = new UITextEdit(*other.uiTextEdit); break;
				case COMPONENT_TYPE_UI_TEXT_LABEL:this->uiTextLabel = new UITextLabel(*other.uiTextLabel); break;
				case COMPONENT_TYPE_UI_TEXTURED_BUTTON:this->uiTexturedButton = new UITexturedButton(*other.uiTexturedButton); break;
				case COMPONENT_TYPE_UI_TEXTURED_PROGRESS_BAR:this->uiTexturedProgressBar = new UITexturedProgressBar(*other.uiTexturedProgressBar); break;
				case COMPONENT_TYPE_UI_TEXTURED_RECTANGLE:this->uiTexturedRectangle = new UITexturedRectangle(*other.uiTexturedRectangle); break;
				case COMPONENT_TYPE_UI_TREE:this->uiTree = new UITree(*other.uiTree); break;
				case COMPONENT_TYPE_UI_VERTICAL_SCROLLBAR:this->uiVerticalScrollbar = new UIVerticalScrollbar(*other.uiVerticalScrollbar); break;
				case COMPONENT_TYPE_UI_VERTICAL_SEPARATOR:this->uiVerticalSeparator = new UIVerticalSeparator(*other.uiVerticalSeparator); break;
				case COMPONENT_TYPE_UI_VERTICAL_SLIDER:this->uiVerticalSlider = new UIVerticalSlider(*other.uiVerticalSlider); break;
			}
		}

		~ComponentVariant()
		{
			switch (this->type)
			{
				case COMPONENT_TYPE_AI_BEHAVIOUR_TREE: delete this->aiBehaviourTree; break;
				case COMPONENT_TYPE_AI_SIGHT_PERCEPTION: delete this->aiSightPerception2D; break;
				case COMPONENT_TYPE_AI_SOUND_PERCEPTION: delete this->aiSoundPerception2D; break;
				case COMPONENT_TYPE_ANIMATED_SPRITE:delete this->animatedSprite; break;
				case COMPONENT_TYPE_AUDIO_LISTENER_2D:delete this->audioListener2D; break;
				case COMPONENT_TYPE_CAMERA_2D: delete this->camera2D; break;
				case COMPONENT_TYPE_CIRCLE_COLLIDER_2D:delete this->circleCollider2D; break;
				case COMPONENT_TYPE_CONSTANT_DIRECTIONAL_FORCE_2D:delete this->constantDirectionalForce2D; break;
				case COMPONENT_TYPE_CONSTANT_POINT_FORCE_2D:delete this->constantPointForce2D; break;
				case COMPONENT_TYPE_COUNTDOWN_TIMER:delete this->countdownTimer; break;
				case COMPONENT_TYPE_FIXED_TRANSFORM_2D:delete this->fixedTransform2D; break;
				case COMPONENT_TYPE_NAVIGATION_MESH_AGENT_2D:delete this->navigationMeshAgent2D; break;
				case COMPONENT_TYPE_NAVIGATION_MESH_BOX_OBSTACLE_2D:delete this->navigationMeshBoxObstacle2D; break;
				case COMPONENT_TYPE_NAVIGATION_PATH_2D:delete this->navigationPath2D; break;
				case COMPONENT_TYPE_NEURAL_NETWORK:delete this->neuralNetwork; break;
				case COMPONENT_TYPE_PHYSICS_CONSTRAINT_2D:delete this->physicsConstraint2D; break;
				case COMPONENT_TYPE_PHYSICS_THRUSTER_2D:delete this->physicsThruster2D; break;
				case COMPONENT_TYPE_PROPERTY_ANIMATION:delete this->propertyAnimation; break;
				case COMPONENT_TYPE_RECTANGULAR_COLIIDER_2D:delete this->rectangularCollider2D; break;
				case COMPONENT_TYPE_RECTANGULAR_MESH_2D:delete this->rectangularMesh2D; break;
				case COMPONENT_TYPE_RECTANGULAR_TRIGGER_AREA_2D:delete this->rectangularTriggerArea2D; break;
				case COMPONENT_TYPE_REGULAR_POLYGONAL_MESH_2D:delete this->regularPolygonalMesh2D; break;
				case COMPONENT_TYPE_RIGID_BODY_2D:delete this->rigidBody2D; break;
				case COMPONENT_TYPE_SPRITE:delete this->sprite; break;
				case COMPONENT_TYPE_STATIC_FLUID_2D:delete this->staticFluid2D; break;
				case COMPONENT_TYPE_STREAMED_AUDIO_SOURCE_2D:delete this->streamedAudioSource2D; break;
				case COMPONENT_TYPE_UI_BUTTON:delete this->uiButton; break;
				case COMPONENT_TYPE_UI_COLOURED_RECTANGLE:delete this->uiColouredRectangle; break;
				case COMPONENT_TYPE_UI_GRAPH_EDIT:delete this->uiGraphEdit; break;
				case COMPONENT_TYPE_UI_GRAPH_NODE:delete this->uiGraphNode; break;
				case COMPONENT_TYPE_UI_HORIZONTAL_SCROLLBAR:delete this->uiHorizontalScrollbar; break;
				case COMPONENT_TYPE_UI_HORIZONTAL_SEPARATOR:delete this->uiHorizontalSeparator; break;
				case COMPONENT_TYPE_UI_HORIZONTAL_SLIDER:delete this->uiHorizontalSlider; break;
				case COMPONENT_TYPE_UI_HOVER_CARD:delete this->uiHoverCard; break;
				case COMPONENT_TYPE_UI_ITEM_LIST:delete this->uiItemList; break;
				case COMPONENT_TYPE_UI_LINK_BUTTON:delete this->uiLinkButton; break;
				case COMPONENT_TYPE_UI_PANEL:delete this->uiPanel; break;
				case COMPONENT_TYPE_UI_PROGRESS_BAR:delete this->uiProgressBar; break;
				case COMPONENT_TYPE_UI_RICH_TEXT_LABEL:delete this->uiRichTextLabel; break;
				case COMPONENT_TYPE_UI_SPIN_BOX:delete this->uiSpinBox; break;
				case COMPONENT_TYPE_UI_TAB:delete this->uiTab; break;
				case COMPONENT_TYPE_UI_TEXT_EDIT:delete this->uiTextEdit; break;
				case COMPONENT_TYPE_UI_TEXT_LABEL:delete this->uiTextLabel; break;
				case COMPONENT_TYPE_UI_TEXTURED_BUTTON:delete this->uiTexturedButton; break;
				case COMPONENT_TYPE_UI_TEXTURED_PROGRESS_BAR:delete this->uiTexturedProgressBar; break;
				case COMPONENT_TYPE_UI_TEXTURED_RECTANGLE:delete this->uiTexturedRectangle; break;
				case COMPONENT_TYPE_UI_TREE:delete this->uiTree; break;
				case COMPONENT_TYPE_UI_VERTICAL_SCROLLBAR:delete this->uiVerticalScrollbar; break;
				case COMPONENT_TYPE_UI_VERTICAL_SEPARATOR:delete this->uiVerticalSeparator; break;
				case COMPONENT_TYPE_UI_VERTICAL_SLIDER:delete this->uiVerticalSlider; break;
			}
		}

        union
        {
			AIBehaviourTree* aiBehaviourTree;
			AISightPerception2D* aiSightPerception2D;
			AISoundPerception2D* aiSoundPerception2D;
			AnimatedSprite* animatedSprite;
			AudioListener2D* audioListener2D;
			Camera2D* camera2D;
			CircleCollider2D* circleCollider2D;
			ConstantDirectionalForce2D* constantDirectionalForce2D;
			ConstantPointForce2D* constantPointForce2D;
			CountdownTimer* countdownTimer;
			FixedTransform2D* fixedTransform2D;
			NavigationMeshAgent2D* navigationMeshAgent2D;
			NavigationMeshBoxObstacle2D* navigationMeshBoxObstacle2D;
			NavigationPath2D* navigationPath2D;
			NeuralNetwork* neuralNetwork;
			PhysicsConstraint2D* physicsConstraint2D;
			PhysicsThruster2D* physicsThruster2D;
			PropertyAnimation* propertyAnimation;
			RectangularCollider2D* rectangularCollider2D;
			RectangularMesh2D* rectangularMesh2D;
			RectangularTriggerArea2D* rectangularTriggerArea2D;
			RegularPolygonalMesh2D* regularPolygonalMesh2D;
			RigidBody2D* rigidBody2D;
			Sprite* sprite;
			StaticFluid2D* staticFluid2D;
			StreamedAudioSource2D* streamedAudioSource2D;
			UIButton* uiButton;
			UIColouredRectangle* uiColouredRectangle;
			UIGraphEdit* uiGraphEdit;
			UIGraphNode* uiGraphNode;
			UIHorizontalScrollbar* uiHorizontalScrollbar;
			UIHorizontalSeparator* uiHorizontalSeparator;
			UIHorizontalSlider* uiHorizontalSlider;
			UIHoverCard* uiHoverCard;
			UIItemList* uiItemList;
			UILinkButton* uiLinkButton;
			UIPanel* uiPanel;
			UIProgressBar* uiProgressBar;
			UIRichTextLabel* uiRichTextLabel;
			UISpinBox* uiSpinBox;
			UITab* uiTab;
			UITextEdit* uiTextEdit;
			UITextLabel* uiTextLabel;
			UITexturedButton* uiTexturedButton;
			UITexturedProgressBar* uiTexturedProgressBar;
			UITexturedRectangle* uiTexturedRectangle;
			UITree* uiTree;
			UIVerticalScrollbar* uiVerticalScrollbar;
			UIVerticalSeparator* uiVerticalSeparator;
			UIVerticalSlider* uiVerticalSlider;
        };
    };
}