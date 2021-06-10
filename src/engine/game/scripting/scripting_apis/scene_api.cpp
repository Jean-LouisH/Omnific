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

#include "scene_api.hpp"

void Lilliputian::SceneAPI::setSceneStorage(SceneStorage* sceneStorage)
{
	this->sceneStorage = sceneStorage;
}

void Lilliputian::SceneAPI::bindEntity(SceneTreeID sceneTreeID, EntityID entityID)
{
	this->boundSceneTreeID = sceneTreeID;
	this->boundEntityID = entityID;
}

void Lilliputian::SceneAPI::setSceneSerializer(SceneSerializer* sceneSerializer)
{
	this->sceneSerializer = sceneSerializer;
}

bool Lilliputian::SceneAPI::hasComponent(ComponentVariant::Type type)
{
	bool result = false;
	std::vector<SceneTree2D>& sceneTree2Ds = this->sceneStorage->getActiveScene().getSceneTree2Ds();

	for (int i = 0; i < sceneTree2Ds.size(); i++)
		if (sceneTree2Ds.at(i).getID() == this->boundSceneTreeID)
			if (sceneTree2Ds.at(i).getEntity2D(this->boundEntityID).components.count(type) > 0)
				return true;

	return result;
}

void Lilliputian::SceneAPI::preloadScene(String sceneFilename)
{
	if (this->sceneSerializer->doesSceneExist(sceneFilename))
	{
		Scene newScene = this->sceneSerializer->loadFromFile(sceneFilename);
		this->sceneStorage->addScene(sceneFilename, newScene);
	}
}

void Lilliputian::SceneAPI::loadScene(String sceneFilename)
{
	if (this->sceneSerializer->doesSceneExist(sceneFilename))
	{
		Scene newScene = this->sceneSerializer->loadFromFile(sceneFilename);
		this->sceneStorage->replaceActiveScene(sceneFilename, newScene);
	}
}

void Lilliputian::SceneAPI::unloadScene(String sceneFilename)
{
	this->sceneStorage->removeScene(sceneFilename);
}

void Lilliputian::SceneAPI::changeToScene(String sceneFilename)
{
	if (this->sceneSerializer->doesSceneExist(sceneFilename))
	{
		this->sceneStorage->changeToScene(sceneFilename);
	}
}

Lilliputian::Entity2D& Lilliputian::SceneAPI::getThisEntity2D()
{
	return this->getThisSceneTree2D().getEntity2D(this->boundEntityID);
}

Lilliputian::SceneTree2D& Lilliputian::SceneAPI::getThisSceneTree2D()
{
	return this->sceneStorage->getActiveScene().getSceneTree2Ds().at(this->boundSceneTreeID);
}

Lilliputian::Scene& Lilliputian::SceneAPI::getScene()
{
	return sceneStorage->getActiveScene();
}

Lilliputian::ComponentVariant& Lilliputian::SceneAPI::getComponentVariant(ComponentVariant::Type type)
{
	ComponentVariant* componentVariant = nullptr;
	std::vector<SceneTree2D>& sceneTree2Ds = this->sceneStorage->getActiveScene().getSceneTree2Ds();
	SceneTree2D* sceneTree2D = nullptr;

	for (int i = 0; i < sceneTree2Ds.size(); i++)
		if (sceneTree2Ds.at(i).getID() == this->boundSceneTreeID)
			sceneTree2D = &sceneTree2Ds.at(i);
	
	Entity2D& entity2D = sceneTree2D->getEntity2D(this->boundEntityID);
	std::vector<ComponentVariant>& componentVariants = sceneTree2D->getComponentVariants();

	for (int i = 0; i < componentVariants.size(); i++)
		if (componentVariants.at(i).getID() == entity2D.components.at(type))
			componentVariant = &componentVariants.at(i);

	return *componentVariant;
}

