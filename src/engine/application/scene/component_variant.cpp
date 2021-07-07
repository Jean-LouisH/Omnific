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
		case Type::BEHAVIOUR_TREE: this->behaviourTree = new BehaviourTree(*other.behaviourTree); break;
		case Type::SIGHT_PERCEPTION: this->sightPerception = new SightPerception(*other.sightPerception); break;
		case Type::SOUND_PERCEPTION: this->soundPerception = new SoundPerception(*other.soundPerception); break;
		case Type::ANIMATED_SPRITE:this->animatedSprite = new AnimatedSprite(*other.animatedSprite); break;
		case Type::AUDIO_LISTENER:this->audioListener = new AudioListener(*other.audioListener); break;
		case Type::AUDIO_STREAM_SOURCE:this->audioStreamSource = new AudioStreamSource(*other.audioStreamSource); break;
		case Type::CAMERA: this->camera = new Camera(*other.camera); break;
		case Type::BALL_COLLIDER:this->ballCollider = new BallCollider(*other.ballCollider); break;
		case Type::CONSTANT_DIRECTIONAL_FORCE:this->constantDirectionalForce = new ConstantDirectionalForce(*other.constantDirectionalForce); break;
		case Type::CONSTANT_POINT_FORCE:this->constantPointForce = new ConstantPointForce(*other.constantPointForce); break;
		case Type::COUNTDOWN_TIMER:this->countdownTimer = new CountdownTimer(*other.countdownTimer); break;
		case Type::KINEMATIC_BODY:this->kinematicBody = new KinematicBody(*other.kinematicBody); break;
		case Type::NAVIGATION_MESH_AGENT:this->navigationMeshAgent = new NavigationMeshAgent(*other.navigationMeshAgent); break;
		case Type::NAVIGATION_MESH_BOX_OBSTACLE:this->navigationMeshBoxObstacle = new NavigationMeshBoxObstacle(*other.navigationMeshBoxObstacle); break;
		case Type::NAVIGATION_PATH:this->navigationPath = new NavigationPath(*other.navigationPath); break;
		case Type::PHYSICS_CONSTRAINT:this->physicsConstraint = new PhysicsConstraint(*other.physicsConstraint); break;
		case Type::PHYSICS_THRUSTER:this->physicsThruster = new PhysicsThruster(*other.physicsThruster); break;
		case Type::PROPERTY_ANIMATION:this->propertyAnimation = new PropertyAnimation(*other.propertyAnimation); break;
		case Type::BOX_COLLIDER:this->boxCollider = new BoxCollider(*other.boxCollider); break;
		case Type::BOX_MESH:this->boxMesh = new BoxMesh(*other.boxMesh); break;
		case Type::BOX_TRIGGER_SPACE:this->boxTriggerSpace = new BoxTriggerSpace(*other.boxTriggerSpace); break;
		case Type::REGULAR_POLYTOPAL_MESH:this->regularPolytopalMesh = new RegularPolytopalMesh(*other.regularPolytopalMesh); break;
		case Type::RIGID_BODY:this->rigidBody = new RigidBody(*other.rigidBody); break;
		case Type::SPRITE:this->sprite = new Sprite(*other.sprite); break;
		case Type::STATIC_FLUID:this->staticFluid = new StaticFluid(*other.staticFluid); break;
		case Type::TRANSFORM:this->transform = new Transform(*other.transform); break;
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
		case Type::BEHAVIOUR_TREE: delete this->behaviourTree; break;
		case Type::SIGHT_PERCEPTION: delete this->sightPerception; break;
		case Type::SOUND_PERCEPTION: delete this->soundPerception; break;
		case Type::ANIMATED_SPRITE:delete this->animatedSprite; break;
		case Type::AUDIO_LISTENER:delete this->audioListener; break;
		case Type::AUDIO_STREAM_SOURCE:delete this->audioStreamSource; break;
		case Type::CAMERA: delete this->camera; break;
		case Type::BALL_COLLIDER:delete this->ballCollider; break;
		case Type::CONSTANT_DIRECTIONAL_FORCE:delete this->constantDirectionalForce; break;
		case Type::CONSTANT_POINT_FORCE:delete this->constantPointForce; break;
		case Type::COUNTDOWN_TIMER:delete this->countdownTimer; break;
		case Type::KINEMATIC_BODY: delete this->kinematicBody; break;
		case Type::NAVIGATION_MESH_AGENT:delete this->navigationMeshAgent; break;
		case Type::NAVIGATION_MESH_BOX_OBSTACLE:delete this->navigationMeshBoxObstacle; break;
		case Type::NAVIGATION_PATH:delete this->navigationPath; break;
		case Type::PHYSICS_CONSTRAINT:delete this->physicsConstraint; break;
		case Type::PHYSICS_THRUSTER:delete this->physicsThruster; break;
		case Type::PROPERTY_ANIMATION:delete this->propertyAnimation; break;
		case Type::BOX_COLLIDER:delete this->boxCollider; break;
		case Type::BOX_MESH:delete this->boxMesh; break;
		case Type::BOX_TRIGGER_SPACE:delete this->boxTriggerSpace; break;
		case Type::REGULAR_POLYTOPAL_MESH:delete this->regularPolytopalMesh; break;
		case Type::RIGID_BODY:delete this->rigidBody; break;
		case Type::SPRITE:delete this->sprite; break;
		case Type::STATIC_FLUID:delete this->staticFluid; break;
		case Type::TRANSFORM: delete this->transform; break;
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
		this->behaviourTree = aiBehaviourTree;
		this->type = Type::BEHAVIOUR_TREE;
	}
}

