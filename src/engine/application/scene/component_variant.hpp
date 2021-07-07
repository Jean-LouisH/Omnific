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

#include "components/behaviour_tree.hpp"
#include "components/sight_perception.hpp"
#include "components/sound_perception.hpp"
#include "components/animated_sprite.hpp"
#include "components/audio_listener.hpp"
#include "components/audio_stream_source.hpp"
#include "components/camera.hpp"
#include "components/ball_collider.hpp"
#include "components/constant_directional_force.hpp"
#include "components/constant_point_force.hpp"
#include "components/countdown_timer.hpp"
#include "components/kinematic_body.hpp"
#include "components/navigation_mesh_agent.hpp"
#include "components/navigation_mesh_box_obstacle.hpp"
#include "components/navigation_path.hpp"
#include "components/physics_constraint.hpp"
#include "components/physics_thruster.hpp"
#include "components/property_animation.hpp"
#include "components/box_collider.hpp"
#include "components/box_mesh.hpp"
#include "components/box_trigger_space.hpp"
#include "components/regular_polytopal_mesh.hpp"
#include "components/rigid_body.hpp"
#include "components/sprite.hpp"
#include "components/static_fluid.hpp"
#include "components/transform.hpp"
#include "components/ui_button.hpp"
#include "components/ui_rectangle.hpp"
#include "components/ui_graph_edit.hpp"
#include "components/ui_graph_node.hpp"
#include "components/ui_scrollbar.hpp"
#include "components/ui_separator.hpp"
#include "components/ui_slider.hpp"
#include "components/ui_hover_card.hpp"
#include "components/ui_item_list.hpp"
#include "components/ui_panel.hpp"
#include "components/ui_progress_bar.hpp"
#include "components/ui_spin_box.hpp"
#include "components/ui_tab.hpp"
#include "components/ui_text_edit.hpp"
#include "components/ui_text_label.hpp"
#include "components/ui_tree.hpp"

namespace Lilliputian
{
    class ComponentVariant
    {
	public:

		enum Type
		{
			NONE,
			BEHAVIOUR_TREE,
			SIGHT_PERCEPTION,
			SOUND_PERCEPTION,
			ANIMATED_SPRITE,
			AUDIO_LISTENER,
			AUDIO_STREAM_SOURCE,
			CAMERA,
			BALL_COLLIDER,
			CONSTANT_DIRECTIONAL_FORCE,
			CONSTANT_POINT_FORCE,
			COUNTDOWN_TIMER,
			KINEMATIC_BODY,
			NAVIGATION_MESH,
			NAVIGATION_MESH_AGENT,
			NAVIGATION_MESH_BOX_OBSTACLE,
			NAVIGATION_PATH,
			PHYSICS_CONSTRAINT,
			PHYSICS_THRUSTER,
			PROPERTY_ANIMATION,
			BOX_COLLIDER,
			BOX_MESH,
			BOX_TRIGGER_SPACE,
			REGULAR_POLYTOPAL_MESH,
			RIGID_BODY,
			SPRITE,
			STATIC_FLUID,
			TRANSFORM,
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

