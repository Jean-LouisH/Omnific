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
#include <utilities/aliases.hpp>

namespace Lilliputian
{
	class SceneAPI
	{
	public:
		void bindScene(Scene* scene);
		void bindEntity(SceneTreeID sceneTreeID, EntityID entityID);
		bool hasComponent(ComponentVariant::Type type);
		void changeToScene(String sceneFilename);
		void setSceneSerializer(SceneSerializer* sceneSerializer);

		Entity2D& getThisEntity2D();
		SceneTree2D& getThisSceneTree2D();
		Scene& getScene();

		ComponentVariant& getComponentVariant(ComponentVariant::Type type);

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
		Scene* scene = nullptr;
		SceneSerializer* sceneSerializer = nullptr;
	};
}