void Lilliputian::SceneAPI::addComponent(AIBehaviourTree* aiBehaviourTree)
{
	ComponentVariant componentVariant;
	componentVariant.setToComponent(aiBehaviourTree);
	this->getThisSceneTree2D().addComponent(this->getThisEntity2D().ID, componentVariant);
}
void Lilliputian::SceneAPI::addComponent(AISightPerception2D* aiSightPerception2D)
{
	ComponentVariant componentVariant;
	componentVariant.setToComponent(aiSightPerception2D);
	this->getThisSceneTree2D().addComponent(this->getThisEntity2D().ID, componentVariant);
}
void Lilliputian::SceneAPI::addComponent(AISoundPerception2D* aiSoundPerception2D)
{
	ComponentVariant componentVariant;
	componentVariant.setToComponent(aiSoundPerception2D);
	this->getThisSceneTree2D().addComponent(this->getThisEntity2D().ID, componentVariant);
}
void Lilliputian::SceneAPI::addComponent(AnimatedSprite* animatedSprite)
{
	ComponentVariant componentVariant;
	componentVariant.setToComponent(animatedSprite);
	this->getThisSceneTree2D().addComponent(this->getThisEntity2D().ID, componentVariant);
}
void Lilliputian::SceneAPI::addComponent(AudioListener2D* audioListener2D)
{
	ComponentVariant componentVariant;
	componentVariant.setToComponent(audioListener2D);
	this->getThisSceneTree2D().addComponent(this->getThisEntity2D().ID, componentVariant);
}
void Lilliputian::SceneAPI::addComponent(AudioStreamSource2D* audioStreamSource2D)
{
	ComponentVariant componentVariant;
	componentVariant.setToComponent(audioStreamSource2D);
	this->getThisSceneTree2D().addComponent(this->getThisEntity2D().ID, componentVariant);
}
void Lilliputian::SceneAPI::addComponent(Camera2D* camera2D)
{
	ComponentVariant componentVariant;
	componentVariant.setToComponent(camera2D);
	this->getThisSceneTree2D().addComponent(this->getThisEntity2D().ID, componentVariant);
}
void Lilliputian::SceneAPI::addComponent(CircleCollider2D* circleCollider2D)
{
	ComponentVariant componentVariant;
	componentVariant.setToComponent(circleCollider2D);
	this->getThisSceneTree2D().addComponent(this->getThisEntity2D().ID, componentVariant);
}
void Lilliputian::SceneAPI::addComponent(ConstantDirectionalForce2D* constantDirectionalForce2D)
{
	ComponentVariant componentVariant;
	componentVariant.setToComponent(constantDirectionalForce2D);
	this->getThisSceneTree2D().addComponent(this->getThisEntity2D().ID, componentVariant);
}
void Lilliputian::SceneAPI::addComponent(ConstantPointForce2D* constantPointForce2D)
{
	ComponentVariant componentVariant;
	componentVariant.setToComponent(constantPointForce2D);
	this->getThisSceneTree2D().addComponent(this->getThisEntity2D().ID, componentVariant);
}
void Lilliputian::SceneAPI::addComponent(CountdownTimer* countdownTimer)
{
	ComponentVariant componentVariant;
	componentVariant.setToComponent(countdownTimer);
	this->getThisSceneTree2D().addComponent(this->getThisEntity2D().ID, componentVariant);
}
void Lilliputian::SceneAPI::addComponent(FixedTransform2D* fixedTransform2D)
{
	ComponentVariant componentVariant;
	componentVariant.setToComponent(fixedTransform2D);
	this->getThisSceneTree2D().addComponent(this->getThisEntity2D().ID, componentVariant);
}
void Lilliputian::SceneAPI::addComponent(KinematicBody2D* kinematicBody2D)
{
	ComponentVariant componentVariant;
	componentVariant.setToComponent(kinematicBody2D);
	this->getThisSceneTree2D().addComponent(this->getThisEntity2D().ID, componentVariant);
}
void Lilliputian::SceneAPI::addComponent(NavigationMeshAgent2D* navigationMeshAgent2D)
{
	ComponentVariant componentVariant;
	componentVariant.setToComponent(navigationMeshAgent2D);
	this->getThisSceneTree2D().addComponent(this->getThisEntity2D().ID, componentVariant);
}
void Lilliputian::SceneAPI::addComponent(NavigationMeshBoxObstacle2D* navigationMeshBoxObstacle2D)
{
	ComponentVariant componentVariant;
	componentVariant.setToComponent(navigationMeshBoxObstacle2D);
	this->getThisSceneTree2D().addComponent(this->getThisEntity2D().ID, componentVariant);
}
void Lilliputian::SceneAPI::addComponent(NavigationPath2D* navigationPath2D)
{
	ComponentVariant componentVariant;
	componentVariant.setToComponent(navigationPath2D);
	this->getThisSceneTree2D().addComponent(this->getThisEntity2D().ID, componentVariant);
}
void Lilliputian::SceneAPI::addComponent(PhysicsConstraint2D* physicsConstraint2D)
{
	ComponentVariant componentVariant;
	componentVariant.setToComponent(physicsConstraint2D);
	this->getThisSceneTree2D().addComponent(this->getThisEntity2D().ID, componentVariant);
}
void Lilliputian::SceneAPI::addComponent(PhysicsThruster2D* physicsThruster2D)
{
	ComponentVariant componentVariant;
	componentVariant.setToComponent(physicsThruster2D);
	this->getThisSceneTree2D().addComponent(this->getThisEntity2D().ID, componentVariant);
}
void Lilliputian::SceneAPI::addComponent(PropertyAnimation* propertyAnimation)
{
	ComponentVariant componentVariant;
	componentVariant.setToComponent(propertyAnimation);
	this->getThisSceneTree2D().addComponent(this->getThisEntity2D().ID, componentVariant);
}
void Lilliputian::SceneAPI::addComponent(RectangularCollider2D* rectangularCollider2D)
{
	ComponentVariant componentVariant;
	componentVariant.setToComponent(rectangularCollider2D);
	this->getThisSceneTree2D().addComponent(this->getThisEntity2D().ID, componentVariant);
}
void Lilliputian::SceneAPI::addComponent(RectangularMesh2D* rectangularMesh2D)
{
	ComponentVariant componentVariant;
	componentVariant.setToComponent(rectangularMesh2D);
	this->getThisSceneTree2D().addComponent(this->getThisEntity2D().ID, componentVariant);
}
void Lilliputian::SceneAPI::addComponent(RectangularTriggerArea2D* rectangularTriggerArea2D)
{
	ComponentVariant componentVariant;
	componentVariant.setToComponent(rectangularTriggerArea2D);
	this->getThisSceneTree2D().addComponent(this->getThisEntity2D().ID, componentVariant);
}
void Lilliputian::SceneAPI::addComponent(RegularPolygonalMesh2D* regularPolygonalMesh2D)
{
	ComponentVariant componentVariant;
	componentVariant.setToComponent(regularPolygonalMesh2D);
	this->getThisSceneTree2D().addComponent(this->getThisEntity2D().ID, componentVariant);
}
void Lilliputian::SceneAPI::addComponent(RigidBody2D* rigidBody2D)
{
	ComponentVariant componentVariant;
	componentVariant.setToComponent(rigidBody2D);
	this->getThisSceneTree2D().addComponent(this->getThisEntity2D().ID, componentVariant);
}
void Lilliputian::SceneAPI::addComponent(Sprite* sprite)
{
	ComponentVariant componentVariant;
	componentVariant.setToComponent(sprite);
	this->getThisSceneTree2D().addComponent(this->getThisEntity2D().ID, componentVariant);
}
void Lilliputian::SceneAPI::addComponent(StaticFluid2D* staticFluid2D)
{
	ComponentVariant componentVariant;
	componentVariant.setToComponent(staticFluid2D);
	this->getThisSceneTree2D().addComponent(this->getThisEntity2D().ID, componentVariant);
}
void Lilliputian::SceneAPI::addComponent(Transform2D* transform2D)
{
	ComponentVariant componentVariant;
	componentVariant.setToComponent(transform2D);
	this->getThisSceneTree2D().addComponent(this->getThisEntity2D().ID, componentVariant);
}
void Lilliputian::SceneAPI::addComponent(UIButton* uiButton)
{
	ComponentVariant componentVariant;
	componentVariant.setToComponent(uiButton);
	this->getThisSceneTree2D().addComponent(this->getThisEntity2D().ID, componentVariant);
}
void Lilliputian::SceneAPI::addComponent(UIGraphEdit* uiGraphEdit)
{
	ComponentVariant componentVariant;
	componentVariant.setToComponent(uiGraphEdit);
	this->getThisSceneTree2D().addComponent(this->getThisEntity2D().ID, componentVariant);
}
void Lilliputian::SceneAPI::addComponent(UIGraphNode* uiGraphNode)
{
	ComponentVariant componentVariant;
	componentVariant.setToComponent(uiGraphNode);
	this->getThisSceneTree2D().addComponent(this->getThisEntity2D().ID, componentVariant);
}
void Lilliputian::SceneAPI::addComponent(UIHoverCard* uiHoverCard)
{
	ComponentVariant componentVariant;
	componentVariant.setToComponent(uiHoverCard);
	this->getThisSceneTree2D().addComponent(this->getThisEntity2D().ID, componentVariant);
}
void Lilliputian::SceneAPI::addComponent(UIItemList* uiItemList)
{
	ComponentVariant componentVariant;
	componentVariant.setToComponent(uiItemList);
	this->getThisSceneTree2D().addComponent(this->getThisEntity2D().ID, componentVariant);
}
void Lilliputian::SceneAPI::addComponent(UIPanel* uiPanel)
{
	ComponentVariant componentVariant;
	componentVariant.setToComponent(uiPanel);
	this->getThisSceneTree2D().addComponent(this->getThisEntity2D().ID, componentVariant);
}
void Lilliputian::SceneAPI::addComponent(UIProgressBar* uiProgressBar)
{
	ComponentVariant componentVariant;
	componentVariant.setToComponent(uiProgressBar);
	this->getThisSceneTree2D().addComponent(this->getThisEntity2D().ID, componentVariant);
}
void Lilliputian::SceneAPI::addComponent(UIRectangle* uiRectangle)
{
	ComponentVariant componentVariant;
	componentVariant.setToComponent(uiRectangle);
	this->getThisSceneTree2D().addComponent(this->getThisEntity2D().ID, componentVariant);
}
void Lilliputian::SceneAPI::addComponent(UIScrollbar* uiScrollbar)
{
	ComponentVariant componentVariant;
	componentVariant.setToComponent(uiScrollbar);
	this->getThisSceneTree2D().addComponent(this->getThisEntity2D().ID, componentVariant);
}
void Lilliputian::SceneAPI::addComponent(UISeparator* uiSeparator)
{
	ComponentVariant componentVariant;
	componentVariant.setToComponent(uiSeparator);
	this->getThisSceneTree2D().addComponent(this->getThisEntity2D().ID, componentVariant);
}
void Lilliputian::SceneAPI::addComponent(UISlider* uiSlider)
{
	ComponentVariant componentVariant;
	componentVariant.setToComponent(uiSlider);
	this->getThisSceneTree2D().addComponent(this->getThisEntity2D().ID, componentVariant);
}
void Lilliputian::SceneAPI::addComponent(UISpinBox* uiSpinBox)
{
	ComponentVariant componentVariant;
	componentVariant.setToComponent(uiSpinBox);
	this->getThisSceneTree2D().addComponent(this->getThisEntity2D().ID, componentVariant);
}
void Lilliputian::SceneAPI::addComponent(UITab* uiTab)
{
	ComponentVariant componentVariant;
	componentVariant.setToComponent(uiTab);
	this->getThisSceneTree2D().addComponent(this->getThisEntity2D().ID, componentVariant);
}
void Lilliputian::SceneAPI::addComponent(UITextEdit* uiTextEdit)
{
	ComponentVariant componentVariant;
	componentVariant.setToComponent(uiTextEdit);
	this->getThisSceneTree2D().addComponent(this->getThisEntity2D().ID, componentVariant);
}
void Lilliputian::SceneAPI::addComponent(UITextLabel* uiTextLabel)
{
	ComponentVariant componentVariant;
	componentVariant.setToComponent(uiTextLabel);
	this->getThisSceneTree2D().addComponent(this->getThisEntity2D().ID, componentVariant);
}
void Lilliputian::SceneAPI::addComponent(UITree* uiTree)
{
	ComponentVariant componentVariant;
	componentVariant.setToComponent(uiTree);
	this->getThisSceneTree2D().addComponent(this->getThisEntity2D().ID, componentVariant);
}

