#include "Pickup.h"

Pickup::Pickup(std::vector<Mesh> pickupMeshes_in, std::vector<bool> isRendered, int points_in)
{
	this->points = points_in;
	this->isRendered = isRendered_in;
	this->pickupMeshes = pickupMeshes_in;
}

bool Pickup::isHit()
{
	if (this->hitItem(charBB, itemBB))
	{
		this->isRendered = false;
	}
	return false;
}

void setRendered(int pickupElement)
{
	this->pickupMeshes.erase(pickupElement);
}
