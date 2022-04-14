#pragma once
#include "MeshStorage.h"
#include "Object.h"
#include "Collision.h"
#include "Object.h"
#include <DirectXCollision.h>
#include <vector>


/*

	AddPick(Object obj, float3 pos)
	{
	objects.emplace_back (obj)
	objects.back().translate(pos)
	}

	finialize()
	{
		matrices = new float4x4[objects.size()]

		for each object
			matrices[i] = object.getWorldMatrix()

		// create dynamic buffer (Backend::createDynamicBuffer)
		// create SRV 
		
	}

	update()
	{

		for each object
			object.rotate()
			matrices[i] = object.getWorldMatrix()

		Backend::UpdateBuffer(worldMatrixBuffer, matrices, sizeof(float4x4) * objects.size())

	}

	class Pickup
	private:
		float4x4 *matrices <- lika stor som objects.size()

*/

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

class Pickup : public Collision
{
public:
	Pickup(int points_in, const int itemsInScene_in);
	void isHit();

	bool get_IsElementRendered( int itemElement_in );

	// == DX Render Specifics ==
	bool CreateInputLayout(const std::string& shaderData);
	bool LoadPickupShader();
	/*bool GiveInstancedObjects(Object* obj, const UINT amount);
	void Clear();
	void ShutDown();
	void drawInstanced();
	void updatePositions();*/

private:
/* ========================================================= */
	// == DX SPECIFICS ==

	ID3D11Buffer* matrixCBuffer;
	ID3D11InputLayout* pickupIL;
	ID3D11VertexShader* pickupVS;
	ID3D11VertexShader* pickupIVS;
	ID3D11PixelShader* pickupPS;
	ID3D11RenderTargetView* const* pickupRTV;
	
	// TEMP : Camera will be fed from Camera Class.
	ID3D11Buffer* pickupCam;

/* ========================================================= */
	// == PICKUP SPECIFICS ==

	int points;
	const int itemsInScene;

	DirectX::BoundingBox charBB;
	std::vector<DirectX::BoundingBox> itemsBB;

	std::vector<Object> pickupObj;
	std::vector<bool> isRendered;
	std::vector<InstancedPickups> instanceVec;

/* ========================================================= */
};