Lilliputian::AIBehaviourTree& Lilliputian::SceneAPI::getAIBehaviourTree()
{
	AIBehaviourTree* aiBehaviourTree = nullptr;
	ComponentVariant::Type type = ComponentVariant::Type::AI_BEHAVIOUR_TREE;

	if (this->hasComponent(type))
		aiBehaviourTree = this->getComponentVariant(type).getAIBehaviourTree();

	return *aiBehaviourTree;
}

Lilliputian::AISightPerception2D& Lilliputian::SceneAPI::getAISightPerception2D()
{
	AISightPerception2D* aiSightPerception2D = nullptr;
	ComponentVariant::Type type = ComponentVariant::Type::AI_SIGHT_PERCEPTION;

	if (this->hasComponent(type))
		aiSightPerception2D = this->getComponentVariant(type).getAISightPerception2D();

	return *aiSightPerception2D;
}

Lilliputian::AISoundPerception2D& Lilliputian::SceneAPI::getAISoundPerception2D()
{
	AISoundPerception2D* aiSoundPerception2D = nullptr;
	ComponentVariant::Type type = ComponentVariant::Type::AI_SOUND_PERCEPTION;

	if (this->hasComponent(type))
		aiSoundPerception2D = this->getComponentVariant(type).getAISoundPerception2D();

	return *aiSoundPerception2D;
}

