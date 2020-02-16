#include "Scene.hpp"

Lilliputian::Scene::Scene()
{
	Subscene2D subscene2D;
	std::stack<Subscene2D> subscene2Ds;

	subscene2Ds.push(subscene2D);
	this->subscene2Ds.push_back(subscene2Ds);
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