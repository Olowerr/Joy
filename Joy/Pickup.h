#pragma once
#include "MeshStorage.h"
#include "Object.h"
#include "Collision.h"
#include "Object.h"
#include <DirectXCollision.h>
#include <vector>


class Pickup : public Object, public Collision
{
public:
	Pickup(Mesh* mesh_in, int points_in, const int itemsInScene_in);
	void isHit();

	bool get_IsElementRendered( int itemElement_in );
	void addObjectToVector();
private:
	int points;
	const int itemsInScene;

	DirectX::BoundingBox charBB;
	std::vector<DirectX::BoundingBox> itemsBB;

	Object pickupObj;
	std::vector<bool> isRendered;
	std::vector<Object> instanceVec;

};

