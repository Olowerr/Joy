#pragma once
#include <DirectXCollision.h>
#include <cmath>


namespace DX = DirectX;
class Collision
{
public:
	Collision();
	void collided(const DX::BoundingBox& charBbox, const DX::BoundingBox& targetBbox);
	void distToComp(DX::XMVECTOR distToMove);
private:
	DX::BoundingBox bBox;
	DX::BoundingBox dBox;
	DX::XMVECTOR distToMoveX;
	DX::XMVECTOR distToMoveY;
	DX::XMVECTOR distToMoveZ;
	float xIntDist;
	float yIntDist;
	float zIntDist;
};