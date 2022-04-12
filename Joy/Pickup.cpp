#include "Pickup.h"

Pickup::Pickup(std::vector<Mesh> pickupMeshes_in, std::vector<bool> isRendered_in, int points_in)
{
	this->points = points_in;
	this->isRendered = isRendered_in;
	this->pickupMeshes = pickupMeshes_in;
}

bool Pickup::isHit()
{
	bool isHitThisFrame = false;

	for (int i = 0; i < this->pickupMeshes.size(); i++)
	{
		if (this->hitItem(charBB, itemBB))
		{
			this->isRendered[i] = false;
			isHitThisFrame = true;
		}
	}
	return isHitThisFrame;
}