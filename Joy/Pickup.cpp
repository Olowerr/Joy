#include "Pickup.h"

Pickup::Pickup(Mesh mesh_in, int points_in)
{
	this->pickupMeshes = mesh_in;
	this->points = points_in;
}

bool Pickup::isHit()
{
	if (this->hitItem(charBB, itemBB))
	{

	}
	return false;
}
