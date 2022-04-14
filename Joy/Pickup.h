#pragma once
#include "MeshStorage.h"
#include "Object.h"
#include "Collision.h"
#include "Object.h"
#include <DirectXCollision.h>
#include <vector>


struct InstancedPickups
{
	ID3D11Buffer* instancedPickupVB;
	ID3D11ShaderResourceView* transformPickupSRV;
	ID3D11ShaderResourceView* mtlPickup;
	UINT instancePickupCount;
	UINT indexPickupCount;

	void Shutdown()
	{
		transformPickupSRV->Release();
		// Don't release vertexBuffer, meshStorage owns it
		// Don't release mtl, meshStorage owns it
	}
};

class Pickup : public Object, public Collision
{
public:
	Pickup(Mesh* mesh_in, int points_in, const int itemsInScene_in);
	void isHit();

	bool get_IsElementRendered( int itemElement_in );

	// == DX Render Specifics ==
	bool LoadPickupShader();
	bool GiveInstancedObjects(Object* obj, const UINT amount);
	bool CreateInputLayout(const std::string &shaderData);
	void Clear();
	void ShutDown();
	void drawInstanced();
	void updatePositions();

private:
/* ========================================================= */
	// == DX SPECIFICS ==

	ID3D11Buffer* matrixCBuffer;
	ID3D11InputLayout* pickupIL;
	ID3D11VertexShader* pickupVS;
	ID3D11PixelShader* pickupPS;
	ID3D11RenderTargetView* const* pickupRTV;
	
	// temp
	ID3D11Buffer* pickupCam;

/* ========================================================= */
	// == PICKUP SPECIFICS ==

	int points;
	const int itemsInScene;

	DirectX::BoundingBox charBB;
	std::vector<DirectX::BoundingBox> itemsBB;

	Object pickupObj;
	std::vector<bool> isRendered;
	std::vector<InstancedPickups> instanceVec;

/* ========================================================= */
};

