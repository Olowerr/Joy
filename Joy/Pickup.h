#pragma once
#include "MeshStorage.h"
#include "Object.h"
#include "Collision.h"

#include <DirectXCollision.h>
#include <vector>
#include <algorithm>

class Pickup : public Collision
{
public:
/* ==CONSTRUCTORS======================================================= */
	Pickup(TempMeshStorage& meshStorage, UINT points_in);

/* ==PICKUPSPECIFIC_FUNCTIONS======================================================= */
	void AddObject(float pX_in, float pY_in, float pZ_in);
	void isHit(); // Collission Checks
	UINT getPoints();
	UINT getAmountHitPickups();
	
/* ==DXSPECIFIC_FUNCTIONS======================================================= */
	bool CreateSRV_CreateMatrixCB();
	void DrawPickupInstances();
	void UpdateMatrices();
	void ShutDown();

private:
/* ==DXSPECIFIC_MEMBERS======================================================= */
	DirectX::XMFLOAT4X4 *matrices;
	ID3D11Buffer* matrixCBuffer;

	ID3D11InputLayout* pickupIL;

	ID3D11VertexShader* pickupVS;
	ID3D11PixelShader* pickupPS;

	ID3D11ShaderResourceView* pickupTransSRV;
	
	// TEMP : Camera will be fed from Camera Class.
	ID3D11Buffer* pickupCam;

/* ==PICKUPSPECIFIC_MEMBER======================================================= */	
	UINT pickupsRendered;
	UINT points;

	Mesh* pickupMesh;

	std::vector<Object*> pickupObjs;

	DirectX::BoundingBox charBB;
	std::vector<DirectX::BoundingBox> itemsBB; // TODO : Connect with the new Boundign boxes from the importer.

/* ==DXSPECIFIC_FUNCTIONS======================================================= */
	bool CreateInputLayout(const std::string& shaderData);
	bool LoadPickupShader();

};

