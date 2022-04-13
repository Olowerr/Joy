#include "Pickup.h"

Pickup::Pickup(std::vector<Mesh*> mesh_in, int points_in, const int itemsInScene_in)
	: Object(mesh_in) // TODO: Fix, but how?
{
	this->points = points_in;

	this->pickupObjs.reserve(itemsInScene_in);
	this->isRendered.reserve(itemsInScene_in);
	this->itemsBB.reserve(itemsInScene_in);

	for (unsigned int i = 0; i < pickupObjs.size(); i++)
	{
		/*
		* How do I iniitialize Objects from meshes?
		* Should I? Or just take in an object vector?
		*/
	}
		
	for (unsigned int i = 0; i < pickupObjs.size(); i++)
	{
		/* 
		* Fetch BB of this specific object.Something that needs constant updating ?
		* Is collision handling the constant update of the BB?
		*/
	}

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
