#include "Pickup.h"

Pickup::Pickup(Mesh mesh_in, int points_in)
{
	this->pickupMeshes = mesh_in;
	this->points = points_in;
	this->isRendered = true;
}

bool Pickup::isHit()
{
	if (this->hitItem(charBB, itemBB))
	{
		this->isRendered = false;
		//hi
	}
	return false;
}
