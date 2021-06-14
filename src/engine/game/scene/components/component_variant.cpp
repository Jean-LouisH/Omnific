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

#include "component_variant.hpp"
#include <game/scene/id_counter.hpp>


Lilliputian::ComponentVariant::ComponentVariant()
{
	this->ID = IDCounter::getNewID();
}

Lilliputian::ComponentVariant::ComponentVariant(const ComponentVariant& other)
{
	this->ID = other.ID;
	this->type = other.type;
	this->entityID = other.entityID;

	switch (other.type)
	{
		case Type::NONE: break;
		case Type::AI_BEHAVIOUR_TREE: this->aiBehaviourTree = new AIBehaviourTree(*other.aiBehaviourTree); break;
		case Type::AI_SIGHT_PERCEPTION: this->aiSightPerception2D = new AISightPerception2D(*other.aiSightPerception2D); break;
		case Type::AI_SOUND_PERCEPTION: this->aiSoundPerception2D = new AISoundPerception2D(*other.aiSoundPerception2D); break;
		case Type::ANIMATED_SPRITE:this->animatedSprite = new AnimatedSprite(*other.animatedSprite); break;
		case Type::AUDIO_LISTENER_2D:this->audioListener2D = new AudioListener2D(*other.audioListener2D); break;
		case Type::AUDIO_STREAM_SOURCE_2D:this->audioStreamSource2D = new AudioStreamSource2D(*other.audioStreamSource2D); break;
		case Type::CAMERA_2D: this->camera2D = new Camera2D(*other.camera2D); break;
		case Type::CIRCLE_COLLIDER_2D:this->circleCollider2D = new CircleCollider2D(*other.circleCollider2D); break;
		case Type::CONSTANT_DIRECTIONAL_FORCE_2D:this->constantDirectionalForce2D = new ConstantDirectionalForce2D(*other.constantDirectionalForce2D); break;
		case Type::CONSTANT_POINT_FORCE_2D:this->constantPointForce2D = new ConstantPointForce2D(*other.constantPointForce2D); break;
		case Type::COUNTDOWN_TIMER:this->countdownTimer = new CountdownTimer(*other.countdownTimer); break;
		case Type::FIXED_TRANSFORM_2D:this->fixedTransform2D = new FixedTransform2D(*other.fixedTransform2D); break;
		case Type::KINEMATIC_BODY_2D:this->kinematicBody2D = new KinematicBody2D(*other.kinematicBody2D); break;
		case Type::NAVIGATION_MESH_AGENT_2D:this->navigationMeshAgent2D = new NavigationMeshAgent2D(*other.navigationMeshAgent2D); break;
		case Type::NAVIGATION_MESH_BOX_OBSTACLE_2D:this->navigationMeshBoxObstacle2D = new NavigationMeshBoxObstacle2D(*other.navigationMeshBoxObstacle2D); break;
		case Type::NAVIGATION_PATH_2D:this->navigationPath2D = new NavigationPath2D(*other.navigationPath2D); break;
		case Type::PHYSICS_CONSTRAINT_2D:this->physicsConstraint2D = new PhysicsConstraint2D(*other.physicsConstraint2D); break;
		case Type::PHYSICS_THRUSTER_2D:this->physicsThruster2D = new PhysicsThruster2D(*other.physicsThruster2D); break;
		case Type::PROPERTY_ANIMATION:this->propertyAnimation = new PropertyAnimation(*other.propertyAnimation); break;
		case Type::RECTANGULAR_COLIIDER_2D:this->rectangularCollider2D = new RectangularCollider2D(*other.rectangularCollider2D); break;
		case Type::RECTANGULAR_MESH_2D:this->rectangularMesh2D = new RectangularMesh2D(*other.rectangularMesh2D); break;
		case Type::RECTANGULAR_TRIGGER_AREA_2D:this->rectangularTriggerArea2D = new RectangularTriggerArea2D(*other.rectangularTriggerArea2D); break;
		case Type::REGULAR_POLYGONAL_MESH_2D:this->regularPolygonalMesh2D = new RegularPolygonalMesh2D(*other.regularPolygonalMesh2D); break;
		case Type::RIGID_BODY_2D:this->rigidBody2D = new RigidBody2D(*other.rigidBody2D); break;
		case Type::SPRITE:this->sprite = new Sprite(*other.sprite); break;
		case Type::STATIC_FLUID_2D:this->staticFluid2D = new StaticFluid2D(*other.staticFluid2D); break;
		case Type::TRANSFORM_2D:this->transform2D = new Transform2D(*other.transform2D); break;
		case Type::UI_BUTTON:this->uiButton = new UIButton(*other.uiButton); break;
		case Type::UI_RECTANGLE:this->uiRectangle = new UIRectangle(*other.uiRectangle); break;
		case Type::UI_GRAPH_EDIT:this->uiGraphEdit = new UIGraphEdit(*other.uiGraphEdit); break;
		case Type::UI_GRAPH_NODE:this->uiGraphNode = new UIGraphNode(*other.uiGraphNode); break;
		case Type::UI_SCROLLBAR:this->uiScrollbar = new UIScrollbar(*other.uiScrollbar); break;
		case Type::UI_SEPARATOR:this->uiSeparator = new UISeparator(*other.uiSeparator); break;
		case Type::UI_SLIDER:this->uiSlider = new UISlider(*other.uiSlider); break;
		case Type::UI_HOVER_CARD:this->uiHoverCard = new UIHoverCard(*other.uiHoverCard); break;
		case Type::UI_ITEM_LIST:this->uiItemList = new UIItemList(*other.uiItemList); break;
		case Type::UI_PANEL:this->uiPanel = new UIPanel(*other.uiPanel); break;
		case Type::UI_PROGRESS_BAR:this->uiProgressBar = new UIProgressBar(*other.uiProgressBar); break;
		case Type::UI_SPIN_BOX:this->uiSpinBox = new UISpinBox(*other.uiSpinBox); break;
		case Type::UI_TAB:this->uiTab = new UITab(*other.uiTab); break;
		case Type::UI_TEXT_EDIT:this->uiTextEdit = new UITextEdit(*other.uiTextEdit); break;
		case Type::UI_TEXT_LABEL:this->uiTextLabel = new UITextLabel(*other.uiTextLabel); break;
		case Type::UI_TREE:this->uiTree = new UITree(*other.uiTree); break;
	}
}