Lilliputian::AnimatedSprite& Lilliputian::SceneAPI::getAnimatedSprite()
{
	AnimatedSprite* animatedSprite = nullptr;
	ComponentVariant::Type type = ComponentVariant::Type::ANIMATED_SPRITE;

	if (this->hasComponent(type))
		animatedSprite = this->getComponentVariant(type).getAnimatedSprite();

	return *animatedSprite;
}

Lilliputian::AudioListener2D& Lilliputian::SceneAPI::getAudioListener2D()
{
	AudioListener2D* audioListener2D = nullptr;
	ComponentVariant::Type type = ComponentVariant::Type::AUDIO_LISTENER_2D;

	if (this->hasComponent(type))
		audioListener2D = this->getComponentVariant(type).getAudioListener2D();

	return *audioListener2D;
}

Lilliputian::AudioStreamSource2D& Lilliputian::SceneAPI::getAudioStreamSource2D()
{
	AudioStreamSource2D* audioStreamSource2D = nullptr;
	ComponentVariant::Type type = ComponentVariant::Type::AUDIO_STREAM_SOURCE_2D;

	if (this->hasComponent(type))
		audioStreamSource2D = this->getComponentVariant(type).getAudioStreamSource2D();

	return *audioStreamSource2D;
}

Lilliputian::Camera2D& Lilliputian::SceneAPI::getCamera2D()
{
	Camera2D* camera2D = nullptr;
	ComponentVariant::Type type = ComponentVariant::Type::CAMERA_2D;

	if (this->hasComponent(type))
		camera2D = this->getComponentVariant(type).getCamera2D();

	return *camera2D;
}