void Lilliputian::ComponentVariant::setTo(SightPerception* aiSightPerception2D)
{
	if (this->type == Type::NONE)
	{
		this->sightPerception = aiSightPerception2D;
		this->type = Type::SIGHT_PERCEPTION;
	}
}

void Lilliputian::ComponentVariant::setTo(SoundPerception* aiSoundPerception2D)
{
	if (this->type == Type::NONE)
	{
		this->soundPerception = aiSoundPerception2D;
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
		this->audioListener = audioListener2D;
		this->type = Type::AUDIO_LISTENER;
	}
}

void Lilliputian::ComponentVariant::setTo(Camera* camera2D)
{
	if (this->type == Type::NONE)
	{
		this->camera = camera2D;
		this->type = Type::CAMERA;
	}
}

void Lilliputian::ComponentVariant::setTo(BallCollider* circleCollider2D)
{
	if (this->type == Type::NONE)
	{
		this->ballCollider = circleCollider2D;
		this->type = Type::BALL_COLLIDER;
	}
}

void Lilliputian::ComponentVariant::setTo(ConstantDirectionalForce* constantDirectionalForce2D)
{
	if (this->type == Type::NONE)
	{
		this->constantDirectionalForce = constantDirectionalForce2D;
		this->type = Type::CONSTANT_DIRECTIONAL_FORCE;
	}
}

void Lilliputian::ComponentVariant::setTo(ConstantPointForce* constantPointForce2D)
{
	if (this->type == Type::NONE)
	{
		this->constantPointForce = constantPointForce2D;
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
		this->kinematicBody = kinematicBody2D;
		this->type = Type::KINEMATIC_BODY;
	}
}

void Lilliputian::ComponentVariant::setTo(NavigationMeshAgent* navigationMeshAgent2D)
{
	if (this->type == Type::NONE)
	{
		this->navigationMeshAgent = navigationMeshAgent2D;
		this->type = Type::NAVIGATION_MESH_AGENT;
	}
}

void Lilliputian::ComponentVariant::setTo(NavigationMeshBoxObstacle* navigationMeshBoxObstacle2D)
{
	if (this->type == Type::NONE)
	{
		this->navigationMeshBoxObstacle = navigationMeshBoxObstacle2D;
		this->type = Type::NAVIGATION_MESH_BOX_OBSTACLE;
	}
}

void Lilliputian::ComponentVariant::setTo(NavigationPath* navigationPath2D)
{
	if (this->type == Type::NONE)
	{
		this->navigationPath = navigationPath2D;
		this->type = Type::NAVIGATION_PATH;
	}
}

void Lilliputian::ComponentVariant::setTo(PhysicsConstraint* physicsConstraint2D)
{
	if (this->type == Type::NONE)
	{
		this->physicsConstraint = physicsConstraint2D;
		this->type = Type::PHYSICS_CONSTRAINT;
	}
}

void Lilliputian::ComponentVariant::setTo(PhysicsThruster* physicsThruster2D)
{
	if (this->type == Type::NONE)
	{
		this->physicsThruster = physicsThruster2D;
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
		this->boxCollider = rectangularCollider2D;
		this->type = Type::BOX_COLLIDER;
	}
}

void Lilliputian::ComponentVariant::setTo(BoxMesh* rectangularMesh2D)
{
	if (this->type == Type::NONE)
	{
		this->boxMesh = rectangularMesh2D;
		this->type = Type::BOX_MESH;
	}
}

void Lilliputian::ComponentVariant::setTo(BoxTriggerSpace* rectangularTriggerArea2D)
{
	if (this->type == Type::NONE)
	{
		this->boxTriggerSpace = rectangularTriggerArea2D;
		this->type = Type::BOX_TRIGGER_SPACE;
	}
}

void Lilliputian::ComponentVariant::setTo(RegularPolytopalMesh* regularPolygonalMesh2D)
{
	if (this->type == Type::NONE)
	{
		this->regularPolytopalMesh = regularPolygonalMesh2D;
		this->type = Type::REGULAR_POLYTOPAL_MESH;
	}
}