Lilliputian::ComponentVariant::~ComponentVariant()
{
	switch (this->type)
	{
		case Type::AI_BEHAVIOUR_TREE: delete this->aiBehaviourTree; break;
		case Type::AI_SIGHT_PERCEPTION: delete this->aiSightPerception2D; break;
		case Type::AI_SOUND_PERCEPTION: delete this->aiSoundPerception2D; break;
		case Type::ANIMATED_SPRITE:delete this->animatedSprite; break;
		case Type::AUDIO_LISTENER_2D:delete this->audioListener2D; break;
		case Type::AUDIO_STREAM_SOURCE_2D:delete this->audioStreamSource2D; break;
		case Type::CAMERA_2D: delete this->camera2D; break;
		case Type::CIRCLE_COLLIDER_2D:delete this->circleCollider2D; break;
		case Type::CONSTANT_DIRECTIONAL_FORCE_2D:delete this->constantDirectionalForce2D; break;
		case Type::CONSTANT_POINT_FORCE_2D:delete this->constantPointForce2D; break;
		case Type::COUNTDOWN_TIMER:delete this->countdownTimer; break;
		case Type::FIXED_TRANSFORM_2D:delete this->fixedTransform2D; break;
		case Type::KINEMATIC_BODY_2D: delete this->kinematicBody2D; break;
		case Type::NAVIGATION_MESH_AGENT_2D:delete this->navigationMeshAgent2D; break;
		case Type::NAVIGATION_MESH_BOX_OBSTACLE_2D:delete this->navigationMeshBoxObstacle2D; break;
		case Type::NAVIGATION_PATH_2D:delete this->navigationPath2D; break;
		case Type::PHYSICS_CONSTRAINT_2D:delete this->physicsConstraint2D; break;
		case Type::PHYSICS_THRUSTER_2D:delete this->physicsThruster2D; break;
		case Type::PROPERTY_ANIMATION:delete this->propertyAnimation; break;
		case Type::RECTANGULAR_COLIIDER_2D:delete this->rectangularCollider2D; break;
		case Type::RECTANGULAR_MESH_2D:delete this->rectangularMesh2D; break;
		case Type::RECTANGULAR_TRIGGER_AREA_2D:delete this->rectangularTriggerArea2D; break;
		case Type::REGULAR_POLYGONAL_MESH_2D:delete this->regularPolygonalMesh2D; break;
		case Type::RIGID_BODY_2D:delete this->rigidBody2D; break;
		case Type::SPRITE:delete this->sprite; break;
		case Type::STATIC_FLUID_2D:delete this->staticFluid2D; break;
		case Type::TRANSFORM_2D: delete this->transform2D; break;
		case Type::UI_BUTTON:delete this->uiButton; break;
		case Type::UI_RECTANGLE:delete this->uiRectangle; break;
		case Type::UI_GRAPH_EDIT:delete this->uiGraphEdit; break;
		case Type::UI_GRAPH_NODE:delete this->uiGraphNode; break;
		case Type::UI_SCROLLBAR:delete this->uiScrollbar; break;
		case Type::UI_SEPARATOR:delete this->uiSeparator; break;
		case Type::UI_SLIDER:delete this->uiSlider; break;
		case Type::UI_HOVER_CARD:delete this->uiHoverCard; break;
		case Type::UI_ITEM_LIST:delete this->uiItemList; break;
		case Type::UI_PANEL:delete this->uiPanel; break;
		case Type::UI_PROGRESS_BAR:delete this->uiProgressBar; break;
		case Type::UI_SPIN_BOX:delete this->uiSpinBox; break;
		case Type::UI_TAB:delete this->uiTab; break;
		case Type::UI_TEXT_EDIT:delete this->uiTextEdit; break;
		case Type::UI_TEXT_LABEL:delete this->uiTextLabel; break;
		case Type::UI_TREE:delete this->uiTree; break;
	}
}

