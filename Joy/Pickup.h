#pragma once
#include "MeshStorage.h"
#include "Object.h"
#include "Character.h"
#include "CameraBase.h"
#include "ObjectHelper.h"

#include <DirectXCollision.h>
#include <vector>
#include <algorithm>

class Pickup
{
public:
	/* ==CONSTRUCTORS_DESTRUCTOR_ETC======================================================= */
	Pickup(Character& joy);

	/* ==PICKUPSPECIFIC_FUNCTIONS======================================================= */
	void AddObject(float pX_in, float pY_in, float pZ_in);	// Adds pickup at position specified.
	void FetchPickupMesh(TempMeshStorage& meshStorage_in);	// Fetches the mesh meant to be the pickup.
	bool isHit();											// Collision check and handling.
	UINT getPickupsCaught();								// Returns amounts of pickups that has been collided with.
	void Reset();

	/* ==DXSPECIFIC_FUNCTIONS======================================================= */
	bool CreateSRV_CreateMatrixSB();						// Creates SRV and Matrix Structured Buffer
	void DrawPickupInstances(Camera* cam_in);				// Draws out the pickups
	void UpdateMatrices();									// Updates Matrices (Rotates the pickup)
	void ShutDown();										// Releases DX Specifics and clears vectors.

private:
	/* ==DXSPECIFIC_MEMBERS======================================================= */
	DirectX::XMFLOAT4X4* matrices;							// Matrices
	ID3D11Buffer* matrixSBuffer;							// Matrix Structure Buffer (For rotation)

	ID3D11VertexShader* pickupVS;							// VertexShader
	ID3D11PixelShader* pickupPS;							// Pixelshader

	ID3D11ShaderResourceView* pickupTransSRV;				// Transform SRV

	/* ==PICKUPSPECIFIC_MEMBER======================================================= */
	Mesh* pickupMesh;										// The mesh of which the pickups use.
	Character& joy;											// Reference to the playable character, used for collision.

	UINT pickupsRendered;									// Amount of pickups that should be rendered.
	std::vector<Object*> pickupObjs;						// All the pickups.

	/* ==DXSPECIFIC_FUNCTIONS======================================================= */
	bool LoadPickupShader();								// Loads PS and VS.


	// NOTES: Pickups are implemented by using
	//		[ Load:		| "FetchPickupMesh" -> "AddObject" -> "CreateSRV_CreateMatrixSB"] 
	//		[ Update:	| "Update Matrices" -> "isHit" ]
	//		[ Render:	| "DrawPickupInstance" ]
	//		[ Shutdown: | "Shutdown"]
};