Lilliputian::CircleCollider2D& Lilliputian::SceneAPI::getCircleCollider2D()
{
	CircleCollider2D* circleCollider2D = nullptr;
	ComponentVariant::Type type = ComponentVariant::Type::CIRCLE_COLLIDER_2D;

	if (this->hasComponent(type))
		circleCollider2D = this->getComponentVariant(type).getCircleCollider2D();

	return *circleCollider2D;
}

Lilliputian::ConstantDirectionalForce2D& Lilliputian::SceneAPI::getConstantDirectionalForce2D()
{
	ConstantDirectionalForce2D* constantDirectionalForce2D = nullptr;
	ComponentVariant::Type type = ComponentVariant::Type::UI_TEXT_LABEL;

	if (this->hasComponent(type))
		constantDirectionalForce2D = this->getComponentVariant(type).getConstantDirectionalForce2D();

	return *constantDirectionalForce2D;
}

Lilliputian::ConstantPointForce2D& Lilliputian::SceneAPI::getConstantPointForce2D()
{
	ConstantPointForce2D* constantPointForce2D = nullptr;
	ComponentVariant::Type type = ComponentVariant::Type::CONSTANT_POINT_FORCE_2D;

	if (this->hasComponent(type))
		constantPointForce2D = this->getComponentVariant(type).getConstantPointForce2D();

	return *constantPointForce2D;
}

Lilliputian::CountdownTimer& Lilliputian::SceneAPI::getCountdownTimer()
{
	CountdownTimer* countdownTimer = nullptr;
	ComponentVariant::Type type = ComponentVariant::Type::COUNTDOWN_TIMER;

	if (this->hasComponent(type))
		countdownTimer = this->getComponentVariant(type).getCountdownTimer();

	return *countdownTimer;
}

