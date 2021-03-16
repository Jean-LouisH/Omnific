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
		void addTransform(EntityID entityID);
		void addComponent(EntityID entityID, ComponentVariant componentVariant);

		void executeFrameLogic();

		Vector<ComponentVariant>& getComponentVariants();
		Transform2D& getEntityTransform(EntityID entityID);
	private:

		Map<EntityID, Entity2D> entities2D;
		Vector<Transform2D> transforms2D;
		Vector<ComponentVariant> componentVariants;

		Rectangle windowDimensions;
		ComponentID currentCamera = 0;
	};
}