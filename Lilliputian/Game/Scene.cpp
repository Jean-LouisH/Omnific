#include "Scene.hpp"

Lilliputian::Scene::Scene()
{
	this->entityIDCount = 0;
}

void Lilliputian::Scene::addNewSubscene2D()
{
	Subscene2D subscene2D;
	Stack<Subscene2D> subscene2Ds;

	subscene2Ds.push(subscene2D);
	this->subscene2Ds.push_back(subscene2Ds);
}

Lilliputian::Entity2D Lilliputian::Scene::newEntity2D()
{
	Entity2D entity2D(this->entityIDCount);
	this->entityIDCount++;
	return entity2D;
}

void Lilliputian::Scene::addEntity2D(Entity2D entity2D)
{
	this->subscene2Ds.at(this->subscene2Ds.size() - 1).top().addEntity2D(entity2D);
}

void Lilliputian::Scene::executeFrameLogic()
{
	for (int i = 0; i < this->subscene2Ds.size(); i++)
		this->subscene2Ds.at(i).top().executeFrameLogic();
}