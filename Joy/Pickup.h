#pragma once
#include "MeshStorage.h"
#include "Object.h"
#include "Collision.h"
#include <DirectXCollision.h>


class Pickup : public Collision
{
public:
	Pickup(Mesh mesh, int points_in);
	bool isHit();
private:
	int points;
	Mesh pickupMeshes;
	DirectX::BoundingBox charBB;
	DirectX::BoundingBox itemBB;


};