Lilliputian::FixedTransform2D& Lilliputian::SceneAPI::getFixedTransform2D()
{
	FixedTransform2D* fixedTransform2D = nullptr;
	ComponentVariant::Type type = ComponentVariant::Type::TRANSFORM_2D;

	if (this->hasComponent(type))
		fixedTransform2D = this->getComponentVariant(type).getFixedTransform2D();

	return *fixedTransform2D;
}

Lilliputian::KinematicBody2D& Lilliputian::SceneAPI::getKinematicBody2D()
{
	KinematicBody2D* kinematicBody2D = nullptr;
	ComponentVariant::Type type = ComponentVariant::Type::KINEMATIC_BODY_2D;

	if (this->hasComponent(type))
		kinematicBody2D = this->getComponentVariant(type).getKinematicBody2D();

	return *kinematicBody2D;
}

Lilliputian::NavigationMeshAgent2D& Lilliputian::SceneAPI::getNavigationMeshAgent2D()
{
	NavigationMeshAgent2D* navigationMeshAgent2D = nullptr;
	ComponentVariant::Type type = ComponentVariant::Type::NAVIGATION_MESH_AGENT_2D;

	if (this->hasComponent(type))
		navigationMeshAgent2D = this->getComponentVariant(type).getNavigationMeshAgent2D();

	return *navigationMeshAgent2D;
}

Lilliputian::NavigationMeshBoxObstacle2D& Lilliputian::SceneAPI::getNavigationMeshBoxObstacle2D()
{
	NavigationMeshBoxObstacle2D* navigationMeshBoxObstacle2D = nullptr;
	ComponentVariant::Type type = ComponentVariant::Type::NAVIGATION_MESH_BOX_OBSTACLE_2D;

	if (this->hasComponent(type))
		navigationMeshBoxObstacle2D = this->getComponentVariant(type).getNavigationMeshBoxObstacle2D();

	return *navigationMeshBoxObstacle2D;
}

Lilliputian::NavigationPath2D& Lilliputian::SceneAPI::getNavigationPath2D()
{
	NavigationPath2D* navigationPath2D = nullptr;
	ComponentVariant::Type type = ComponentVariant::Type::NAVIGATION_PATH_2D;

	if (this->hasComponent(type))
		navigationPath2D = this->getComponentVariant(type).getNavigationPath2D();

	return *navigationPath2D;
}

Lilliputian::PhysicsConstraint2D& Lilliputian::SceneAPI::getPhysicsConstraint2D()
{
	PhysicsConstraint2D* physicsConstraint2D = nullptr;
	ComponentVariant::Type type = ComponentVariant::Type::PHYSICS_CONSTRAINT_2D;

	if (this->hasComponent(type))
		physicsConstraint2D = this->getComponentVariant(type).getPhysicsConstraint2D();

	return *physicsConstraint2D;
}

Lilliputian::PhysicsThruster2D& Lilliputian::SceneAPI::getPhysicsThruster2D()
{
	PhysicsThruster2D* physicsThruster2D = nullptr;
	ComponentVariant::Type type = ComponentVariant::Type::PHYSICS_THRUSTER_2D;

	if (this->hasComponent(type))
		physicsThruster2D = this->getComponentVariant(type).getPhysicsThruster2D();

	return *physicsThruster2D;
}

Lilliputian::PropertyAnimation& Lilliputian::SceneAPI::getPropertyAnimation()
{
	PropertyAnimation* propertyAnimation = nullptr;
	ComponentVariant::Type type = ComponentVariant::Type::PROPERTY_ANIMATION;

	if (this->hasComponent(type))
		propertyAnimation = this->getComponentVariant(type).getPropertyAnimation();

	return *propertyAnimation;
}

Lilliputian::RectangularCollider2D& Lilliputian::SceneAPI::getRectangularCollider2D()
{
	RectangularCollider2D* rectangularCollider2D = nullptr;
	ComponentVariant::Type type = ComponentVariant::Type::RECTANGULAR_COLIIDER_2D;

	if (this->hasComponent(type))
		rectangularCollider2D = this->getComponentVariant(type).getRectangularCollider2D();

	return *rectangularCollider2D;
}

Lilliputian::RectangularMesh2D& Lilliputian::SceneAPI::getRectangularMesh2D()
{
	RectangularMesh2D* rectangularMesh2D = nullptr;
	ComponentVariant::Type type = ComponentVariant::Type::RECTANGULAR_MESH_2D;

	if (this->hasComponent(type))
		rectangularMesh2D = this->getComponentVariant(type).getRectangularMesh2D();

	return *rectangularMesh2D;
}

