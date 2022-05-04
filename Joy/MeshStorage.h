#pragma once
#include <vector>
#include <DirectXMath.h>
#include <DirectXCollision.h>

#include "Backend.h"


struct Vertex
{
	Vertex(float* Pos, float* Norm, float* Uv)
		:pos(Pos), normal(Norm), uv(Uv)
	{ }
	Vertex(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 norm, DirectX::XMFLOAT2 uv)
		:pos(pos), normal(norm), uv(uv)
	{ }
	
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT2 uv;
};

struct Mesh
{
	static const UINT Stirde = sizeof(Vertex);
	static const UINT Offset = 0;
	DirectX::BoundingBox bBox;
	void Shutdown()
	{
		diffuseTextureSRV->Release();
		vertexBuffer->Release();
	}

	ID3D11ShaderResourceView* diffuseTextureSRV;

	ID3D11Buffer* vertexBuffer;
	//ID3D11Buffer* indexBuffer;
	UINT vertexCount; // index count
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

	// ptrs or reference? ( nullptr or ERROR mesh? )
	Mesh* GetMesh(const std::string& name);
	Mesh* GetMesh(UINT index);

private:

	const std::string meshPath = "../Resources/Meshes/";
	static const UINT MeshCount = 4;
	Mesh meshes[MeshCount];
	const std::string meshNames[MeshCount] =
	{
		"Joy.obj",
		"Kub.obj",
		"Ground.obj",
		"Portal.obj"
	};

	void import(UINT index);
};