void Lilliputian::ComponentVariant::setEntityID(EntityID entityID)
{
	this->entityID = entityID;
}

void Lilliputian::ComponentVariant::setTo(AIBehaviourTree* aiBehaviourTree)
{
	if (this->type == Type::NONE)
	{
		this->aiBehaviourTree = aiBehaviourTree;
		this->type = Type::AI_BEHAVIOUR_TREE;
	}
}

void Lilliputian::ComponentVariant::setTo(AISightPerception2D* aiSightPerception2D)
{
	if (this->type == Type::NONE)
	{
		this->aiSightPerception2D = aiSightPerception2D;
		this->type = Type::AI_SIGHT_PERCEPTION;
	}
}

void Lilliputian::ComponentVariant::setTo(AISoundPerception2D* aiSoundPerception2D)
{
	if (this->type == Type::NONE)
	{
		this->aiSoundPerception2D = aiSoundPerception2D;
		this->type = Type::AI_SOUND_PERCEPTION;
	}
}

void Lilliputian::ComponentVariant::setTo(AnimatedSprite* animatedSprite)
{
	if (this->type == Type::NONE)
	{
		this->animatedSprite = animatedSprite;
		this->type = Type::ANIMATED_SPRITE;
	}
}

void Lilliputian::ComponentVariant::setTo(AudioListener2D* audioListener2D)
{
	if (this->type == Type::NONE)
	{
		this->audioListener2D = audioListener2D;
		this->type = Type::AUDIO_LISTENER_2D;
	}
}

void Lilliputian::ComponentVariant::setTo(Camera2D* camera2D)
{
	if (this->type == Type::NONE)
	{
		this->camera2D = camera2D;
		this->type = Type::CAMERA_2D;
	}
}

