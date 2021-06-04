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

#pragma once


#include "utilities/aliases.hpp"

#include "ai_behaviour_tree.hpp"
#include "ai_sight_perception.hpp"
#include "ai_sound_perception.hpp"
#include "animated_sprite.hpp"
#include "audio_listener_2d.hpp"
#include "audio_stream_source_2d.hpp"
#include "camera_2d.hpp"
#include "circle_collider_2d.hpp"
#include "constant_directional_force_2d.hpp"
#include "constant_point_force_2d.hpp"
#include "countdown_timer.hpp"
#include "fixed_transform_2d.hpp"
#include "kinematic_body_2d.hpp"
#include "navigation_mesh_agent_2d.hpp"
#include "navigation_mesh_box_obstacle_2d.hpp"
#include "navigation_path_2d.hpp"
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
#include "transform_2d.hpp"
#include "ui_button.hpp"
#include "ui_rectangle.hpp"
#include "ui_graph_edit.hpp"
#include "ui_graph_node.hpp"
#include "ui_scrollbar.hpp"
#include "ui_separator.hpp"
#include "ui_slider.hpp"
#include "ui_hover_card.hpp"
#include "ui_item_list.hpp"
#include "ui_panel.hpp"
#include "ui_progress_bar.hpp"
#include "ui_spin_box.hpp"
#include "ui_tab.hpp"
#include "ui_text_edit.hpp"
#include "ui_text_label.hpp"
#include "ui_tree.hpp"

namespace Lilliputian
{
    class ComponentVariant
    {
	public:

		enum Type
		{
			NONE,
			AI_BEHAVIOUR_TREE,
			AI_SIGHT_PERCEPTION,
			AI_SOUND_PERCEPTION,
			ANIMATED_SPRITE,
			AUDIO_LISTENER_2D,
			AUDIO_STREAM_SOURCE_2D,
			CAMERA_2D,
			CIRCLE_COLLIDER_2D,
			CONSTANT_DIRECTIONAL_FORCE_2D,
			CONSTANT_POINT_FORCE_2D,
			COUNTDOWN_TIMER,
			FIXED_TRANSFORM_2D,
			KINEMATIC_BODY_2D,
			NAVIGATION_MESH_2D,
			NAVIGATION_MESH_AGENT_2D,
			NAVIGATION_MESH_BOX_OBSTACLE_2D,
			NAVIGATION_PATH_2D,
			PHYSICS_CONSTRAINT_2D,
			PHYSICS_THRUSTER_2D,
			PROPERTY_ANIMATION,
			RECTANGULAR_COLIIDER_2D,
			RECTANGULAR_MESH_2D,
			RECTANGULAR_TRIGGER_AREA_2D,
			REGULAR_POLYGONAL_MESH_2D,
			RIGID_BODY_2D,
			SPRITE,
			STATIC_FLUID_2D,
			TRANSFORM_2D,
			UI_BUTTON,
			UI_RECTANGLE,
			UI_GRAPH_EDIT,
			UI_GRAPH_NODE,
			UI_SCROLLBAR,
			UI_SEPARATOR,
			UI_SLIDER,
			UI_HOVER_CARD,
			UI_ITEM_LIST,
			UI_PANEL,
			UI_PROGRESS_BAR,
			UI_SPIN_BOX,
			UI_TAB,
			UI_TEXT_EDIT,
			UI_TEXT_LABEL,
			UI_TREE
		};

		ComponentVariant();
		ComponentVariant(const ComponentVariant& other);
		~ComponentVariant();

		void setEntityID(EntityID entityID);

