#pragma once
#include <vector>
#include <DirectXMath.h>
#include <DirectXCollision.h>
#include "StoredData.h"

#include "Backend.h"

struct Mesh
{
	static const UINT Stirde = sizeof(JOY::Vertex);
	static const UINT Offset = 0;
	DirectX::BoundingBox bBox;
	void Shutdown()
	{
		diffuseTextureSRV->Release();
		vertexBuffer->Release();
	}

	ID3D11ShaderResourceView* diffuseTextureSRV;

	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer = nullptr;
	UINT indexCount;
};

/*

	Most of this file is temporary, will change with .JOY lib
	Most of this file is temporary, will change with .JOY lib
	Most of this file is temporary, will change with .JOY lib
	Most of this file is temporary, will change with .JOY lib
	Most of this file is temporary, will change with .JOY lib
	Most of this file is temporary, will change with .JOY lib

*/

class TempMeshStorage
{
public:

	TempMeshStorage();
	~TempMeshStorage();

	void LoadAll();
	void UnLoadAll();

	void LoadMenuObjects();
	void LoadEasyObjects();

	// ptrs or reference? ( nullptr or ERROR mesh? )
	Mesh* GetMesh(const std::string& name);
	Mesh* GetMesh(UINT index);

	Mesh fbxMesh;
private:

	const std::string meshPath = "../Resources/Meshes/";
	static const UINT MeshCount = 11;
	Mesh meshes[MeshCount];
	const std::string meshNames[MeshCount] =
	{
		"Joy.obj",
		"Kub.obj",
		"Ground.obj",
		"Portal.obj",
		"Wall.obj",
		"Wall2.obj",
		"sphere.obj",
		"FirstTree.obj",
		"HighscoreScreen.obj",
		"Frame.obj",
		"Future_Bench.obj"
	};



	void import(UINT index);
};