void Lilliputian::ComponentVariant::setTo(CircleCollider2D* circleCollider2D)
{
	if (this->type == Type::NONE)
	{
		this->circleCollider2D = circleCollider2D;
		this->type = Type::CIRCLE_COLLIDER_2D;
	}
}

void Lilliputian::ComponentVariant::setTo(ConstantDirectionalForce2D* constantDirectionalForce2D)
{
	if (this->type == Type::NONE)
	{
		this->constantDirectionalForce2D = constantDirectionalForce2D;
		this->type = Type::CONSTANT_DIRECTIONAL_FORCE_2D;
	}
}

void Lilliputian::ComponentVariant::setTo(ConstantPointForce2D* constantPointForce2D)
{
	if (this->type == Type::NONE)
	{
		this->constantPointForce2D = constantPointForce2D;
		this->type = Type::CONSTANT_POINT_FORCE_2D;
	}
}

void Lilliputian::ComponentVariant::setTo(CountdownTimer* countdownTimer)
{
	if (this->type == Type::NONE)
	{
		this->countdownTimer = countdownTimer;
		this->type = Type::COUNTDOWN_TIMER;
	}
}

void Lilliputian::ComponentVariant::setTo(FixedTransform2D* fixedTransform2D)
{
	if (this->type == Type::NONE)
	{
		this->fixedTransform2D = fixedTransform2D;
		this->type = Type::FIXED_TRANSFORM_2D;
	}
}

void Lilliputian::ComponentVariant::setTo(KinematicBody2D* kinematicBody2D)
{
	if (this->type == Type::NONE)
	{
		this->kinematicBody2D = kinematicBody2D;
		this->type = Type::KINEMATIC_BODY_2D;
	}
}

void Lilliputian::ComponentVariant::setTo(NavigationMeshAgent2D* navigationMeshAgent2D)
{
	if (this->type == Type::NONE)
	{
		this->navigationMeshAgent2D = navigationMeshAgent2D;
		this->type = Type::NAVIGATION_MESH_AGENT_2D;
	}
}

void Lilliputian::ComponentVariant::setTo(NavigationMeshBoxObstacle2D* navigationMeshBoxObstacle2D)
{
	if (this->type == Type::NONE)
	{
		this->navigationMeshBoxObstacle2D = navigationMeshBoxObstacle2D;
		this->type = Type::NAVIGATION_MESH_BOX_OBSTACLE_2D;
	}
}

void Lilliputian::ComponentVariant::setTo(NavigationPath2D* navigationPath2D)
{
	if (this->type == Type::NONE)
	{
		this->navigationPath2D = navigationPath2D;
		this->type = Type::NAVIGATION_PATH_2D;
	}
}

void Lilliputian::ComponentVariant::setTo(PhysicsConstraint2D* physicsConstraint2D)
{
	if (this->type == Type::NONE)
	{
		this->physicsConstraint2D = physicsConstraint2D;
		this->type = Type::PHYSICS_CONSTRAINT_2D;
	}
}

void Lilliputian::ComponentVariant::setTo(PhysicsThruster2D* physicsThruster2D)
{
	if (this->type == Type::NONE)
	{
		this->physicsThruster2D = physicsThruster2D;
		this->type = Type::PHYSICS_THRUSTER_2D;
	}
}

void Lilliputian::ComponentVariant::setTo(PropertyAnimation* propertyAnimation)
{
	if (this->type == Type::NONE)
	{
		this->propertyAnimation = propertyAnimation;
		this->type = Type::PROPERTY_ANIMATION;
	}
}

void Lilliputian::ComponentVariant::setTo(RectangularCollider2D* rectangularCollider2D)
{
	if (this->type == Type::NONE)
	{
		this->rectangularCollider2D = rectangularCollider2D;
		this->type = Type::RECTANGULAR_COLIIDER_2D;
	}
}

void Lilliputian::ComponentVariant::setTo(RectangularMesh2D* rectangularMesh2D)
{
	if (this->type == Type::NONE)
	{
		this->rectangularMesh2D = rectangularMesh2D;
		this->type = Type::RECTANGULAR_MESH_2D;
	}
}