void Lilliputian::ComponentVariant::setTo(RigidBody* rigidBody2D)
{
	if (this->type == Type::NONE)
	{
		this->rigidBody = rigidBody2D;
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
		this->staticFluid = staticFluid2D;
		this->type = Type::STATIC_FLUID;
	}
}

void Lilliputian::ComponentVariant::setTo(Transform* transform2D)
{
	if (this->type == Type::NONE)
	{
		this->transform = transform2D;
		this->type = Type::TRANSFORM;
	}
}

void Lilliputian::ComponentVariant::setTo(AudioStreamSource* audioStreamSource2D)
{
	if (this->type == Type::NONE)
	{
		this->audioStreamSource = audioStreamSource2D;
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

Lilliputian::BehaviourTree* Lilliputian::ComponentVariant::getBehaviourTree()
{
	return this->behaviourTree;
}
Lilliputian::SightPerception* Lilliputian::ComponentVariant::getSightPerception()
{
	return this->sightPerception;
}
Lilliputian::SoundPerception* Lilliputian::ComponentVariant::getSoundPerception()
{
	return this->soundPerception;
}
Lilliputian::AnimatedSprite* Lilliputian::ComponentVariant::getAnimatedSprite()
{
	return this->animatedSprite;
}
Lilliputian::AudioListener* Lilliputian::ComponentVariant::getAudioListener()
{
	return this->audioListener;
}
Lilliputian::Camera* Lilliputian::ComponentVariant::getCamera()
{
	return this->camera;
}
Lilliputian::BallCollider* Lilliputian::ComponentVariant::getBallCollider()
{
	return this->ballCollider;
}
Lilliputian::ConstantDirectionalForce* Lilliputian::ComponentVariant::getConstantDirectionalForce()
{
	return this->constantDirectionalForce;
}
Lilliputian::ConstantPointForce* Lilliputian::ComponentVariant::getConstantPointForce()
{
	return this->constantPointForce;
}
Lilliputian::CountdownTimer* Lilliputian::ComponentVariant::getCountdownTimer()
{
	return this->countdownTimer;
}
Lilliputian::KinematicBody* Lilliputian::ComponentVariant::getKinematicBody()
{
	return this->kinematicBody;
}
Lilliputian::NavigationMeshAgent* Lilliputian::ComponentVariant::getNavigationMeshAgent()
{
	return this->navigationMeshAgent;
}
Lilliputian::NavigationMeshBoxObstacle* Lilliputian::ComponentVariant::getNavigationMeshBoxObstacle()
{
	return this->navigationMeshBoxObstacle;
}
Lilliputian::NavigationPath* Lilliputian::ComponentVariant::getNavigationPath()
{
	return this->navigationPath;
}
Lilliputian::PhysicsConstraint* Lilliputian::ComponentVariant::getPhysicsConstraint()
{
	return this->physicsConstraint;
}
Lilliputian::PhysicsThruster* Lilliputian::ComponentVariant::getPhysicsThruster()
{
	return this->physicsThruster;
}
Lilliputian::PropertyAnimation* Lilliputian::ComponentVariant::getPropertyAnimation()
{
	return this->propertyAnimation;
}
Lilliputian::BoxCollider* Lilliputian::ComponentVariant::getBoxCollider()
{
	return this->boxCollider;
}
Lilliputian::BoxMesh* Lilliputian::ComponentVariant::getBoxMesh()
{
	return this->boxMesh;
}
Lilliputian::BoxTriggerSpace* Lilliputian::ComponentVariant::getBoxTriggerSpace()
{
	return this->boxTriggerSpace;
}
Lilliputian::RegularPolytopalMesh* Lilliputian::ComponentVariant::getRegularPolytopalMesh()
{
	return this->regularPolytopalMesh;
}
Lilliputian::RigidBody* Lilliputian::ComponentVariant::getRigidBody()
{
	return this->rigidBody;
}
Lilliputian::Sprite* Lilliputian::ComponentVariant::getSprite()
{
	return this->sprite;
}
Lilliputian::StaticFluid* Lilliputian::ComponentVariant::getStaticFluid()
{
	return this->staticFluid;
}
Lilliputian::Transform* Lilliputian::ComponentVariant::getTransform()
{
	return this->transform;
}
Lilliputian::AudioStreamSource* Lilliputian::ComponentVariant::getAudioStreamSource()
{
	return this->audioStreamSource;
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
		case ComponentVariant::Type::BOX_MESH: return (this->boxMesh->getImage()); break;
		case ComponentVariant::Type::REGULAR_POLYTOPAL_MESH: return (this->regularPolytopalMesh->getImage()); break;
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