Lilliputian::RectangularTriggerArea2D& Lilliputian::SceneAPI::getRectangularTriggerArea2D()
{
	RectangularTriggerArea2D* rectangularTriggerArea2D = nullptr;
	ComponentVariant::Type type = ComponentVariant::Type::RECTANGULAR_TRIGGER_AREA_2D;

	if (this->hasComponent(type))
		rectangularTriggerArea2D = this->getComponentVariant(type).getRectangularTriggerArea2D();

	return *rectangularTriggerArea2D;
}

Lilliputian::RegularPolygonalMesh2D& Lilliputian::SceneAPI::getRegularPolygonalMesh2D()
{
	RegularPolygonalMesh2D* regularPolygonalMesh2D = nullptr;
	ComponentVariant::Type type = ComponentVariant::Type::REGULAR_POLYGONAL_MESH_2D;

	if (this->hasComponent(type))
		regularPolygonalMesh2D = this->getComponentVariant(type).getRegularPolygonalMesh2D();

	return *regularPolygonalMesh2D;
}

Lilliputian::RigidBody2D& Lilliputian::SceneAPI::getRigidBody2D()
{
	RigidBody2D* rigidBody2D = nullptr;
	ComponentVariant::Type type = ComponentVariant::Type::RIGID_BODY_2D;

	if (this->hasComponent(type))
		rigidBody2D = this->getComponentVariant(type).getRigidBody2D();

	return *rigidBody2D;
}

Lilliputian::Sprite& Lilliputian::SceneAPI::getSprite()
{
	Sprite* sprite = nullptr;
	ComponentVariant::Type type = ComponentVariant::Type::SPRITE;

	if (this->hasComponent(type))
		sprite = this->getComponentVariant(type).getSprite();

	return *sprite;
}

Lilliputian::StaticFluid2D& Lilliputian::SceneAPI::getStaticFluid2D()
{
	StaticFluid2D* staticFluid2D = nullptr;
	ComponentVariant::Type type = ComponentVariant::Type::STATIC_FLUID_2D;

	if (this->hasComponent(type))
		staticFluid2D = this->getComponentVariant(type).getStaticFluid2D();

	return *staticFluid2D;
}

Lilliputian::Transform2D& Lilliputian::SceneAPI::getTransform2D()
{
	Transform2D* transform2D = nullptr;
	ComponentVariant::Type type = ComponentVariant::Type::TRANSFORM_2D;

	if (this->hasComponent(type))
		transform2D = this->getComponentVariant(type).getTransform2D();

	return *transform2D;
}

Lilliputian::UIButton& Lilliputian::SceneAPI::getUIButton()
{
	UIButton* uiButton = nullptr;
	ComponentVariant::Type type = ComponentVariant::Type::UI_BUTTON;

	if (this->hasComponent(type))
		uiButton = this->getComponentVariant(type).getUIButton();

	return *uiButton;
}

Lilliputian::UIGraphEdit& Lilliputian::SceneAPI::getUIGraphEdit()
{
	UIGraphEdit* uiGraphEdit = nullptr;
	ComponentVariant::Type type = ComponentVariant::Type::UI_GRAPH_EDIT;

	if (this->hasComponent(type))
		uiGraphEdit = this->getComponentVariant(type).getUIGraphEdit();

	return *uiGraphEdit;
}

Lilliputian::UIGraphNode& Lilliputian::SceneAPI::getUIGraphNode()
{
	UIGraphNode* uiGraphNode = nullptr;
	ComponentVariant::Type type = ComponentVariant::Type::UI_GRAPH_NODE;

	if (this->hasComponent(type))
		uiGraphNode = this->getComponentVariant(type).getUIGraphNode();

	return *uiGraphNode;
}

Lilliputian::UIHoverCard& Lilliputian::SceneAPI::getUIHoverCard()
{
	UIHoverCard* uiHoverCard = nullptr;
	ComponentVariant::Type type = ComponentVariant::Type::UI_HOVER_CARD;

	if (this->hasComponent(type))
		uiHoverCard = this->getComponentVariant(type).getUIHoverCard();

	return *uiHoverCard;
}

