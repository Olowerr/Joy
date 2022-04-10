#pragma once
#include <vector>
#include <DirectXMath.h>

#include "Backend.h"


struct Vertex
{
	Vertex(float* Pos, float* Uv, float* Norm)
		:pos(Pos), uv(Uv), normal(Norm)
	{ }
	Vertex(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT2 uv, DirectX::XMFLOAT3 norm)
		:pos(pos), uv(uv), normal(norm)
	{ }
	
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT2 uv;
	DirectX::XMFLOAT3 normal;
};

struct Mesh
{
	static const UINT Stirde = sizeof(Vertex);
	static const UINT Offset = 0;

	// temp, should maybe be in Object
	ID3D11ShaderResourceView* diffuseTextureSRV;

	ID3D11Buffer* vertexBuffer;
	//ID3D11Buffer* indexBuffer;
	UINT vertexCount; // index count
};

class MeshStorage
{
public:
	MeshStorage();
	void Shutdown();
	
	void LoadAll();

	// ptrs or reference? ( nullptr or ERROR mesh? )
	Mesh* GetMesh(const std::string& name);
	Mesh* GetMesh(UINT index);

private:


	const std::string meshPath = "../Resources/Meshes/";
	static const UINT MeshCount = 2;
	Mesh meshes[MeshCount];
	const std::string meshNames[MeshCount] =
	{
		"cat.obj"
	};


	void import(UINT index);
};