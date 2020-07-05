#pragma once

#include "../Utilities/Aliases.hpp"
#include "../Utilities/Vector2.hpp"

namespace Lilliputian
{
	class RigidBody2D
	{
	private:
		EntityID entityID;
	public:
		double mass_kg;
		double friction_ratio;
		double angularDrag_ratio;
		double angularVelocity_rad_per_s;
		double gravity_scale;
		double elasticity_ratio;
		Vector2 drag_ratio;
		Vector2 velocity_px_per_s;

		RigidBody2D(EntityID entityID);
		EntityID getEntityID();
	};
}