Lilliputian::UIItemList& Lilliputian::SceneAPI::getUIItemList()
{
	UIItemList* uiitemList = nullptr;
	ComponentVariant::Type type = ComponentVariant::Type::UI_ITEM_LIST;

	if (this->hasComponent(type))
		uiitemList = this->getComponentVariant(type).getUIItemList();

	return *uiitemList;
}

Lilliputian::UIPanel& Lilliputian::SceneAPI::getUIPanel()
{
	UIPanel* uiPanel = nullptr;
	ComponentVariant::Type type = ComponentVariant::Type::UI_PANEL;

	if (this->hasComponent(type))
		uiPanel = this->getComponentVariant(type).getUIPanel();

	return *uiPanel;
}

Lilliputian::UIProgressBar& Lilliputian::SceneAPI::getUIProgressBar()
{
	UIProgressBar* uiProgressBar = nullptr;
	ComponentVariant::Type type = ComponentVariant::Type::UI_PROGRESS_BAR;

	if (this->hasComponent(type))
		uiProgressBar = this->getComponentVariant(type).getUIProgressBar();

	return *uiProgressBar;
}

Lilliputian::UIRectangle& Lilliputian::SceneAPI::getUIRectangle()
{
	UIRectangle* uiRectangle = nullptr;
	ComponentVariant::Type type = ComponentVariant::Type::UI_RECTANGLE;

	if (this->hasComponent(type))
		uiRectangle = this->getComponentVariant(type).getUIRectangle();

	return *uiRectangle;
}

Lilliputian::UIScrollbar& Lilliputian::SceneAPI::getUIScrollbar()
{
	UIScrollbar* uiScrollbar = nullptr;
	ComponentVariant::Type type = ComponentVariant::Type::UI_SCROLLBAR;

	if (this->hasComponent(type))
		uiScrollbar = this->getComponentVariant(type).getUIScrollbar();

	return *uiScrollbar;
}

Lilliputian::UISeparator& Lilliputian::SceneAPI::getUISeparator()
{
	UISeparator* uiSeparator = nullptr;
	ComponentVariant::Type type = ComponentVariant::Type::UI_SEPARATOR;

	if (this->hasComponent(type))
		uiSeparator = this->getComponentVariant(type).getUISeparator();

	return *uiSeparator;
}


Lilliputian::UISlider& Lilliputian::SceneAPI::getUISlider()
{
	UISlider* uiSlider = nullptr;
	ComponentVariant::Type type = ComponentVariant::Type::UI_SLIDER;

	if (this->hasComponent(type))
		uiSlider = this->getComponentVariant(type).getUISlider();

	return *uiSlider;
}

Lilliputian::UISpinBox& Lilliputian::SceneAPI::getUISpinBox()
{
	UISpinBox* uiSpinBox = nullptr;
	ComponentVariant::Type type = ComponentVariant::Type::UI_SPIN_BOX;

	if (this->hasComponent(type))
		uiSpinBox = this->getComponentVariant(type).getUISpinBox();

	return *uiSpinBox;
}

Lilliputian::UITab& Lilliputian::SceneAPI::getUITab()
{
	UITab* uiTab = nullptr;
	ComponentVariant::Type type = ComponentVariant::Type::UI_TAB;

	if (this->hasComponent(type))
		uiTab = this->getComponentVariant(type).getUITab();

	return *uiTab;
}

Lilliputian::UITextEdit& Lilliputian::SceneAPI::getUITextEdit()
{
	UITextEdit* uiTextEdit = nullptr;
	ComponentVariant::Type type = ComponentVariant::Type::UI_TEXT_EDIT;

	if (this->hasComponent(type))
		uiTextEdit = this->getComponentVariant(type).getUITextEdit();

	return *uiTextEdit;
}

Lilliputian::UITextLabel& Lilliputian::SceneAPI::getUITextLabel()
{
	UITextLabel* uiTextLabel = nullptr;
	ComponentVariant::Type type = ComponentVariant::Type::UI_TEXT_LABEL;

	if (this->hasComponent(type))
		uiTextLabel = this->getComponentVariant(type).getUITextLabel();

	return *uiTextLabel;
}

Lilliputian::UITree& Lilliputian::SceneAPI::getUITree()
{
	UITree* uiTree = nullptr;
	ComponentVariant::Type type = ComponentVariant::Type::UI_TREE;

	if (this->hasComponent(type))
		uiTree = this->getComponentVariant(type).getUITree();

	return *uiTree;
}