		void setTo(BehaviourTree* aiBehaviourTree);
		void setTo(SightPerception* aiSightPerception2D);
		void setTo(SoundPerception* aiSoundPerception2D);
		void setTo(AnimatedSprite* animatedSprite);
		void setTo(AudioListener* audioListener2D);
		void setTo(Camera* camera2D);
		void setTo(BallCollider* circleCollider2D);
		void setTo(ConstantDirectionalForce* constantDirectionalForce2D);
		void setTo(ConstantPointForce* constantPointForce2D);
		void setTo(CountdownTimer* countdownTimer);
		void setTo(KinematicBody* kinematicBody2D);
		void setTo(NavigationMeshAgent* navigationMeshAgent2D);
		void setTo(NavigationMeshBoxObstacle* navigationMeshBoxObstacle2D);
		void setTo(NavigationPath* navigationPath2D);
		void setTo(PhysicsConstraint* physicsConstraint2D);
		void setTo(PhysicsThruster* physicsThruster2D);
		void setTo(PropertyAnimation* propertyAnimation);
		void setTo(BoxCollider* rectangularCollider2D);
		void setTo(BoxMesh* rectangularMesh2D);
		void setTo(BoxTriggerSpace* rectangularTriggerArea2D);
		void setTo(RegularPolytopalMesh* regularPolygonalMesh2D);
		void setTo(RigidBody* rigidBody2D);
		void setTo(Sprite* sprite);
		void setTo(StaticFluid* staticFluid2D);
		void setTo(Transform* transform2D);
		void setTo(AudioStreamSource* audioStreamSource2D);
		void setTo(UIButton* uiButton);
		void setTo(UIRectangle* uiRectangle);
		void setTo(UIGraphEdit* uiGraphEdit);
		void setTo(UIGraphNode* uiGraphNode);
		void setTo(UIScrollbar* uiScrollbar);
		void setTo(UISeparator* uiSeparator);
		void setTo(UISlider* uiSlider);
		void setTo(UIHoverCard* uiHoverCard);
		void setTo(UIItemList* uiItemList);
		void setTo(UIPanel* uiPanel);
		void setTo(UIProgressBar* uiProgressBar);
		void setTo(UISpinBox* uiSpinBox);
		void setTo(UITab* uiTab);
		void setTo(UITextEdit* uiTextEdit);
		void setTo(UITextLabel* uiTextLabel);
		void setTo(UITree* uiTree);

		bool isRenderable();

		BehaviourTree* getBehaviourTree();
		SightPerception* getSightPerception();
		SoundPerception* getSoundPerception();
		AnimatedSprite* getAnimatedSprite();
		AudioListener* getAudioListener();
		Camera* getCamera();
		BallCollider* getBallCollider();
		ConstantDirectionalForce* getConstantDirectionalForce();
		ConstantPointForce* getConstantPointForce();
		CountdownTimer* getCountdownTimer();
		KinematicBody* getKinematicBody();
		NavigationMeshAgent* getNavigationMeshAgent();
		NavigationMeshBoxObstacle* getNavigationMeshBoxObstacle();
		NavigationPath* getNavigationPath();
		PhysicsConstraint* getPhysicsConstraint();
		PhysicsThruster* getPhysicsThruster();
		PropertyAnimation* getPropertyAnimation();
		BoxCollider* getBoxCollider();
		BoxMesh* getBoxMesh();
		BoxTriggerSpace* getBoxTriggerSpace();
		RegularPolytopalMesh* getRegularPolytopalMesh();
		RigidBody* getRigidBody();
		Sprite* getSprite();
		StaticFluid* getStaticFluid();
		Transform* getTransform();
		AudioStreamSource* getAudioStreamSource();
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
			BehaviourTree* behaviourTree;
			SightPerception* sightPerception;
			SoundPerception*soundPerception;
			AnimatedSprite* animatedSprite;
			AudioListener* audioListener;
			Camera* camera;
			BallCollider* ballCollider;
			ConstantDirectionalForce* constantDirectionalForce;
			ConstantPointForce* constantPointForce;
			CountdownTimer* countdownTimer;
			KinematicBody* kinematicBody;
			NavigationMeshAgent* navigationMeshAgent;
			NavigationMeshBoxObstacle* navigationMeshBoxObstacle;
			NavigationPath* navigationPath;
			PhysicsConstraint* physicsConstraint;
			PhysicsThruster* physicsThruster;
			PropertyAnimation* propertyAnimation;
			BoxCollider* boxCollider;
			BoxMesh* boxMesh;
			BoxTriggerSpace* boxTriggerSpace;
			RegularPolytopalMesh* regularPolytopalMesh;
			RigidBody* rigidBody;
			Sprite* sprite;
			StaticFluid* staticFluid;
			Transform* transform;
			AudioStreamSource* audioStreamSource;
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
    };
}