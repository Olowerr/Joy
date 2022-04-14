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
	Pickup(TempMeshStorage& meshStorage, int points_in, const int itemsInScene_in);

/* ==PICKUPSPECIFIC_FUNCTIONS======================================================= */
	void isHit();
	bool get_IsElementRendered( int itemElement_in );

	void AddPickupObjects(Object * obj, float position[3]);
	void UpdateMatrices();

/* ==DXSPECIFIC_FUNCTIONS======================================================= */
	bool CreateInputLayout(const std::string& shaderData);
	bool LoadPickupShader();
	bool GiveInstancedObjects(std::vector<Object> obj, const UINT amount);


private:
/* ==DXSPECIFIC_MEMBERS======================================================= */
	ID3D11Buffer* matrixCBuffer;
	ID3D11InputLayout* pickupIL;
	ID3D11VertexShader* pickupVS;
	ID3D11PixelShader* pickupPS;
	ID3D11RenderTargetView* const* pickupRTV;
	
	// TEMP : Camera will be fed from Camera Class.
	ID3D11Buffer* pickupCam;

/* ==PICKUPSPECIFIC_MEMBER======================================================= */
	int points;
	const int itemsInScene;
	std::vector<Object> pickupObjs;
	std::vector<bool> isRendered;





	DirectX::BoundingBox charBB;
	std::vector<DirectX::BoundingBox> itemsBB;

	

};

