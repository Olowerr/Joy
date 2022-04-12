#pragma once
#include "MeshStorage.h"
#include "Object.h"
#include "Collision.h"
#include <DirectXCollision.h>
#include <vector>


class Pickup : public Collision
{
public:
	Pickup(std::vector<Mesh> pickupMeshes_in, std::vector<bool> isRendered, int points_in);
	bool isHit();
private:
	int points;
	std::vector<Mesh> pickupMeshes;
	std::vector<bool> isRendered;

	DirectX::BoundingBox charBB;
	DirectX::BoundingBox itemBB;
};