void Lilliputian::ComponentVariant::setTo(RectangularTriggerArea2D* rectangularTriggerArea2D)
{
	if (this->type == Type::NONE)
	{
		this->rectangularTriggerArea2D = rectangularTriggerArea2D;
		this->type = Type::RECTANGULAR_TRIGGER_AREA_2D;
	}
}

void Lilliputian::ComponentVariant::setTo(RegularPolygonalMesh2D* regularPolygonalMesh2D)
{
	if (this->type == Type::NONE)
	{
		this->regularPolygonalMesh2D = regularPolygonalMesh2D;
		this->type = Type::REGULAR_POLYGONAL_MESH_2D;
	}
}

void Lilliputian::ComponentVariant::setTo(RigidBody2D* rigidBody2D)
{
	if (this->type == Type::NONE)
	{
		this->rigidBody2D = rigidBody2D;
		this->type = Type::RIGID_BODY_2D;
	}
}

void Lilliputian::ComponentVariant::setTo(Sprite* sprite)
{
	if (this->type == Type::NONE)
	{
		this->sprite = sprite;
		this->type = Type::SPRITE;
	}
}

void Lilliputian::ComponentVariant::setTo(StaticFluid2D* staticFluid2D)
{
	if (this->type == Type::NONE)
	{
		this->staticFluid2D = staticFluid2D;
		this->type = Type::STATIC_FLUID_2D;
	}
}

void Lilliputian::ComponentVariant::setTo(Transform2D* transform2D)
{
	if (this->type == Type::NONE)
	{
		this->transform2D = transform2D;
		this->type = Type::TRANSFORM_2D;
	}
}

void Lilliputian::ComponentVariant::setTo(AudioStreamSource2D* audioStreamSource2D)
{
	if (this->type == Type::NONE)
	{
		this->audioStreamSource2D = audioStreamSource2D;
		this->type = Type::AUDIO_STREAM_SOURCE_2D;
	}
}

void Lilliputian::ComponentVariant::setTo(UIButton* uiButton)
{
	if (this->type == Type::NONE)
	{
		this->uiButton = uiButton;
		this->type = Type::UI_BUTTON;
	}
}

void Lilliputian::ComponentVariant::setTo(UIRectangle* uiRectangle)
{
	if (this->type == Type::NONE)
	{
		this->uiRectangle = uiRectangle;
		this->type = Type::UI_RECTANGLE;
	}
}

void Lilliputian::ComponentVariant::setTo(UIGraphEdit* uiGraphEdit)
{
	if (this->type == Type::NONE)
	{
		this->uiGraphEdit = uiGraphEdit;
		this->type = Type::UI_GRAPH_EDIT;
	}
}

void Lilliputian::ComponentVariant::setTo(UIGraphNode* uiGraphNode)
{
	if (this->type == Type::NONE)
	{
		this->uiGraphNode = uiGraphNode;
		this->type = Type::UI_GRAPH_NODE;
	}
}

void Lilliputian::ComponentVariant::setTo(UIScrollbar* uiScrollbar)
{
	if (this->type == Type::NONE)
	{
		this->uiScrollbar = uiScrollbar;
		this->type = Type::UI_SCROLLBAR;
	}
}

void Lilliputian::ComponentVariant::setTo(UISeparator* uiSeparator)
{
	if (this->type == Type::NONE)
	{
		this->uiSeparator = uiSeparator;
		this->type = Type::UI_SEPARATOR;
	}
}

void Lilliputian::ComponentVariant::setTo(UISlider* uiSlider)
{
	if (this->type == Type::NONE)
	{
		this->uiSlider = uiSlider;
		this->type = Type::UI_SLIDER;
	}
}

void Lilliputian::ComponentVariant::setTo(UIHoverCard* uiHoverCard)
{
	if (this->type == Type::NONE)
	{
		this->uiHoverCard = uiHoverCard;
		this->type = Type::UI_HOVER_CARD;
	}
}

void Lilliputian::ComponentVariant::setTo(UIItemList* uiItemList)
{
	if (this->type == Type::NONE)
	{
		this->uiItemList = uiItemList;
		this->type = Type::UI_ITEM_LIST;
	}
}

