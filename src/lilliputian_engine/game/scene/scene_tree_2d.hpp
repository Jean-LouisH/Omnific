#pragma once

#include "entity_2d.hpp"
#include "components/component_variant.hpp"
#include "utilities/rectangle.hpp"
#include "utilities/collections/vector.hpp"
#include "utilities/collections/map.hpp"
#include "utilities/aliases.hpp"

namespace Lilliputian
{
	class SceneTree2D
	{
	public:
		SceneTree2D()
		{
			this->windowDimensions.width = 0;
			this->windowDimensions.height = 0;
		}

		void addEntity2D(Entity2D entity2D);
		void addComponent(EntityID entityID, ComponentVariant componentVariant);

		void executeFrameLogic();
	private:

		Map<EntityID, Entity2D> entities2D;

		Vector<AIBehaviourTree> aiBehaviourTrees;
		Vector<AISightPerception2D> aiSightPerceptions2D;
		Vector<AISoundPerception2D> aiSoundPerceptions2D;
		Vector<AnimatedSprite> animatedSprites2D;
		Vector<AudioListener2D> audioListeners2D;
		Vector<Camera2D> cameras2D;
		Vector<CircleCollider2D> circleColliders2D;
		Vector<ConstantDirectionalForce2D> constantDirectionalForces2D;
		Vector<ConstantPointForce2D> constantPointForces2D;
		Vector<CountdownTimer> countdownTimers;
		Vector<FixedTransform2D> fixedTransforms2D;
		Vector<NavigationMeshAgent2D> navigationMeshAgents2D;
		Vector<NavigationMeshBoxObstacle2D> navigationMeshBoxObstacles2D;
		Vector<NavigationPath2D> navigationPaths2D;
		Vector<NeuralNetwork> neuralNetworks;
		Vector<PhysicsConstraint2D> physicsConstraints2D;
		Vector<PhysicsThruster2D> physicsThrusters2D;
		Vector<PropertyAnimation> propertyAnimations;
		Vector<RectangularCollider2D> rectangularColliders2D;
		Vector<RectangularMesh2D> rectangularMeshes2D;
		Vector<RectangularTriggerArea2D> rectangularTriggerAreas2D;
		Vector<RegularPolygonalMesh2D> regularPolygonalMeshes2D;
		Vector<RigidBody2D> rigidBodies2D;
		Vector<Sprite> sprites;
		Vector<StaticFluid2D> staticFluids2D;
		Vector<StreamedAudioSource2D> streamedAudioSources2D;
		Vector<UIButton> uiButtons;
		Vector<UIColouredRectangle> uiColouredRectangles;
		Vector<UIGraphEdit> uiGraphEdits;
		Vector<UIGraphNode> uiGraphNodes;
		Vector<UIHorizontalScrollbar> uiHorizontalScrollbars;
		Vector<UIHorizontalSeparator> uiHorizontalSeparators;
		Vector<UIHorizontalSlider> uiHorizontalSliders;
		Vector<UIHoverCard> uiHoverCards;
		Vector<UIItemList> uiItemLists;
		Vector<UILinkButton> uiLinkButtons;
		Vector<UIPanel> uiPanels;
		Vector<UIProgressBar> uiProgressBars;
		Vector<UIRichTextLabel> uiRichTextLabels;
		Vector<UISpinBox> uiSpinBoxes;
		Vector<UITab> uiTabs;
		Vector<UITextEdit> uiTextEdits;
		Vector<UITextLabel> uiTextLabels;
		Vector<UITexturedButton> uiTexturedButtons;
		Vector<UITexturedProgressBar> uiTexturedProgressBars;
		Vector<UITexturedRectangle> uiTexturedRectangles;
		Vector<UITree> uiTrees;
		Vector<UIVerticalScrollbar> uiVerticalScrollbars;
		Vector<UIVerticalSeparator> uiVerticalSeparators;
		Vector<UIVerticalSlider> uiVerticalSliders;

		Rectangle windowDimensions;
		ComponentID currentCamera = 0;
	};
}