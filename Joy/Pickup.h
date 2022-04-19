#pragma once
#include "MeshStorage.h"
#include "Object.h"
#include "Collision.h"

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

class Pickup : public Collision
{
public:
/* ==CONSTRUCTORS======================================================= */
	Pickup(TempMeshStorage& meshStorage, int points_in);

/* ==PICKUPSPECIFIC_FUNCTIONS======================================================= */
	void isHit();
	bool Get_IsElementRendered( int itemElement_in );

	void AddObject(float pX_in, float pY_in, float pZ_in);
	void UpdateMatrices();

/* ==DXSPECIFIC_FUNCTIONS======================================================= */
	bool CreateInputLayout(const std::string& shaderData);
	bool CreateSRV_CreateMatrixCB();
	bool LoadPickupShader();
	bool ShutDown();

	void DrawPickupInstances();

private:
/* ==DXSPECIFIC_MEMBERS======================================================= */
	DirectX::XMFLOAT4X4 *matrices;
	ID3D11Buffer* matrixCBuffer;

	ID3D11InputLayout* pickupIL;

	ID3D11VertexShader* pickupVS;
	ID3D11PixelShader* pickupPS;

	ID3D11RenderTargetView* const* pickupRTV;
	ID3D11ShaderResourceView* pickupTransSRV;
	
	// TEMP : Camera will be fed from Camera Class.
	ID3D11Buffer* pickupCam;

/* ==PICKUPSPECIFIC_MEMBER======================================================= */
	int points;

	Mesh* pickupMesh;

	std::vector<Object> pickupObjs;
	std::vector<bool> isRendered;

	DirectX::BoundingBox charBB;
	std::vector<DirectX::BoundingBox> itemsBB; // NOTE: Will be replaced entirely by the mesh Bounding Box and the collision hitbox function.

};