void Lilliputian::ComponentVariant::setTo(UIPanel* uiPanel)
{
	if (this->type == Type::NONE)
	{
		this->uiPanel = uiPanel;
		this->type = Type::UI_PANEL;
	}
}

void Lilliputian::ComponentVariant::setTo(UIProgressBar* uiProgressBar)
{
	if (this->type == Type::NONE)
	{
		this->uiProgressBar = uiProgressBar;
		this->type = Type::UI_PROGRESS_BAR;
	}
}

void Lilliputian::ComponentVariant::setTo(UISpinBox* uiSpinBox)
{
	if (this->type == Type::NONE)
	{
		this->uiSpinBox = uiSpinBox;
		this->type = Type::UI_SPIN_BOX;
	}
}

void Lilliputian::ComponentVariant::setTo(UITab* uiTab)
{
	if (this->type == Type::NONE)
	{
		this->uiTab = uiTab;
		this->type = Type::UI_TAB;
	}
}

void Lilliputian::ComponentVariant::setTo(UITextEdit* uiTextEdit)
{
	if (this->type == Type::NONE)
	{
		this->uiTextEdit = uiTextEdit;
		this->type = Type::UI_TEXT_EDIT;
	}
}

void Lilliputian::ComponentVariant::setTo(UITextLabel* uiTextLabel)
{
	if (this->type == Type::NONE)
	{
		this->uiTextLabel = uiTextLabel;
		this->type = Type::UI_TEXT_LABEL;
	}
}

void Lilliputian::ComponentVariant::setTo(UITree* uiTree)
{
	if (this->type == Type::NONE)
	{
		this->uiTree = uiTree;
		this->type = Type::UI_TREE;
	}
}

bool Lilliputian::ComponentVariant::isRenderable()
{
	return this->type == Type::SPRITE ||
		this->type == Type::ANIMATED_SPRITE ||
		this->type == Type::RECTANGULAR_MESH_2D ||
		this->type == Type::REGULAR_POLYGONAL_MESH_2D ||
		this->type == Type::UI_BUTTON ||
		this->type == Type::UI_RECTANGLE ||
		this->type == Type::UI_GRAPH_EDIT ||
		this->type == Type::UI_GRAPH_NODE ||
		this->type == Type::UI_SCROLLBAR ||
		this->type == Type::UI_SEPARATOR ||
		this->type == Type::UI_SLIDER ||
		this->type == Type::UI_HOVER_CARD ||
		this->type == Type::UI_ITEM_LIST ||
		this->type == Type::UI_PANEL ||
		this->type == Type::UI_PROGRESS_BAR ||
		this->type == Type::UI_SPIN_BOX ||
		this->type == Type::UI_TAB ||
		this->type == Type::UI_TEXT_EDIT ||
		this->type == Type::UI_TEXT_LABEL ||
		this->type == Type::UI_TREE;
}

