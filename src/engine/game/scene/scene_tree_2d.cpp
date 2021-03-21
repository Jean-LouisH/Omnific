//MIT License
//
//Copyright (c) 2020 Jean-Louis Haywood
//
//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software and associated documentation files (the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions:
//
//The above copyright notice and this permission notice shall be included in all
//copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//SOFTWARE.

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