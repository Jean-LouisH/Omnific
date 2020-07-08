#pragma once

#include "Entity2D.hpp"
#include "AudioListener2D.hpp"
#include "AudioSource2D.hpp"
#include "BoxCollider2D.hpp"
#include "Camera2D.hpp"
#include "PropertyAnimation.hpp"
#include "RigidBody2D.hpp"
#include "Sprite2D.hpp"
#include "../Utilities/Rectangle.hpp"
#include "../Utilities/Collections/Vector.hpp"

namespace Lilliputian
{
	class Subscene2D
	{
	private:
		Vector<Entity2D> entities2D;
		Vector<AudioListener2D> audioListeners2D;
		Vector<AudioSource2D> audioSources2D;
		Vector<BoxCollider2D> boxColliders2D;
		Vector<Camera2D> cameras2D;
		Vector<PropertyAnimation> propertyAnimations;
		Vector<RigidBody2D> rigidBodies2D;
		Vector<Sprite2D> sprites2D;
		Rectangle windowDimensions;
	public:
		Subscene2D();
		void addEntity2D(Entity2D entity2D);
		void executeFrameLogic();
	};
}