Lilliputian::AIBehaviourTree* Lilliputian::ComponentVariant::getAIBehaviourTree()
{
	return this->aiBehaviourTree;
}
Lilliputian::AISightPerception2D* Lilliputian::ComponentVariant::getAISightPerception2D()
{
	return this->aiSightPerception2D;
}
Lilliputian::AISoundPerception2D* Lilliputian::ComponentVariant::getAISoundPerception2D()
{
	return this->aiSoundPerception2D;
}
Lilliputian::AnimatedSprite* Lilliputian::ComponentVariant::getAnimatedSprite()
{
	return this->animatedSprite;
}
Lilliputian::AudioListener2D* Lilliputian::ComponentVariant::getAudioListener2D()
{
	return this->audioListener2D;
}
Lilliputian::Camera2D* Lilliputian::ComponentVariant::getCamera2D()
{
	return this->camera2D;
}
Lilliputian::CircleCollider2D* Lilliputian::ComponentVariant::getCircleCollider2D()
{
	return this->circleCollider2D;
}
Lilliputian::ConstantDirectionalForce2D* Lilliputian::ComponentVariant::getConstantDirectionalForce2D()
{
	return this->constantDirectionalForce2D;
}
Lilliputian::ConstantPointForce2D* Lilliputian::ComponentVariant::getConstantPointForce2D()
{
	return this->constantPointForce2D;
}
Lilliputian::CountdownTimer* Lilliputian::ComponentVariant::getCountdownTimer()
{
	return this->countdownTimer;
}
Lilliputian::FixedTransform2D* Lilliputian::ComponentVariant::getFixedTransform2D()
{
	return this->fixedTransform2D;
}
Lilliputian::KinematicBody2D* Lilliputian::ComponentVariant::getKinematicBody2D()
{
	return this->kinematicBody2D;
}
Lilliputian::NavigationMeshAgent2D* Lilliputian::ComponentVariant::getNavigationMeshAgent2D()
{
	return this->navigationMeshAgent2D;
}
Lilliputian::NavigationMeshBoxObstacle2D* Lilliputian::ComponentVariant::getNavigationMeshBoxObstacle2D()
{
	return this->navigationMeshBoxObstacle2D;
}
Lilliputian::NavigationPath2D* Lilliputian::ComponentVariant::getNavigationPath2D()
{
	return this->navigationPath2D;
}
Lilliputian::PhysicsConstraint2D* Lilliputian::ComponentVariant::getPhysicsConstraint2D()
{
	return this->physicsConstraint2D;
}
Lilliputian::PhysicsThruster2D* Lilliputian::ComponentVariant::getPhysicsThruster2D()
{
	return this->physicsThruster2D;
}
Lilliputian::PropertyAnimation* Lilliputian::ComponentVariant::getPropertyAnimation()
{
	return this->propertyAnimation;
}
Lilliputian::RectangularCollider2D* Lilliputian::ComponentVariant::getRectangularCollider2D()
{
	return this->rectangularCollider2D;
}
Lilliputian::RectangularMesh2D* Lilliputian::ComponentVariant::getRectangularMesh2D()
{
	return this->rectangularMesh2D;
}
Lilliputian::RectangularTriggerArea2D* Lilliputian::ComponentVariant::getRectangularTriggerArea2D()
{
	return this->rectangularTriggerArea2D;
}
Lilliputian::RegularPolygonalMesh2D* Lilliputian::ComponentVariant::getRegularPolygonalMesh2D()
{
	return this->regularPolygonalMesh2D;
}
Lilliputian::RigidBody2D* Lilliputian::ComponentVariant::getRigidBody2D()
{
	return this->rigidBody2D;
}
Lilliputian::Sprite* Lilliputian::ComponentVariant::getSprite()
{
	return this->sprite;
}
Lilliputian::StaticFluid2D* Lilliputian::ComponentVariant::getStaticFluid2D()
{
	return this->staticFluid2D;
}
Lilliputian::Transform2D* Lilliputian::ComponentVariant::getTransform2D()
{
	return this->transform2D;
}
Lilliputian::AudioStreamSource2D* Lilliputian::ComponentVariant::getAudioStreamSource2D()
{
	return this->audioStreamSource2D;
}
Lilliputian::UIButton* Lilliputian::ComponentVariant::getUIButton()
{
	return this->uiButton;
}
Lilliputian::UIRectangle* Lilliputian::ComponentVariant::getUIRectangle()
{
	return this->uiRectangle;
}
Lilliputian::UIGraphEdit* Lilliputian::ComponentVariant::getUIGraphEdit()
{
	return this->uiGraphEdit;
}
Lilliputian::UIGraphNode* Lilliputian::ComponentVariant::getUIGraphNode()
{
	return this->uiGraphNode;
}
Lilliputian::UIScrollbar* Lilliputian::ComponentVariant::getUIScrollbar()
{
	return this->uiScrollbar;
}
Lilliputian::UISeparator* Lilliputian::ComponentVariant::getUISeparator()
{
	return this->uiSeparator;
}
Lilliputian::UISlider* Lilliputian::ComponentVariant::getUISlider()
{
	return this->uiSlider;
}
Lilliputian::UIHoverCard* Lilliputian::ComponentVariant::getUIHoverCard()
{
	return this->uiHoverCard;
}
Lilliputian::UIItemList* Lilliputian::ComponentVariant::getUIItemList()
{
	return this->uiItemList;
}
Lilliputian::UIPanel* Lilliputian::ComponentVariant::getUIPanel()
{
	return this->uiPanel;
}
Lilliputian::UIProgressBar* Lilliputian::ComponentVariant::getUIProgressBar()
{
	return this->uiProgressBar;
}
Lilliputian::UISpinBox* Lilliputian::ComponentVariant::getUISpinBox()
{
	return this->uiSpinBox;
}
Lilliputian::UITab* Lilliputian::ComponentVariant::getUITab()
{
	return this->uiTab;
}
Lilliputian::UITextEdit* Lilliputian::ComponentVariant::getUITextEdit()
{
	return this->uiTextEdit;
}
Lilliputian::UITextLabel* Lilliputian::ComponentVariant::getUITextLabel()
{
	return this->uiTextLabel;
}
Lilliputian::UITree* Lilliputian::ComponentVariant::getUITree()
{
	return this->uiTree;
}

