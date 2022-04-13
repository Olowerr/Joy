#pragma once
#include "MeshStorage.h"
#include "Object.h"
#include "Collision.h"
#include <DirectXCollision.h>
#include <vector>


class Pickup : public Object, public Collision
{
public:
	Pickup(std::vector<Mesh*> mesh_in, int points_in, const int itemsInScene_in);
	void isHit();

	bool get_IsElementRendered( int itemElement_in );
private:
	int points;

	DirectX::BoundingBox charBB;
	std::vector<DirectX::BoundingBox> itemsBB;

	std::vector<Object> pickupObjs;
	std::vector<bool> isRendered;

};

