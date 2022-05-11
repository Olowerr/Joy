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
	void collided(Object* character, Object* target, int index);
	void distToComp(DX::XMVECTOR distToMove, Object& charBbox);
	bool HitObject(Object* charBbox, Object* targetBbox, int index);
	bool hitItem(const DX::BoundingBox& charbBox, const DX::BoundingBox& itemBbox);
	bool getCollidedY();

private:
	DX::BoundingBox bBox;
	DX::BoundingBox dBox;
	DX::XMVECTOR distToMoveX;
	DX::XMVECTOR distToMoveY;
	DX::XMVECTOR distToMoveZ;
	float xIntDist;
	float yIntDist;
	float zIntDist;
	bool stopFall;
	bool dontStopMovement;
	bool collidedY;
};