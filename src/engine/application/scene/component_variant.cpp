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
#include <application/scene/id_counter.hpp>


Lilliputian::ComponentVariant::ComponentVariant()
{
	this->ID = IDCounter::getNewComponentID();
}

Lilliputian::ComponentVariant::ComponentVariant(const ComponentVariant& other)
{
	this->ID = other.ID;
	this->type = other.type;
	this->entityID = other.entityID;

	switch (other.type)
	{
		case Type::NONE: break;
		case Type::BEHAVIOUR_TREE: this->aiBehaviourTree = new BehaviourTree(*other.aiBehaviourTree); break;
		case Type::SIGHT_PERCEPTION: this->aiSightPerception2D = new SightPerception(*other.aiSightPerception2D); break;
		case Type::SOUND_PERCEPTION: this->aiSoundPerception2D = new SoundPerception(*other.aiSoundPerception2D); break;
		case Type::ANIMATED_SPRITE:this->animatedSprite = new AnimatedSprite(*other.animatedSprite); break;
		case Type::AUDIO_LISTENER:this->audioListener2D = new AudioListener(*other.audioListener2D); break;
		case Type::AUDIO_STREAM_SOURCE:this->audioStreamSource2D = new AudioStreamSource(*other.audioStreamSource2D); break;
		case Type::CAMERA: this->camera2D = new Camera(*other.camera2D); break;
		case Type::BALL_COLLIDER:this->circleCollider2D = new BallCollider(*other.circleCollider2D); break;
		case Type::CONSTANT_DIRECTIONAL_FORCE:this->constantDirectionalForce2D = new ConstantDirectionalForce(*other.constantDirectionalForce2D); break;
		case Type::CONSTANT_POINT_FORCE:this->constantPointForce2D = new ConstantPointForce(*other.constantPointForce2D); break;
		case Type::COUNTDOWN_TIMER:this->countdownTimer = new CountdownTimer(*other.countdownTimer); break;
		case Type::KINEMATIC_BODY:this->kinematicBody2D = new KinematicBody(*other.kinematicBody2D); break;
		case Type::NAVIGATION_MESH_AGENT:this->navigationMeshAgent2D = new NavigationMeshAgent(*other.navigationMeshAgent2D); break;
		case Type::NAVIGATION_MESH_BOX_OBSTACLE:this->navigationMeshBoxObstacle2D = new NavigationMeshBoxObstacle(*other.navigationMeshBoxObstacle2D); break;
		case Type::NAVIGATION_PATH:this->navigationPath2D = new NavigationPath(*other.navigationPath2D); break;
		case Type::PHYSICS_CONSTRAINT:this->physicsConstraint2D = new PhysicsConstraint(*other.physicsConstraint2D); break;
		case Type::PHYSICS_THRUSTER:this->physicsThruster2D = new PhysicsThruster(*other.physicsThruster2D); break;
		case Type::PROPERTY_ANIMATION:this->propertyAnimation = new PropertyAnimation(*other.propertyAnimation); break;
		case Type::BOX_COLLIDER:this->rectangularCollider2D = new BoxCollider(*other.rectangularCollider2D); break;
		case Type::BOX_MESH:this->rectangularMesh2D = new BoxMesh(*other.rectangularMesh2D); break;
		case Type::BOX_TRIGGER_SPACE:this->rectangularTriggerArea2D = new BoxTriggerSpace(*other.rectangularTriggerArea2D); break;
		case Type::REGULAR_POLYTOPAL_MESH:this->regularPolygonalMesh2D = new RegularPolytopalMesh(*other.regularPolygonalMesh2D); break;
		case Type::RIGID_BODY:this->rigidBody2D = new RigidBody(*other.rigidBody2D); break;
		case Type::SPRITE:this->sprite = new Sprite(*other.sprite); break;
		case Type::STATIC_FLUID:this->staticFluid2D = new StaticFluid(*other.staticFluid2D); break;
		case Type::TRANSFORM:this->transform2D = new Transform(*other.transform2D); break;
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
		case Type::BEHAVIOUR_TREE: delete this->aiBehaviourTree; break;
		case Type::SIGHT_PERCEPTION: delete this->aiSightPerception2D; break;
		case Type::SOUND_PERCEPTION: delete this->aiSoundPerception2D; break;
		case Type::ANIMATED_SPRITE:delete this->animatedSprite; break;
		case Type::AUDIO_LISTENER:delete this->audioListener2D; break;
		case Type::AUDIO_STREAM_SOURCE:delete this->audioStreamSource2D; break;
		case Type::CAMERA: delete this->camera2D; break;
		case Type::BALL_COLLIDER:delete this->circleCollider2D; break;
		case Type::CONSTANT_DIRECTIONAL_FORCE:delete this->constantDirectionalForce2D; break;
		case Type::CONSTANT_POINT_FORCE:delete this->constantPointForce2D; break;
		case Type::COUNTDOWN_TIMER:delete this->countdownTimer; break;
		case Type::KINEMATIC_BODY: delete this->kinematicBody2D; break;
		case Type::NAVIGATION_MESH_AGENT:delete this->navigationMeshAgent2D; break;
		case Type::NAVIGATION_MESH_BOX_OBSTACLE:delete this->navigationMeshBoxObstacle2D; break;
		case Type::NAVIGATION_PATH:delete this->navigationPath2D; break;
		case Type::PHYSICS_CONSTRAINT:delete this->physicsConstraint2D; break;
		case Type::PHYSICS_THRUSTER:delete this->physicsThruster2D; break;
		case Type::PROPERTY_ANIMATION:delete this->propertyAnimation; break;
		case Type::BOX_COLLIDER:delete this->rectangularCollider2D; break;
		case Type::BOX_MESH:delete this->rectangularMesh2D; break;
		case Type::BOX_TRIGGER_SPACE:delete this->rectangularTriggerArea2D; break;
		case Type::REGULAR_POLYTOPAL_MESH:delete this->regularPolygonalMesh2D; break;
		case Type::RIGID_BODY:delete this->rigidBody2D; break;
		case Type::SPRITE:delete this->sprite; break;
		case Type::STATIC_FLUID:delete this->staticFluid2D; break;
		case Type::TRANSFORM: delete this->transform2D; break;
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

void Lilliputian::ComponentVariant::setTo(BehaviourTree* aiBehaviourTree)
{
	if (this->type == Type::NONE)
	{
		this->aiBehaviourTree = aiBehaviourTree;
		this->type = Type::BEHAVIOUR_TREE;
	}
}

void Lilliputian::ComponentVariant::setTo(SightPerception* aiSightPerception2D)
{
	if (this->type == Type::NONE)
	{
		this->aiSightPerception2D = aiSightPerception2D;
		this->type = Type::SIGHT_PERCEPTION;
	}
}

void Lilliputian::ComponentVariant::setTo(SoundPerception* aiSoundPerception2D)
{
	if (this->type == Type::NONE)
	{
		this->aiSoundPerception2D = aiSoundPerception2D;
		this->type = Type::SOUND_PERCEPTION;
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

void Lilliputian::ComponentVariant::setTo(AudioListener* audioListener2D)
{
	if (this->type == Type::NONE)
	{
		this->audioListener2D = audioListener2D;
		this->type = Type::AUDIO_LISTENER;
	}
}

void Lilliputian::ComponentVariant::setTo(Camera* camera2D)
{
	if (this->type == Type::NONE)
	{
		this->camera2D = camera2D;
		this->type = Type::CAMERA;
	}
}

void Lilliputian::ComponentVariant::setTo(BallCollider* circleCollider2D)
{
	if (this->type == Type::NONE)
	{
		this->circleCollider2D = circleCollider2D;
		this->type = Type::BALL_COLLIDER;
	}
}

void Lilliputian::ComponentVariant::setTo(ConstantDirectionalForce* constantDirectionalForce2D)
{
	if (this->type == Type::NONE)
	{
		this->constantDirectionalForce2D = constantDirectionalForce2D;
		this->type = Type::CONSTANT_DIRECTIONAL_FORCE;
	}
}

void Lilliputian::ComponentVariant::setTo(ConstantPointForce* constantPointForce2D)
{
	if (this->type == Type::NONE)
	{
		this->constantPointForce2D = constantPointForce2D;
		this->type = Type::CONSTANT_POINT_FORCE;
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

void Lilliputian::ComponentVariant::setTo(KinematicBody* kinematicBody2D)
{
	if (this->type == Type::NONE)
	{
		this->kinematicBody2D = kinematicBody2D;
		this->type = Type::KINEMATIC_BODY;
	}
}

void Lilliputian::ComponentVariant::setTo(NavigationMeshAgent* navigationMeshAgent2D)
{
	if (this->type == Type::NONE)
	{
		this->navigationMeshAgent2D = navigationMeshAgent2D;
		this->type = Type::NAVIGATION_MESH_AGENT;
	}
}

void Lilliputian::ComponentVariant::setTo(NavigationMeshBoxObstacle* navigationMeshBoxObstacle2D)
{
	if (this->type == Type::NONE)
	{
		this->navigationMeshBoxObstacle2D = navigationMeshBoxObstacle2D;
		this->type = Type::NAVIGATION_MESH_BOX_OBSTACLE;
	}
}

void Lilliputian::ComponentVariant::setTo(NavigationPath* navigationPath2D)
{
	if (this->type == Type::NONE)
	{
		this->navigationPath2D = navigationPath2D;
		this->type = Type::NAVIGATION_PATH;
	}
}

void Lilliputian::ComponentVariant::setTo(PhysicsConstraint* physicsConstraint2D)
{
	if (this->type == Type::NONE)
	{
		this->physicsConstraint2D = physicsConstraint2D;
		this->type = Type::PHYSICS_CONSTRAINT;
	}
}

void Lilliputian::ComponentVariant::setTo(PhysicsThruster* physicsThruster2D)
{
	if (this->type == Type::NONE)
	{
		this->physicsThruster2D = physicsThruster2D;
		this->type = Type::PHYSICS_THRUSTER;
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

void Lilliputian::ComponentVariant::setTo(BoxCollider* rectangularCollider2D)
{
	if (this->type == Type::NONE)
	{
		this->rectangularCollider2D = rectangularCollider2D;
		this->type = Type::BOX_COLLIDER;
	}
}

void Lilliputian::ComponentVariant::setTo(BoxMesh* rectangularMesh2D)
{
	if (this->type == Type::NONE)
	{
		this->rectangularMesh2D = rectangularMesh2D;
		this->type = Type::BOX_MESH;
	}
}

void Lilliputian::ComponentVariant::setTo(BoxTriggerSpace* rectangularTriggerArea2D)
{
	if (this->type == Type::NONE)
	{
		this->rectangularTriggerArea2D = rectangularTriggerArea2D;
		this->type = Type::BOX_TRIGGER_SPACE;
	}
}

void Lilliputian::ComponentVariant::setTo(RegularPolytopalMesh* regularPolygonalMesh2D)
{
	if (this->type == Type::NONE)
	{
		this->regularPolygonalMesh2D = regularPolygonalMesh2D;
		this->type = Type::REGULAR_POLYTOPAL_MESH;
	}
}

void Lilliputian::ComponentVariant::setTo(RigidBody* rigidBody2D)
{
	if (this->type == Type::NONE)
	{
		this->rigidBody2D = rigidBody2D;
		this->type = Type::RIGID_BODY;
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

void Lilliputian::ComponentVariant::setTo(StaticFluid* staticFluid2D)
{
	if (this->type == Type::NONE)
	{
		this->staticFluid2D = staticFluid2D;
		this->type = Type::STATIC_FLUID;
	}
}

void Lilliputian::ComponentVariant::setTo(Transform* transform2D)
{
	if (this->type == Type::NONE)
	{
		this->transform2D = transform2D;
		this->type = Type::TRANSFORM;
	}
}

void Lilliputian::ComponentVariant::setTo(AudioStreamSource* audioStreamSource2D)
{
	if (this->type == Type::NONE)
	{
		this->audioStreamSource2D = audioStreamSource2D;
		this->type = Type::AUDIO_STREAM_SOURCE;
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
		this->type == Type::BOX_MESH ||
		this->type == Type::REGULAR_POLYTOPAL_MESH ||
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

Lilliputian::BehaviourTree* Lilliputian::ComponentVariant::getAIBehaviourTree()
{
	return this->aiBehaviourTree;
}
Lilliputian::SightPerception* Lilliputian::ComponentVariant::getAISightPerception2D()
{
	return this->aiSightPerception2D;
}
Lilliputian::SoundPerception* Lilliputian::ComponentVariant::getAISoundPerception2D()
{
	return this->aiSoundPerception2D;
}
Lilliputian::AnimatedSprite* Lilliputian::ComponentVariant::getAnimatedSprite()
{
	return this->animatedSprite;
}
Lilliputian::AudioListener* Lilliputian::ComponentVariant::getAudioListener2D()
{
	return this->audioListener2D;
}
Lilliputian::Camera* Lilliputian::ComponentVariant::getCamera2D()
{
	return this->camera2D;
}
Lilliputian::BallCollider* Lilliputian::ComponentVariant::getCircleCollider2D()
{
	return this->circleCollider2D;
}
Lilliputian::ConstantDirectionalForce* Lilliputian::ComponentVariant::getConstantDirectionalForce2D()
{
	return this->constantDirectionalForce2D;
}
Lilliputian::ConstantPointForce* Lilliputian::ComponentVariant::getConstantPointForce2D()
{
	return this->constantPointForce2D;
}
Lilliputian::CountdownTimer* Lilliputian::ComponentVariant::getCountdownTimer()
{
	return this->countdownTimer;
}
Lilliputian::KinematicBody* Lilliputian::ComponentVariant::getKinematicBody2D()
{
	return this->kinematicBody2D;
}
Lilliputian::NavigationMeshAgent* Lilliputian::ComponentVariant::getNavigationMeshAgent2D()
{
	return this->navigationMeshAgent2D;
}
Lilliputian::NavigationMeshBoxObstacle* Lilliputian::ComponentVariant::getNavigationMeshBoxObstacle2D()
{
	return this->navigationMeshBoxObstacle2D;
}
Lilliputian::NavigationPath* Lilliputian::ComponentVariant::getNavigationPath2D()
{
	return this->navigationPath2D;
}
Lilliputian::PhysicsConstraint* Lilliputian::ComponentVariant::getPhysicsConstraint2D()
{
	return this->physicsConstraint2D;
}
Lilliputian::PhysicsThruster* Lilliputian::ComponentVariant::getPhysicsThruster2D()
{
	return this->physicsThruster2D;
}
Lilliputian::PropertyAnimation* Lilliputian::ComponentVariant::getPropertyAnimation()
{
	return this->propertyAnimation;
}
Lilliputian::BoxCollider* Lilliputian::ComponentVariant::getRectangularCollider2D()
{
	return this->rectangularCollider2D;
}
Lilliputian::BoxMesh* Lilliputian::ComponentVariant::getRectangularMesh2D()
{
	return this->rectangularMesh2D;
}
Lilliputian::BoxTriggerSpace* Lilliputian::ComponentVariant::getRectangularTriggerArea2D()
{
	return this->rectangularTriggerArea2D;
}
Lilliputian::RegularPolytopalMesh* Lilliputian::ComponentVariant::getRegularPolygonalMesh2D()
{
	return this->regularPolygonalMesh2D;
}
Lilliputian::RigidBody* Lilliputian::ComponentVariant::getRigidBody2D()
{
	return this->rigidBody2D;
}
Lilliputian::Sprite* Lilliputian::ComponentVariant::getSprite()
{
	return this->sprite;
}
Lilliputian::StaticFluid* Lilliputian::ComponentVariant::getStaticFluid2D()
{
	return this->staticFluid2D;
}
Lilliputian::Transform* Lilliputian::ComponentVariant::getTransform2D()
{
	return this->transform2D;
}
Lilliputian::AudioStreamSource* Lilliputian::ComponentVariant::getAudioStreamSource2D()
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
		case ComponentVariant::Type::BOX_MESH: return (this->rectangularMesh2D->getImage()); break;
		case ComponentVariant::Type::REGULAR_POLYTOPAL_MESH: return (this->regularPolygonalMesh2D->getImage()); break;
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