Lilliputian::ComponentID Lilliputian::ComponentVariant::getID()
{
	return this->ID;
}

Lilliputian::ComponentVariant::Type Lilliputian::ComponentVariant::getType()
{
	return this->type;
}

Lilliputian::EntityID Lilliputian::ComponentVariant::getEntityID()
{
	return this->entityID;
}

Lilliputian::Image& Lilliputian::ComponentVariant::getImage()
{
	Image* image = nullptr;

	switch (this->type)
	{
		case ComponentVariant::Type::SPRITE: return (this->sprite->getImage()); break;
		case ComponentVariant::Type::ANIMATED_SPRITE: return (this->animatedSprite->getCurrentFrame()); break;
		case ComponentVariant::Type::RECTANGULAR_MESH_2D: return (this->rectangularMesh2D->getImage()); break;
		case ComponentVariant::Type::REGULAR_POLYGONAL_MESH_2D: return (this->regularPolygonalMesh2D->getImage()); break;
		case ComponentVariant::Type::UI_BUTTON: return (this->uiButton->getImage()); break;
		case ComponentVariant::Type::UI_RECTANGLE: return (this->uiRectangle->getImage()); break;
		case ComponentVariant::Type::UI_GRAPH_EDIT: return (this->uiGraphEdit->getImage()); break;
		case ComponentVariant::Type::UI_GRAPH_NODE: return (this->uiGraphNode->getImage()); break;
		case ComponentVariant::Type::UI_SCROLLBAR: return (this->uiScrollbar->getImage()); break;
		case ComponentVariant::Type::UI_SEPARATOR: return (this->uiSeparator->getImage()); break;
		case ComponentVariant::Type::UI_SLIDER: return (this->uiSlider->getImage()); break;
		case ComponentVariant::Type::UI_HOVER_CARD: return (this->uiHoverCard->getImage()); break;
		case ComponentVariant::Type::UI_ITEM_LIST: return (this->uiItemList->getImage()); break;
		case ComponentVariant::Type::UI_PANEL: return (this->uiPanel->getImage()); break;
		case ComponentVariant::Type::UI_PROGRESS_BAR: return (this->uiProgressBar->getImage()); break;
		case ComponentVariant::Type::UI_SPIN_BOX: return (this->uiSpinBox->getImage()); break;
		case ComponentVariant::Type::UI_TAB: return (this->uiTab->getImage()); break;
		case ComponentVariant::Type::UI_TEXT_EDIT: return (this->uiTextEdit->getImage()); break;
		case ComponentVariant::Type::UI_TEXT_LABEL: return (this->uiTextLabel->getImage()); break;
		case ComponentVariant::Type::UI_TREE:; return (this->uiTree->getImage()); break;
	}

	return *image;
}

void Lilliputian::ComponentVariant::unloadImage()
{
	Image* image = &this->getImage();

	if (image != nullptr)
		image->unload();
}