		void setToAIBehaviourTree(AIBehaviourTree* aiBehaviourTree);
		void setToAISightPerception2D(AISightPerception2D* aiSightPerception2D);
		void setToAISoundPerception2D(AISoundPerception2D* aiSoundPerception2D);
		void setToAnimatedSprite(AnimatedSprite* animatedSprite);
		void setToAudioListener2D(AudioListener2D* audioListener2D);
		void setToCamera2D(Camera2D* camera2D);
		void setToCircleCollider2D(CircleCollider2D* circleCollider2D);
		void setToConstantDirectionalForce2D(ConstantDirectionalForce2D* constantDirectionalForce2D);
		void setToConstantPointForce2D(ConstantPointForce2D* constantPointForce2D);
		void setToCountdownTimer(CountdownTimer* countdownTimer);
		void setToFixedTransform2D(FixedTransform2D* fixedTransform2D);
		void setToKinematicBody2D(KinematicBody2D* kinematicBody2D);
		void setToNavigationMeshAgent2D(NavigationMeshAgent2D* navigationMeshAgent2D);
		void setToNavigationMeshBoxObstacle2D(NavigationMeshBoxObstacle2D* navigationMeshBoxObstacle2D);
		void setToNavigationPath2D(NavigationPath2D* navigationPath2D);
		void setToPhysicsConstraint2D(PhysicsConstraint2D* physicsConstraint2D);
		void setToPhysicsThruster2D(PhysicsThruster2D* physicsThruster2D);
		void setToPropertyAnimation(PropertyAnimation* propertyAnimation);
		void setToRectangularCollider2D(RectangularCollider2D* rectangularCollider2D);
		void setToRectangularMesh2D(RectangularMesh2D* rectangularMesh2D);
		void setToRectangularTriggerArea2D(RectangularTriggerArea2D* rectangularTriggerArea2D);
		void setToRectangularPolgyonalMesh2D(RegularPolygonalMesh2D* regularPolygonalMesh2D);
		void setToRigidBody2D(RigidBody2D* rigidBody2D);
		void setToSprite(Sprite* sprite);
		void setToStaticFluid2D(StaticFluid2D* staticFluid2D);
		void setToTransform2D(Transform2D* transform2D);
		void setToAudioStreamSource2D(AudioStreamSource2D* audioStreamSource2D);
		void setToUIButton(UIButton* uiButton);
		void setToUIRectangle(UIRectangle* uiRectangle);
		void setToUIGraphEdit(UIGraphEdit* uiGraphEdit);
		void setToUIGraphNode(UIGraphNode* uiGraphNode);
		void setToUIScrollbar(UIScrollbar* uiScrollbar);
		void setToUISeparator(UISeparator* uiSeparator);
		void setToUISlider(UISlider* uiSlider);
		void setToUIHoverCard(UIHoverCard* uiHoverCard);
		void setToUIItemList(UIItemList* uiItemList);
		void setToUIPanel(UIPanel* uiPanel);
		void setToUIProgressBar(UIProgressBar* uiProgressBar);
		void setToUISpinBox(UISpinBox* uiSpinBox);
		void setToUITab(UITab* uiTab);
		void setToUITextEdit(UITextEdit* uiTextEdit);
		void setToUITextLabel(UITextLabel* uiTextLabel);
		void setToUITree(UITree* uiTree);

		bool isRenderable();

		AIBehaviourTree* getAIBehaviourTree();
		AISightPerception2D* getAISightPerception2D();
		AISoundPerception2D* getAISoundPerception2D();
		AnimatedSprite* getAnimatedSprite();
		AudioListener2D* getAudioListener2D();
		Camera2D* getCamera2D();
		CircleCollider2D* getCircleCollider2D();
		ConstantDirectionalForce2D* getConstantDirectionalForce2D();
		ConstantPointForce2D* getConstantPointForce2D();
		CountdownTimer* getCountdownTimer();
		FixedTransform2D* getFixedTransform2D();
		KinematicBody2D* getKinematicBody2D();
		NavigationMeshAgent2D* getNavigationMeshAgent2D();
		NavigationMeshBoxObstacle2D* getNavigationMeshBoxObstacle2D();
		NavigationPath2D* getNavigationPath2D();
		PhysicsConstraint2D* getPhysicsConstraint2D();
		PhysicsThruster2D* getPhysicsThruster2D();
		PropertyAnimation* getPropertyAnimation();
		RectangularCollider2D* getRectangularCollider2D();
		RectangularMesh2D* getRectangularMesh2D();
		RectangularTriggerArea2D* getRectangularTriggerArea2D();
		RegularPolygonalMesh2D* getRegularPolygonalMesh2D();
		RigidBody2D* getRigidBody2D();
		Sprite* getSprite();
		StaticFluid2D* getStaticFluid2D();
		Transform2D* getTransform2D();
		AudioStreamSource2D* getAudioStreamSource2D();
		UIButton* getUIButton();
		UIRectangle* getUIRectangle();
		UIGraphEdit* getUIGraphEdit();
		UIGraphNode* getUIGraphNode();
		UIScrollbar* getUIScrollbar();
		UISeparator* getUISeparator();
		UISlider* getUISlider();
		UIHoverCard* getUIHoverCard();
		UIItemList* getUIItemList();
		UIPanel* getUIPanel();
		UIProgressBar* getUIProgressBar();
		UISpinBox* getUISpinBox();
		UITab* getUITab();
		UITextEdit* getUITextEdit();
		UITextLabel* getUITextLabel();
		UITree* getUITree();

		ComponentID getID();
		Type getType();
		EntityID getEntityID();
		Image& getImage();

		void unloadImage();

	private:

		ComponentID ID = 0;
		Type type = Type::NONE;
		EntityID entityID = DUMMY_ENTITY;
		
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
			KinematicBody2D* kinematicBody2D;
			NavigationMeshAgent2D* navigationMeshAgent2D;
			NavigationMeshBoxObstacle2D* navigationMeshBoxObstacle2D;
			NavigationPath2D* navigationPath2D;
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
			Transform2D* transform2D;
			AudioStreamSource2D* audioStreamSource2D;
			UIButton* uiButton;
			UIRectangle* uiRectangle;
			UIGraphEdit* uiGraphEdit;
			UIGraphNode* uiGraphNode;
			UIScrollbar* uiScrollbar;
			UISeparator* uiSeparator;
			UISlider* uiSlider;
			UIHoverCard* uiHoverCard;
			UIItemList* uiItemList;
			UIPanel* uiPanel;
			UIProgressBar* uiProgressBar;
			UISpinBox* uiSpinBox;
			UITab* uiTab;
			UITextEdit* uiTextEdit;
			UITextLabel* uiTextLabel;
			UITree* uiTree;
        };

		void initialize();
    };
}