#pragma once
#include <DirectXCollision.h>
#include <cmath>
#include <iostream>
#include "Object.h"


namespace DX = DirectX;
class Collision
{
public:
	Collision();
	void collided(Object* charBbox, Object* targetBbox);
	void distToComp(DX::XMVECTOR distToMove, Object& charBbox);
	bool hitItem(Object* charBbox, Object* targetBbox);
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