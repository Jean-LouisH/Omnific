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

#include "utilities/string.hpp"
#include <game/scene/components/component_variant.hpp>
#include <game/scene/scene.hpp>
#include <game/scene_serializer.hpp>
#include <game/scene_storage.hpp>
#include <utilities/aliases.hpp>
#include <game/scene/event_bus.hpp>

namespace Lilliputian
{
	class SceneAPI
	{
	public:
		void setSceneStorage(SceneStorage* sceneStorage);
		void bindEntity(SceneTreeID sceneTreeID, EntityID entityID);
		void setSceneSerializer(SceneSerializer* sceneSerializer);

		bool hasComponent(ComponentVariant::Type type);
		void preloadScene(String sceneFilename);
		void loadScene(String sceneFilename);
		void unloadScene(String sceneFilename);
		void changeToScene(String sceneFilename);

		Entity2D& getThisEntity2D();
		SceneTree2D& getThisSceneTree2D();
		Scene& getScene();

		ComponentVariant& getComponentVariant(ComponentVariant::Type type);

		void addComponent(AIBehaviourTree* aiBehaviourTree);
		void addComponent(AISightPerception2D* aiSightPerception2D);
		void addComponent(AISoundPerception2D* aiSoundPerception2D);
		void addComponent(AnimatedSprite* animatedSprite);
		void addComponent(AudioListener2D* audioListener2D);
		void addComponent(AudioStreamSource2D* audioStreamSource2D);
		void addComponent(Camera2D* camera2D);
		void addComponent(CircleCollider2D* circleCollider2D);
		void addComponent(ConstantDirectionalForce2D* constantDirectionalForce2D);
		void addComponent(ConstantPointForce2D* constantPointForce2D);
		void addComponent(CountdownTimer* countdownTimer);
		void addComponent(FixedTransform2D* fixedTransform2D);
		void addComponent(KinematicBody2D* kinematicBody2D);
		void addComponent(NavigationMeshAgent2D* navigationMeshAgent2D);
		void addComponent(NavigationMeshBoxObstacle2D*navigationMeshBoxObstacle2D);
		void addComponent(NavigationPath2D* navigationPath2D);
		void addComponent(PhysicsConstraint2D* physicsConstraint2D);
		void addComponent(PhysicsThruster2D* physicsThruster2D);
		void addComponent(PropertyAnimation* propertyAnimation);
		void addComponent(RectangularCollider2D* rectangularCollider2D);
		void addComponent(RectangularMesh2D* rectangularMesh2D);
		void addComponent(RectangularTriggerArea2D* rectangularTriggerArea2D);
		void addComponent(RegularPolygonalMesh2D* regularPolygonalMesh2D);
		void addComponent(RigidBody2D* rigidBody2D);
		void addComponent(Sprite* sprite);
		void addComponent(StaticFluid2D* staticFluid2D);
		void addComponent(Transform2D* transform2D);
		void addComponent(UIButton* uiButton);
		void addComponent(UIGraphEdit* uiGraphEdit);
		void addComponent(UIGraphNode* uiGraphNode);
		void addComponent(UIHoverCard* uiHoverCard);
		void addComponent(UIItemList* uiItemList);
		void addComponent(UIPanel* uiPanel);
		void addComponent(UIProgressBar* uiProgressBar);
		void addComponent(UIRectangle* uiRectangle);
		void addComponent(UIScrollbar* uiScrollbar);
		void addComponent(UISeparator* uiSeparator);
		void addComponent(UISlider* uiSlider);
		void addComponent(UISpinBox* uiSpinBox);
		void addComponent(UITab* uiTab);
		void addComponent(UITextEdit* uiTextEdit);
		void addComponent(UITextLabel* uiTextLabel);
		void addComponent(UITree* uiTree);

		AIBehaviourTree& getAIBehaviourTree();
		AISightPerception2D& getAISightPerception2D();
		AISoundPerception2D& getAISoundPerception2D();
		AnimatedSprite& getAnimatedSprite();
		AudioListener2D& getAudioListener2D();
		AudioStreamSource2D& getAudioStreamSource2D();
		Camera2D& getCamera2D();
		CircleCollider2D& getCircleCollider2D();
		ConstantDirectionalForce2D& getConstantDirectionalForce2D();
		ConstantPointForce2D& getConstantPointForce2D();
		CountdownTimer& getCountdownTimer();
		FixedTransform2D& getFixedTransform2D();
		KinematicBody2D& getKinematicBody2D();
		NavigationMeshAgent2D& getNavigationMeshAgent2D();
		NavigationMeshBoxObstacle2D& getNavigationMeshBoxObstacle2D();
		NavigationPath2D& getNavigationPath2D();
		PhysicsConstraint2D& getPhysicsConstraint2D();
		PhysicsThruster2D& getPhysicsThruster2D();
		PropertyAnimation& getPropertyAnimation();
		RectangularCollider2D& getRectangularCollider2D();
		RectangularMesh2D& getRectangularMesh2D();
		RectangularTriggerArea2D& getRectangularTriggerArea2D();
		RegularPolygonalMesh2D& getRegularPolygonalMesh2D();
		RigidBody2D& getRigidBody2D();
		Sprite& getSprite();
		StaticFluid2D& getStaticFluid2D();
		Transform2D& getTransform2D();
		UIButton& getUIButton();
		UIGraphEdit& getUIGraphEdit();
		UIGraphNode& getUIGraphNode();
		UIHoverCard& getUIHoverCard();
		UIItemList& getUIItemList();
		UIPanel& getUIPanel();
		UIProgressBar& getUIProgressBar();
		UIRectangle& getUIRectangle();
		UIScrollbar& getUIScrollbar();
		UISeparator& getUISeparator();
		UISlider& getUISlider();
		UISpinBox& getUISpinBox();
		UITab& getUITab();
		UITextEdit& getUITextEdit();
		UITextLabel& getUITextLabel();
		UITree& getUITree();
	private:
		SceneTreeID boundSceneTreeID = 0;
		EntityID boundEntityID = 0;
		SceneStorage* sceneStorage = nullptr;
		SceneSerializer* sceneSerializer = nullptr;
	};
}