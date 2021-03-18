#include "scene_tree_2d.hpp"

void Lilliputian::SceneTree2D::addEntity2D(Entity2D entity2D)
{
	this->entities2D.emplace(entity2D.ID, entity2D);
}

void Lilliputian::SceneTree2D::addComponent(EntityID entityID, ComponentVariant componentVariant)
{
	componentVariant.entityID = entityID;
	this->componentVariants.push_back(componentVariant);
	this->entities2D.at(entityID).addComponent(componentVariant.type, this->componentVariants.size() - 1);

	if (componentVariant.type == ComponentVariant::COMPONENT_TYPE_TRANSFORM_2D)
		this->transform2DIndexCache.push_back(this->componentVariants.size() - 1);
	else if (componentVariant.type == ComponentVariant::COMPONENT_TYPE_CAMERA_2D)
		if (componentVariant.camera2D->getIsStreaming())
			this->currentCamera = this->componentVariants.size() - 1;
}

void Lilliputian::SceneTree2D::executeFrameLogic()
{
	for (int i = 0; i < this->entities2D.size(); i++)
	{

	}
}

Lilliputian::ComponentID Lilliputian::SceneTree2D::getCurrentCameraIndex()
{
	return this->currentCamera;
}

Lilliputian::Vector<Lilliputian::ComponentVariant>& Lilliputian::SceneTree2D::getComponentVariants()
{
	return this->componentVariants;
}

Lilliputian::Transform2D& Lilliputian::SceneTree2D::getEntityTransform(EntityID entityID)
{
	Transform2D* transform2D = new Transform2D();

	for (int i = 0; i < this->transform2DIndexCache.size(); i++)
	{
		if (this->componentVariants.at(i).entityID == entityID)
		{
			delete transform2D;
			transform2D = this->componentVariants.at(i).transform2D;
		}
	}

	return *transform2D;
}