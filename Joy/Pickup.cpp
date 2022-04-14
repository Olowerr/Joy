#include "Pickup.h"

Pickup::Pickup(Mesh* mesh_in, int points_in, const int itemsInScene_in)
	: Object(mesh_in), pickupObj(mesh_in), itemsInScene(itemsInScene_in)
{
	this->points = points_in;

	this->isRendered.reserve(itemsInScene);
	this->itemsBB.reserve(itemsInScene);
	this->instanceVec.reserve(itemsInScene);


	for (unsigned int i = 0; i < isRendered.size(); i++)
		this->isRendered[i] = true;
}

void Pickup::isHit()
{

	for (unsigned int i = 0; i < isRendered.size(); i++)
	{
		if (hitItem(charBB, itemsBB[i]))
		{
			this->isRendered[i] = false;
		}
	}
}

bool Pickup::get_IsElementRendered(int itemElement_in)
{
	return this->isRendered[itemElement_in];
}

void Pickup::addObjectToVector()
{
	for (unsigned int i = 0; i < this->instanceVec.size(); i++)
	{
		this->instanceVec[i] = this->pickupObj;
	}
}


