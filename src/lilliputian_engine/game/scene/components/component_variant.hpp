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
        Component::ComponentType type = Component::ComponentType::COMPONENT_TYPE_NONE;
		EntityID entityID = NO_ENTITY;

		ComponentVariant() = default;
		ComponentVariant(const ComponentVariant& componentVariant)
		{

		}
		~ComponentVariant()
		{

		}

        union
        {
			AIBehaviourTree aiBehaviourTree;
			AISightPerception2D aiSightPerception2D;
			AISoundPerception2D aiSoundPerception2D;
			AnimatedSprite animatedSprite2D;
			AudioListener2D audioListener2D;
			Camera2D camera2D;
			CircleCollider2D circleCollider2D;
			ConstantDirectionalForce2D constantDirectionalForce2D;
			ConstantPointForce2D constantPointForce2D;
			CountdownTimer countdownTimer;
			FixedTransform2D fixedTransform2D;
			NavigationMeshAgent2D navigationMeshAgent2D;
			NavigationMeshBoxObstacle2D navigationMeshBoxObstacle2D;
			NavigationPath2D navigationPath2D;
			NeuralNetwork neuralNetwork;
			PhysicsConstraint2D physicsConstraint2D;
			PhysicsThruster2D physicsThruster2D;
			PropertyAnimation propertyAnimation;
			RectangularCollider2D rectangularCollider2D;
			RectangularMesh2D rectangularMesh2D;
			RectangularTriggerArea2D rectangularTriggerArea2D;
			RegularPolygonalMesh2D regularPolygonalMesh2D;
			RigidBody2D rigidBody2D;
			Sprite sprite;
			StaticFluid2D staticFluid2D;
			StreamedAudioSource2D streamedAudioSource2D;
			UIButton uiButton;
			UIColouredRectangle uiColouredRectangle;
			UIGraphEdit uiGraphEdit;
			UIGraphNode uiGraphNode;
			UIHorizontalScrollbar uiHorizontalScrollbar;
			UIHorizontalSeparator uiHorizontalSeparator;
			UIHorizontalSlider uiHorizontalSlider;
			UIHoverCard uiHoverCard;
			UIItemList uiItemList;
			UILinkButton uiLinkButton;
			UIPanel uiPanel;
			UIProgressBar uiProgressBar;
			UIRichTextLabel uiRichTextLabel;
			UISpinBox uiSpinBox;
			UITab uiTab;
			UITextEdit uiTextEdit;
			UITextLabel uiTextLabel;
			UITexturedButton uiTexturedButton;
			UITexturedProgressBar uiTexturedProgressBar;
			UITexturedRectangle uiTexturedRectangle;
			UITree uiTree;
			UIVerticalScrollbar uiVerticalScrollbar;
			UIVerticalSeparator uiVerticalSeparator;
			UIVerticalSlider uiVerticalSlider;
        };
    };
}